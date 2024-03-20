#ifndef SUPPLY_RPC_QUERIER_HPP
#define SUPPLY_RPC_QUERIER_HPP

#include "rpc_client.h"

#include "supply_data.h"

class SupplyRPCQuerier
{
public:
    explicit SupplyRPCQuerier(RPCClient& rpc)
        : rpc_(rpc)
    {
    }

    Supply operator()() const
    {
        auto res = rpc_.Call("querysupply", std::string("0"));
        if (!res.result.isObject()) {
            throw std::runtime_error("the type of result from `querysupply` is not object");
        }

        if (!res.result.exists("calc")) {
            throw std::runtime_error("member `calc` cannot be found");
        }
        auto calcObj = res.result["calc"];

        if (!res.result.exists("last")) {
            throw std::runtime_error("member `last` cannot be found");
        }
        auto lastObj = res.result["last"];

        Supply supply;

        supply.dist_height = res.result["dist_height"].get_int();

        supply.calc.height = calcObj["calc_height"].get_int();
        supply.calc.burned = static_cast<uint64_t>(calcObj["burned"].get_real());
        supply.calc.total = static_cast<uint64_t>(calcObj["total_supplied"].get_real());
        supply.calc.actual = static_cast<uint64_t>(calcObj["actual_supplied"].get_real());

        supply.last.height = lastObj["last_height"].get_int();
        supply.last.burned = static_cast<uint64_t>(lastObj["burned"].get_real());
        supply.last.total = static_cast<uint64_t>(lastObj["total_supplied"].get_real());
        supply.last.actual = static_cast<uint64_t>(lastObj["actual_supplied"].get_real());

        return supply;
    }

private:
    RPCClient& rpc_;
};

#endif
