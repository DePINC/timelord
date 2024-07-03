#ifndef ACCUMULATED_BLOCKS_COUNT_QUERIER_HPP
#define ACCUMULATED_BLOCKS_COUNT_QUERIER_HPP

#include "rpc_client.h"

class AccumulatedBlocksCountQuerier
{
public:
    explicit AccumulatedBlocksCountQuerier(RPCClient* prpc)
        : m_prpc(prpc)
    {
    }

    int operator()() const
    {
        auto res = m_prpc->Call("querynumoffullmortgageblocks");
        return std::atoi(res.result.get_str().c_str());
    }

private:
    RPCClient* m_prpc { nullptr };
};

#endif