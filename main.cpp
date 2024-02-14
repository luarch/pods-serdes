#include <PodsSerdesConfig.h>
#include <argparse/argparse.hpp>
#include <nlohmann/json.hpp>

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("PodsSerdes", PodsSerdes_VERSION);
    program.add_description(PodsSerdes_DESCRIPTION);
    program.add_argument("input_files")
        .help("Input files to process. Every single struct will be handled generating code")
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

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    auto files = program.get<std::vector<std::string>>("input_files");
    auto parseEnums = program.get<bool>("--parse-enums");

    auto outputCxx = program.present("--output-cxx");
    auto outputPython = program.present("--output-python");

    if (!outputCxx && !outputPython)
    {
        std::cerr << "No output file specified" << std::endl;
        std::cerr << program;
        return 1;
    }

    if (outputCxx)
    {
        std::cout << "Output C++ header to " << outputCxx.value() << std::endl;
        // TODO handle outputCxx.value()
    }

    if (outputPython)
    {
        std::cout << "Output Python header to " << outputPython.value() << std::endl;
        // TODO handle outputPython.value()
    }

    return 0;
}
