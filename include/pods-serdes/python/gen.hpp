#pragma once

#include <pods-serdes/sourceGenerator.hpp>

#include <loguru.hpp>
#include <fstream>

namespace luarch::pods_serdes::python {

struct DataclassGenerator : public SourceGenerator<DataclassGenerator>
{
    constexpr static const char *Prologue()
    {
        return "from dataclasses import dataclass\n"
               "from typing import List\n";
    }

    constexpr static const char *Epilogue()
    {
        return "";
    }

    static const char *StructStart(const std::string &structName)
    {
        static char buffer[1 << 10];
        sprintf(buffer,
                "@dataclass\n"
                "class %s:",
                structName.data());

        return buffer;
    }

    static const char *StructEnd(const std::string &structName)
    {
        return "\n";
    }

    static const char *Member(const std::string &structName, const MemberField &member, bool isLast)
    {
        static char buffer[1 << 10];
        const char *format;

        switch (member.kind)
        {
        case MemberKind::Integral:
            format = "    %s: int = 0";
            sprintf(buffer, format, member.name.data());
            break;
        case MemberKind::FloatingPoint:
            format = "    %s: float = 0.0";
            sprintf(buffer, format, member.name.data());
            break;
        case MemberKind::Boolean:
            format = "    %s: bool = False";
            sprintf(buffer, format, member.name.data());
            break;
        case MemberKind::Char:
        case MemberKind::CharArray:
            format = "    %s: str = ''";
            sprintf(buffer, format, member.name.data());
            break;
        case MemberKind::IntegralArray:
            format = "    %s: List[int] = [0] * %d";
            sprintf(buffer, format, member.name.data(), member.arraySize);
            break;
        case MemberKind::FloatingPointArray:
            format = "    %s: List[float] = [0.0] * %d";
            sprintf(buffer, format, member.name.data(), member.arraySize);
            break;
        default:
            LOG_F(ERROR, "Unknown member kind: %d", (int)member.kind);
            return "";
        }

        sprintf(buffer, format, member.name.data(), member.type.data());
        return buffer;
    }
};


static bool Generate(const StructsMap &structs, const std::string &outputFile)
{
    std::ofstream fs{outputFile, std::ios::out | std::ios::trunc};
    if (!fs.is_open())
    {
        LOG_F(ERROR, "Failed to open file: %s", outputFile.data());
        return false;
    }

    DataclassGenerator::Generate(structs, fs);
    return true;
}

}
