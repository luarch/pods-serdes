#pragma once

#include <clang-c/Index.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace luarch::pods_serdes {

class CXStringWrapper
{
public:
    CXStringWrapper(CXString cxString) : mCXString(cxString) {}

    ~CXStringWrapper()
    {
        clang_disposeString(mCXString);
    }

    const char *data() const
    {
        return clang_getCString(mCXString);
    }

    operator std::string() const
    {
        return data();
    }

private:
    CXString mCXString;
};

enum class MemberKind
{
    Integral,
    FloatingPoint,
    Boolean,
    Char,

    // Array types
    IntegralArray,
    FloatingPointArray,
    CharArray,
};

static std::string to_string(MemberKind kind)
{
    switch (kind)
    {
    case MemberKind::Integral:
        return "Integral";
    case MemberKind::FloatingPoint:
        return "FloatingPoint";
    case MemberKind::Boolean:
        return "Boolean";
    case MemberKind::Char:
        return "Char";
    case MemberKind::IntegralArray:
        return "IntegralArray";
    case MemberKind::FloatingPointArray:
        return "FloatingPointArray";
    case MemberKind::CharArray:
        return "CharArray";
    default:
        return "<INVALID>";
    }
}

template <MemberKind kind>
constexpr bool IsMemberKind(CXTypeKind typeKind);

template <>
inline constexpr bool IsMemberKind<MemberKind::Integral>(CXTypeKind typeKind)
{
    return typeKind == CXType_Int || typeKind == CXType_UInt || typeKind == CXType_Long || typeKind == CXType_ULong ||
           typeKind == CXType_LongLong || typeKind == CXType_ULongLong;
}

template <>
inline constexpr bool IsMemberKind<MemberKind::FloatingPoint>(CXTypeKind typeKind)
{
    return typeKind == CXType_Float || typeKind == CXType_Double || typeKind == CXType_LongDouble;
}

template <>
inline constexpr bool IsMemberKind<MemberKind::Boolean>(CXTypeKind typeKind)
{
    return typeKind == CXType_Bool;
}

template <>
inline constexpr bool IsMemberKind<MemberKind::Char>(CXTypeKind typeKind)
{
    return typeKind == CXType_Char_S || typeKind == CXType_SChar || typeKind == CXType_Char_U ||
           typeKind == CXType_UChar || typeKind == CXType_WChar || typeKind == CXType_Char16 ||
           typeKind == CXType_Char32;
}

struct MemberField
{
    std::string name;
    std::string type;
    MemberKind kind;
    size_t arraySize;
};

inline std::string to_string(const MemberField &member)
{
    return member.name + " : " + member.type + " <" + to_string(member.kind) + "[" + std::to_string(member.arraySize) +
           "]" + ">";
}

using StructsMap = std::unordered_map<std::string, std::vector<MemberField>>;

} // namespace luarch::pods_serdes
