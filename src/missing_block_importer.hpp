#ifndef MISSING_BLOCK_IMPORTER_HPP
#define MISSING_BLOCK_IMPORTER_HPP

#include <plog/Log.h>

#include <tinyformat.h>

#include "timelord_utils.h"

/**
 * @brief Find and import missing blocks from RPC server to local database
 *
 * @tparam LocalDBQuerier local database querier type
 * @tparam RPCQuerier RPC querier type
 * @tparam Saver local database saver type
 * @param local_querier local database querier
 * @param rpc_querier RPC querier
 * @param saver use this functor to save block to local database
 * @param min_height import blocks only when the height >= min_height
 * @param force_from_min_height ignore the existing blocks from local db
 *
 * @return the number of blocks are saved
 */
template <typename LocalDBQuerier, typename RPCQuerier, typename Saver> int ImportMissingBlocks(LocalDBQuerier local_querier, RPCQuerier rpc_querier, Saver saver, int min_height, bool force_from_min_height, int batch_max = 1000)
{
    PLOGD << "querying blocks...";
    auto blocks = rpc_querier(1, 0);
    if (blocks.empty()) {
        // something is wrong of the RPC service, cannot proceed
        PLOGE << "RPC service returns no block";
        return 0;
    }
    int max_height = blocks.front().height;
    int start_height = min_height;
    if (!force_from_min_height) {
        blocks = local_querier(1, 0);
        if (!blocks.empty()) {
            start_height = blocks.front().height;
        }
    }

    int num_heights = max_height - start_height;

    PLOGD << tinyformat::format("%s: max_height=%d, start_height=%d, num_heights=%d", __func__, max_height, start_height, num_heights);

    if (num_heights == 0) {
        return 0;
    }
    PLOGD << tinyformat::format("querying total %d blocks from RPC service", num_heights);

    int num_saved { 0 };
    int skip_num { 0 };
    while (num_heights > 0) {
        int query_heights = std::min(num_heights, batch_max);
        PLOGD << tinyformat::format("%s: querying from %d skip %d, %d left", __func__, query_heights, skip_num, num_heights);
        blocks = rpc_querier(query_heights, skip_num);
        num_heights -= query_heights;
        skip_num += query_heights;

        for (auto const& block : blocks) {
            try {
                saver(block);
                ++num_saved;
                if (num_saved % 131 == 0) {
                    PLOGD << tinyformat::format("%s: saved total %d blocks...", __func__, num_saved);
                }
            } catch (std::exception const& e) {
                PLOGE << tinyformat::format("cannot save block (hash=%s, height=%s) into local database, err: %s", Uint256ToHex(block.hash), block.height, e.what());
            }
        }
    }

    PLOGD << tinyformat::format("%s: saved total %d blocks.", __func__, num_saved);
    return num_saved;
}

#endif
