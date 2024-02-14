#pragma once

#include <pods-serdes/sourceGenerator.hpp>

#include <loguru.hpp>
#include <fstream>

namespace luarch::pods_serdes::python {

struct DataclassGenerator : public SourceGenerator<DataclassGenerator>
{
    constexpr static const char *Prologue()
    {
        return "";
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
