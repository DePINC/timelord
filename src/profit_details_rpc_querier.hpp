#ifndef PROFIT_DETAILS_RPC_QUERIER_HPP
#define PROFIT_DETAILS_RPC_QUERIER_HPP

#include "rpc_client.h"

#include "profit_details.h"

class ProfitDetailsRPCQuerier
{
    RPCClient* m_rpc;

public:
    explicit ProfitDetailsRPCQuerier(RPCClient* rpc)
        : m_rpc(rpc)
    {
    }

    ProfitDetails operator()(int days, int from_height) const
    {
        auto res = m_rpc->Call("queryprofit", std::to_string(days), std::to_string(from_height));
        if (!res.result.isObject()) {
            throw std::runtime_error("cannot query profit from rpc `queryprofit`");
        }
        auto const& json = res.result;
        ProfitDetails details;
        details.height = json["height"].get_int();
        details.subsidy = json["subsidy"].get_int64();
        details.subsidyHuman = json["subsidyHuman"].get_str();
        details.netspaceTB = json["netspaceTB"].get_str();
        details.profitPerTB = json["profitPerTB"].get_int();
        details.profitPerTBHuman = json["profitPerTBHuman"].get_str();

        if (!json.exists("profitFullmortgage") || !json["profitFullmortgage"].isArray()) {
            throw std::runtime_error("`profitFullmortgage` is not an array");
        }
        auto const& profitFullmortgage_json = json["profitFullmortgage"];
        for (auto const& entry_json : profitFullmortgage_json.getValues()) {
            if (!entry_json.isObject()) {
                throw std::runtime_error("the entry from `profitFullmortgage` array is not an object");
            }
            ProfitMiner miner;
            miner.address = entry_json["address"].get_str();
            miner.netspaceTB = entry_json["netspaceTB"].get_str();
            miner.latestHeight = entry_json["latestHeight"].get_str();
            details.profitFullmortgage.push_back(std::move(miner));
        }
        details.profitFullmortgagePerTB = json["profitFullmortgagePerTB"].get_int64();
        details.profitFullmortgagePerTBHuman = json["profitFullmortgagePerTBHuman"].get_str();
        details.profitFullmortgagePerPB = json["profitFullmortgagePerPB"].get_int64();
        details.profitFullmortgagePerPBHuman = json["profitFullmortgagePerPBHuman"].get_str();
        details.days = json["days"].get_int();

        return details;
    }
};

#endif
