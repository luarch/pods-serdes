#pragma once

#include <pods-serdes/utilities.hpp>

#include <loguru.hpp>
#include <optional>
#include <unordered_set>

namespace luarch::pods_serdes {

class TranslationUnitTraversor
{
public:
    TranslationUnitTraversor(const std::string &translationUnitFile,
                             const std::vector<std::string> &translationUnitArgs, StructsMap &structs)
        : mStructs(structs), mTranslationUnitFile(std::move(translationUnitFile)),
          mTranslationUnitArgs(translationUnitArgs), mIndex(clang_createIndex(0, 0))
    {}

    ~TranslationUnitTraversor()
    {
        clang_disposeTranslationUnit(mTranslationUnit);
        clang_disposeIndex(mIndex);
    }

    TranslationUnitTraversor(const TranslationUnitTraversor &) = delete;
    TranslationUnitTraversor &operator=(const TranslationUnitTraversor &) = delete;
    TranslationUnitTraversor(TranslationUnitTraversor &&) = delete;
    TranslationUnitTraversor &operator=(TranslationUnitTraversor &&) = delete;

    bool Traverse()
    {
        if (!ParseTranslationUnit())
        {
            return false;
        }

        TraverseStructs();
        return mIsParsed;
    }

    const std::string &GetTranslationUnitFile() const
    {
        return mTranslationUnitFile;
    }

private:
    bool ParseTranslationUnit()
    {
        std::vector<const char *> commandLineArgsStorage;
        for (const auto &arg : mTranslationUnitArgs)
        {
            commandLineArgsStorage.push_back(arg.data());
        }

        mTranslationUnit =
            clang_parseTranslationUnit(mIndex, mTranslationUnitFile.data(), commandLineArgsStorage.data(),
                                       commandLineArgsStorage.size(), nullptr, 0, CXTranslationUnit_None);

        if (!mTranslationUnit)
        {
            LOG_F(ERROR, "Failed to parse translation unit: %s", mTranslationUnitFile.c_str());
            return false;
        }
        auto numDiags = clang_getNumDiagnostics(mTranslationUnit);
        for (size_t i = 0; i < numDiags; ++i)
        {
            auto diag = clang_getDiagnostic(mTranslationUnit, i);
            auto diagStr = CXStringWrapper(clang_getDiagnosticSpelling(diag));
            LOG_F(WARNING, "Diagnostic for %s: %s", mTranslationUnitFile.data(), diagStr.data());
        }

        return true;
    }

    void TraverseStructs()
    {
        auto rootCursor = clang_getTranslationUnitCursor(mTranslationUnit);
        struct TraverseState
        {
            std::optional<std::string> currentStructName{};
            bool success{true};
            StructsMap &structs;
        } traverseState{{}, true, mStructs};

        auto visitor = [](CXCursor current, CXCursor parent, CXClientData data) {
            TraverseState &state = *reinterpret_cast<TraverseState *>(data);

            auto kind = clang_getCursorKind(current);
            if (kind == CXCursor_StructDecl)
            {
                auto name = CXStringWrapper(clang_getCursorSpelling(current));
                if (clang_getCursorKind(parent) != CXCursor_TranslationUnit)
                {
                    LOG_F(WARNING, "Nested struct %s is detected but not supported", name.data());
                    return CXChildVisit_Continue;
                }

                auto type = clang_getCursorType(current);
                if (!clang_isPODType(type))
                {
                    LOG_F(WARNING, "Non-POD struct %s is detected but not supported", name.data());
                    return CXChildVisit_Continue;
                }

                LOG_F(1, "Found struct: %s", name.data());
                state.currentStructName = name;
                return CXChildVisit_Recurse;
            }
            else if (kind == CXCursor_FieldDecl)
            {
                auto parentName = CXStringWrapper(clang_getCursorSpelling(parent));
                auto name = CXStringWrapper(clang_getCursorSpelling(current));
                if (state.currentStructName != parentName.data())
                {
                    // TODO handle unknown field for unknown struct
                    LOG_F(ERROR, "Found item to unknown struct: %s", name.data());
                    state.success = false;
                    return CXChildVisit_Break;
                }

                auto type = clang_getCursorType(current);
                if (type.kind == CXType_Typedef)
                {
                    auto typeDeclCursor = clang_getTypeDeclaration(type);
                    type = clang_getTypedefDeclUnderlyingType(typeDeclCursor);
                }

                if (type.kind == CXType_Invalid)
                {
                    LOG_F(ERROR, "Failed to get type for %s.", name.data());
                    state.success = false;
                    return CXChildVisit_Break;
                }

                auto memberField = GetMemberField(name, type);
                if (!memberField)
                {
                    state.success = false;
                    return CXChildVisit_Break;
                }

                state.structs[state.currentStructName.value()].push_back(memberField.value());
                LOG_F(1, "In struct %s, found item: %s", state.currentStructName->data(), to_string(memberField.value()).data());
            }

            return CXChildVisit_Continue;
        };

        clang_visitChildren(rootCursor, visitor, &traverseState);

        if (!traverseState.success)
        {
            LOG_F(ERROR, "Failed to traverse translation unit: %s", mTranslationUnitFile.c_str());
        }
        else
        {
            LOG_F(INFO, "Traversed translation unit: %s", mTranslationUnitFile.c_str());
            mIsParsed = true;
        }
    }

    static std::optional<MemberField> GetMemberField(const std::string &name, CXType type)
    {
        MemberField result{.name = name, .type = CXStringWrapper(clang_getTypeSpelling(type))};

        bool unsupportedType = false;
        if (IsMemberKind<MemberKind::Integral>(type.kind))
        {
            result.kind = MemberKind::Integral;
        }
        else if (IsMemberKind<MemberKind::FloatingPoint>(type.kind))
        {
            result.kind = MemberKind::FloatingPoint;
        }
        else if (IsMemberKind<MemberKind::Boolean>(type.kind))
        {
            result.kind = MemberKind::Boolean;
        }
        else if (IsMemberKind<MemberKind::Char>(type.kind))
        {
            result.kind = MemberKind::Char;
        }
        else if (type.kind == CXType_ConstantArray)
        {
            auto arrayElementTypeKind = clang_getElementType(type).kind;
            if (IsMemberKind<MemberKind::Integral>(arrayElementTypeKind))
            {
                result.kind = MemberKind::IntegralArray;
            }
            else if (IsMemberKind<MemberKind::FloatingPoint>(arrayElementTypeKind))
            {
                result.kind = MemberKind::FloatingPointArray;
            }
            else if (IsMemberKind<MemberKind::Char>(arrayElementTypeKind))
            {
                result.kind = MemberKind::CharArray;
            }
            else
            {
                unsupportedType = true;
            }
            result.arraySize = clang_getArraySize(type);
        }
        else
        {
            unsupportedType = true;
        }

        if (unsupportedType)
        {
            LOG_F(WARNING, "Unsupported type: %s for %s", result.type.data(), result.name.data());
            return std::nullopt;
        }
        return result;
    }

    const std::string mTranslationUnitFile;
    const std::vector<std::string> mTranslationUnitArgs;
    StructsMap &mStructs;

    CXIndex mIndex{nullptr};
    CXTranslationUnit mTranslationUnit{nullptr};
    bool mIsParsed{false};
};

} // namespace luarch::pods_serdes
