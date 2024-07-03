#ifndef QUERIER_DEFS_H
#define QUERIER_DEFS_H

#include <functional>
#include <vector>

#include "accumulated_amounts_querier.hpp"
#include "block_info.h"
#include "netspace_data.h"
#include "pledge_info.h"
#include "rank_record.h"
#include "supply_data.h"
#include "timelord_status.h"
#include "vdf_record.h"

using TimelordStatusQuerierType = std::function<TimelordStatus()>;

using LastBlockInfoQuerierType = std::function<BlockInfo()>;

using BlockInfoRangeQuerierType = std::function<std::vector<BlockInfo>(int num_heights, int skip)>;

using BlockInfoSaverType = std::function<void(BlockInfo const& block_info)>;

using NumHeightsByHoursQuerierType = std::function<int(int pass_hours)>;

using VDFPackByChallengeQuerierType = std::function<VDFRecordPack(uint256 const& challenge)>;

using NetspaceQuerierType = std::function<std::vector<NetspaceData>(int num_heights)>;

using RankQuerierType = std::function<std::tuple<std::vector<RankRecord>, int>(int pass_hours)>;

using NetspaceSizeQuerierType = std::function<uint64_t(int pass_hours, int best_height)>;

using SupplyQuerierType = std::function<Supply()>;

using PledgeInfoQuerierType = std::function<PledgeInfo()>;

using RecentlyNetspaceSizeQuerierType = std::function<uint64_t()>;

using VDFProofSubmitterType = std::function<void(uint256 const& challenge, Bytes const& y, Bytes const& proof, int witness_type, uint64_t iters, int duration)>;

using AccumulatedAmountsQuerierType = std::function<AccumulatedAmountMap(int, int)>; // 0-skip full mortgage blocks, 1-the number of blocks will be shown

using AccumulatedCountQuerierType = std::function<int()>;

#endif