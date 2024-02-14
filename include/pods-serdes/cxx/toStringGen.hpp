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
        return "";
    }

    static const char *StructStart(const std::string &structName)
    {
        return "";
    }

    static const char *StructEnd(const std::string &structName)
    {
        return "";
    }

    static const char *Member(const std::string &structName, const MemberField &member, bool isLast)
    {
        return "";
    }
};

} // namespace luarch::pods_serdes::cxx
