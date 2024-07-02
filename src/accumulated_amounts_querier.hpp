#ifndef ACCUMULATED_AMOUNTS_QUERIER_HPP
#define ACCUMULATED_AMOUNTS_QUERIER_HPP

#include <map>
#include <vector>

#include "rpc_client.h"

struct ContributeBlock {
    int height;
    int64_t amount;
};

struct AccumulatedAmount {
    int height;
    int num_of_distributions;
    int num_of_distributed;
    bool no_more_distribution;
    int64_t subsidy;
    int64_t original_accumulated;
    int64_t actual_accumulated;
    std::string miner;
    int64_t reward;
    int64_t calc_reward;
    std::vector<ContributeBlock> contributed_blocks;
};

using AccumulatedAmountMap = std::map<int, AccumulatedAmount>;

class AccumulatedAmountsQuerier
{
public:
    explicit AccumulatedAmountsQuerier(RPCClient* pclient)
        : m_pclient(pclient)
    {
        assert(m_pclient != nullptr);
    }

    AccumulatedAmountMap operator()(int from_height, int num_blocks) const
    {
        auto res = m_pclient->Call("queryfullmortgageinfo", from_height, num_blocks);
        if (!res.result.isArray()) {
            throw std::runtime_error("the reply value is not an array");
        }
        AccumulatedAmountMap amounts;
        for (auto const& val : res.result.getValues()) {
            AccumulatedAmount entry;
            entry.height = val["height"].get_int();
            entry.num_of_distributions = val["numOfDistributions"].get_int();
            entry.num_of_distributed = val["numOfDistributed"].get_int();
            entry.no_more_distribution = val["noMoreDistribution"].get_bool();
            entry.subsidy = val["subsidy"].get_int64();
            entry.original_accumulated = val["originalAccumulated"].get_int64();
            entry.actual_accumulated = val["actualAccumulated"].get_int64();
            entry.miner = val["miner"].get_str();
            entry.reward = val["reward"].get_int64();
            entry.calc_reward = val["calculatedReward"].get_int64();
            auto const& contributed_from_blocks_val = val["contributedFromBlocks"];
            for (auto const& block_val : contributed_from_blocks_val.getValues()) {
                ContributeBlock block;
                block.height = block_val["height"].get_int();
                block.amount = block_val["amount"].get_int64();
                entry.contributed_blocks.push_back(std::move(block));
            }
            amounts.insert_or_assign(entry.height, entry);
        }
        return amounts;
    }

private:
    RPCClient* m_pclient { nullptr };
};

#endif
