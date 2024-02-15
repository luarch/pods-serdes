#include <PodsSerdesConfig.h>

#include <argparse/argparse.hpp>
#include <pods-serdes/cxx/gen.hpp>
#include <pods-serdes/python/gen.hpp>
#include <pods-serdes/tuTraversor.hpp>

int main(int argc, char *argv[])
{
    using namespace luarch::pods_serdes;

#ifndef NDEBUG
    loguru::g_stderr_verbosity = loguru::Verbosity_MAX;
#endif

    argparse::ArgumentParser program("PodsSerdes", PodsSerdes_VERSION);
    program.add_description(std::string{PodsSerdes_DESCRIPTION} +
                            ".\nExtra args are parsed to libclang for parsing input files.");
    program.add_argument("-f", "--input_files")
        .help("Input files to process. Every single struct will be handled generating code")
        .required()
        .nargs(argparse::nargs_pattern::at_least_one);
    program.add_argument("-c", "--output-cxx")
        .nargs(1)
        .help("Output file for C++ serialization/deserialization code as C++ header files");
    program.add_argument("-p", "--output-python")
        .nargs(1)
        .help("Output file for Python serialization/deserialization code as Python data classes");
    program.add_argument("-e", "--parse-enums")
        .implicit_value(true)
        .default_value(false)
        .nargs(0)
        .help("Parse enums (incl. raw defines, global variables) as well");

    std::vector<std::string> unknownArgs;
    try
    {
        unknownArgs = program.parse_known_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    for (auto &arg : unknownArgs)
    {
        LOG_F(INFO, "Clang index argument: %s", arg.c_str());
    }

    auto files = program.get<std::vector<std::string>>("--input_files");
    auto parseEnums = program.get<bool>("--parse-enums");

    auto outputCxx = program.present("--output-cxx");
    auto outputPython = program.present("--output-python");

    if (!outputCxx && !outputPython)
    {
        std::cerr << "No output file specified" << std::endl;
        std::cerr << program;
        return 1;
    }

    StructsMap structs;
    for (auto &file : files)
    {
        if (!TranslationUnitTraversor(file, unknownArgs, structs).Traverse())
        {
            return 0;
        }
    }

    if (outputCxx)
    {
        LOG_F(INFO, "Output C++ header source to %s", outputCxx.value().c_str());
        cxx::Generate(structs, files, outputCxx.value());
    }

    if (outputPython)
    {
        LOG_F(INFO, "Output Python source to %s", outputPython.value().c_str());
        python::Generate(structs, outputPython.value());
    }

    return 0;
}
