#include "USTPFtdcUserApiStruct.h"
#include <pods_serdes_gen/messages.hpp>
#include <iostream>

struct StructVisitor
{
    void StartStruct(const std::string &structName)
    {
        std::cout << "Start of Struct: " << structName << std::endl;
        mStructName = structName;
    }

    void EndStruct() const
    {
        std::cout << "End of Struct" << std::endl;
    }

    template <typename T>
    void Member(const std::string &memberName, const T &member) const
    {
        std::cout << "Member: " << memberName << " in " << mStructName << " value is: " << member << std::endl;
    }

    std::string mStructName;
};

int main(int argc, char *argv[])
{
    CUstpFtdcRspUserLoginField data{};
    std::cout << "Before Change: " << pods_serdes_gen::to_string(data) << "\n" << std::endl;

    char strData[10];
    auto j = pods_serdes_gen::to_json(data);
    j["UserID"] = "123";
    j["FemasLifeCycle"] = 100;
    j["DataCenterID"] = 100;
    pods_serdes_gen::from_json(j, data);
    std::cout << "After Change: " << pods_serdes_gen::to_string(data) << "\n" << std::endl;

    std::cout << "Member visitor:" << std::endl;
    pods_serdes_gen::visit(StructVisitor{}, data);

    return 0;
}
