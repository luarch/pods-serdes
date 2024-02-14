#pragma once

#include <pods-serdes/sourceGenerator.hpp>

namespace luarch::pods_serdes::cxx {

struct ToJsonGenerator : public SourceGenerator<ToJsonGenerator>
{
    constexpr static const char *Prologue()
    {
        return "// To JSON converters\n";
    }

    constexpr static const char *Epilogue()
    {
        return "";
    }

    static const char *StructStart(const std::string &structName)
    {
        static char buffer[1 << 10];
        auto format =
R"(inline nlohmann::json to_json(const ::%s &data)
{
    return nlohmann::json{)";

        sprintf(buffer, format, structName.data());
        return buffer;
    }

    static const char *StructEnd(const std::string &structName)
    {
        return
R"(    };
})";
    }

    static const char *Member(const std::string &structName, const MemberField &member, bool isLast)
    {
        static char buffer[1 << 10];
        auto format =
R"(        {"%s", data.%s},)";

        sprintf(buffer, format, member.name.data(), member.name.data());
        return buffer;
    }
};

} // namespace luarch::pods_serdes::cxx
