#include <nlohmann/json.hpp>
#include "USTPFtdcUserApiStruct.h"

namespace pods_serdes_gen {

inline nlohmann::json to_json(const ::CUstpFtdcRspUserLoginField &data)
{
    nlohmann::json j {
        {"TradingDay", data.TradingDay},
        {"BrokerID", data.BrokerID},
        // ...continue with all members
    };

    return j;
}

inline void from_json(const nlohmann::json &j, CUstpFtdcRspUserLoginField &data)
{
    j.at("TradingDay").get_to(data.TradingDay);
    j.at("FemasLifeCycle").get_to(data.FemasLifeCycle);
    // ...continue with all members
}

inline std::string to_string(const CUstpFtdcRspUserLoginField &data)
{
    std::ostringstream oss;
    oss << "CUstpFtdcRspUserLoginField: {";

    oss << "TradingDay: \"" << std::string{data.TradingDay, 9} << "\", ";
    oss << "FemasLifeCycle: " << data.FemasLifeCycle << ", ";

    {
        char lastCharItem{0};
        std::ios formatSavior(nullptr);
        formatSavior.copyfmt(oss);
        oss << "LastCharItem: "
            << "0x" << std::setfill('0') << std::setw(2) << std::hex << lastCharItem;
        oss.copyfmt(formatSavior);
        oss << " ("
            << std::string{lastCharItem, 1} << ")" << ", ";
    }

    oss << "}";
    return {};
}

template<typename Visitor>
inline void visit(Visitor &&visitor, const CUstpFtdcRspUserLoginField &data)
{
    visitor.StartStruct("CUstpFtdcRspUserLoginField");
    visitor.Member("TradingDay", data.TradingDay);
    visitor.Member("FemasLifeCycle", data.FemasLifeCycle);
    //...
    visitor.EndStruct();
}

} // namespace pods_serdes_gen
