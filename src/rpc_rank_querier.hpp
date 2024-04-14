#ifndef RPC_RANK_QUERIER_H
#define RPC_RANK_QUERIER_H

#include "rpc_client.h"

#include "rank_record.h"

class RPCRankQuerier
{
    RPCClient* m_rpc { nullptr };
    int m_fork_height { 0 };

public:
    RPCRankQuerier(RPCClient* rpc, int fork_height)
        : m_rpc(rpc)
        , m_fork_height(fork_height)
    {
    }

    std::tuple<std::vector<RankRecord>, int> operator()(int hours) const
    {
        if (hours == 0) {
            hours = 24 * 7;
        }
        int heights = hours * 20;
        auto res = m_rpc->Call("queryblocksummary", std::to_string(heights));
        if (res.result.isNull() || !res.result.isArray()) {
            throw std::runtime_error("The result from rpc service is invalid");
        }
        std::vector<RankRecord> records;
        for (auto const& entry : res.result.getValues()) {
            RankRecord record;
            record.farmer_pk = entry["address"].get_str();
            record.produced_blocks = entry["count"].get_int();
            record.average_difficulty = entry["average_difficulty"].get_int64();
            record.total_reward = entry["total_reward"].get_int64();
            records.push_back(std::move(record));
        }
        return std::make_tuple(records, m_fork_height);
    }
};

#endif
