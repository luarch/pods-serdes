#pragma once

#include <pods-serdes/sourceGenerator.hpp>

namespace luarch::pods_serdes::cxx {

struct FromJsonGenerator : public SourceGenerator<FromJsonGenerator>
{
    constexpr static const char *Prologue()
    {
        return "// From JSON converters\n";
    }

    constexpr static const char *Epilogue()
    {
        return "";
    }

    static const char *StructStart(const std::string &structName)
    {
        static char buffer[1 << 10];
        auto format =
R"(inline void from_json(const nlohmann::json &j, ::%s &data)
{)";

        sprintf(buffer, format, structName.data());
        return buffer;
    }

    static const char *StructEnd(const std::string &structName)
    {
        return "}";
    }

    static const char *Member(const std::string &structName, const MemberField &member, bool isLast)
    {
        static char buffer[1 << 10];
        auto format =
R"(    j.at("%s").get_to(data.%s);)";

        sprintf(buffer, format, member.name.data(), member.name.data());
        return buffer;
    }
};

} // namespace luarch::pods_serdes::cxx
