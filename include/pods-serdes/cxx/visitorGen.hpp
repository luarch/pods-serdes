#pragma once

#include <pods-serdes/sourceGenerator.hpp>

namespace luarch::pods_serdes::cxx {

struct VisitorGenerator : public SourceGenerator<VisitorGenerator>
{
    constexpr static const char *Prologue()
    {
        return "// Programmable visitors\n";
    }

    constexpr static const char *Epilogue()
    {
        return "\n";
    }

    static const char *StructStart(const std::string &structName)
    {
        static char buffer[1 << 10];
        auto format =
R"(template <typename Visitor>
inline void visit(Visitor &&visitor, const ::%s &data)
{
    visitor.StartStruct("%s");)";

        sprintf(buffer, format, structName.data(), structName.data());
        return buffer;
    }

    static const char *StructEnd(const std::string &structName)
    {
        return R"(    visitor.EndStruct();
})";
    }

    static const char *Member(const std::string &structName, const MemberField &member, bool isLast)
    {
        static char buffer[1 << 10];
        auto format = "    visitor.Member(\"%s\", data.%s);";

        sprintf(buffer, format, member.name.data(), member.name.data());
        return buffer;
    }
};

} // namespace luarch::pods_serdes::cxx
