#pragma once

#include <pods-serdes/cxx/fromJsonGen.hpp>
#include <pods-serdes/cxx/toJsonGen.hpp>
#include <pods-serdes/cxx/toStringGen.hpp>
#include <pods-serdes/cxx/visitorGen.hpp>

#include <filesystem>
#include <fstream>
#include <loguru.hpp>

namespace luarch::pods_serdes::cxx {

static bool Generate(const StructsMap &structs, std::vector<std::string> &inputFiles, const std::string &outputFile)
{
    std::ofstream fs{outputFile, std::ios::out | std::ios::trunc};
    if (!fs.is_open())
    {
        LOG_F(ERROR, "Failed to open file: %s", outputFile.data());
        return false;
    }

    fs << R"(#pragma once
#include <nlohmann/json.hpp>
)";

    for (const auto &inputFile : inputFiles)
    {
        auto inputFileBasename = std::filesystem::path(inputFile).filename();
        fs << R"(#include )" << inputFileBasename << R"(
)";
    }

    fs << R"(
namespace pods_serdes_gen {

)";

    ToJsonGenerator::Generate(structs, fs);
    FromJsonGenerator::Generate(structs, fs);
    ToStringGenerator::Generate(structs, fs);
    VisitorGenerator::Generate(structs, fs);

fs << R"(

} // namespace pods_serdes_gen)";

    return true;
}

} // namespace luarch::pods_serdes::cxx
