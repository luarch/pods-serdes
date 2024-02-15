#pragma once

#include <pods-serdes/sourceGenerator.hpp>

namespace luarch::pods_serdes::cxx {

struct ToStringGenerator : public SourceGenerator<ToStringGenerator>
{
    constexpr static const char *Prologue()
    {
        return "// To String converters\n";
    }

    constexpr static const char *Epilogue()
    {
        return "\n";
    }

    static const char *StructStart(const std::string &structName)
    {
        static char buffer[1 << 10];
        auto format = 
R"(inline std::string to_string(const ::%s &data)
{
    std::ostringstream oss;
    oss << "%s: {";)";

        sprintf(buffer, format, structName.data(), structName.data());
        return buffer;
    }

    static const char *StructEnd(const std::string &structName)
    {
        return
R"(    oss << "}";
    return oss.str();
})";
    }

    static const char *Member(const std::string &structName, const MemberField &member, bool isLast)
    {
        static char buffer[1 << 10];

        if (member.kind == MemberKind::CharArray)
        {
            auto format =
R"(    oss << "%s :\"" << std::string_view{data.%s, %d} << "\",";)";
            auto formatIsLast =
R"(    oss << "%s :\"" << std::string_view{data.%s, %d} << "\"";)";

            sprintf(buffer, isLast ? formatIsLast : format, member.name.data(), member.name.data(), member.arraySize);
        }
        else if (member.kind == MemberKind::Char)
        {
            auto format =
R"A(    oss << "%s :\"" << data.%s << "\"" << " (0x" << std::hex << (int) data.%s << ")" << std::dec << ",";)A";
            auto formatIsLast =
R"A(    oss << "%s :\"" << data.%s << "\"" << " (0x" << std::hex << (int) data.%s << ")" << std::dec;)A";
            sprintf(buffer, isLast ? formatIsLast : format, member.name.data(), member.name.data(), member.name.data());
        }
        else if (member.kind == MemberKind::IntegralArray || member.kind == MemberKind::FloatingPointArray)
        {
            // TODO integer and fp arrays are not supported for ToString serialization
            auto format =
R"(    oss << "%s :<UnsupportedArray>" << ",";)";
            auto formatIsLast =
R"(    oss << "%s :<UnsupportedArray>";)";
            sprintf(buffer, isLast ? formatIsLast : format, member.name.data(), member.name.data());
        }
        else
        {
            auto format =
R"(    oss << "%s :" << data.%s << ",";)";
            auto formatIsLast =
R"(    oss << "%s :" << data.%s;)";
            sprintf(buffer, isLast ? formatIsLast : format, member.name.data(), member.name.data());
        }

        return buffer;
    }
};

} // namespace luarch::pods_serdes::cxx
