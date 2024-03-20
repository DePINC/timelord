#ifndef LOCAL_DB_NETSPACE_SIZE_QUERIER_HPP
#define LOCAL_DB_NETSPACE_SIZE_QUERIER_HPP

#include "local_sqlite_storage.h"

#include "consuming_timer.h"

class LocalDBNetspaceMaxSizeQuerier
{
public:
    LocalDBNetspaceMaxSizeQuerier(LocalSQLiteStorage& db, int fork_height)
        : db_(db)
        , fork_height_(fork_height)
    {
    }

    uint64_t operator()(int pass_hours, int best_height) const
    {
        if (pass_hours == 0) {
            ConsumingTimer t("query_maxnetspace");
            auto res = db_.QueryMaxNetspace(fork_height_, best_height);
            t.PrintLog();
            return res;
        } else {
            ConsumingTimer t("numheights_timerng");
            int num_heights = db_.QueryNumHeightsByTimeRange(pass_hours, fork_height_);
            t.PrintLog();

            ConsumingTimer t2("maxnetspace");
            auto res = db_.QueryMaxNetspace(num_heights, best_height);
            t2.PrintLog();
            return res;
        }
    }

private:
    LocalSQLiteStorage& db_;
    int fork_height_;
};

class LocalDBNetspaceMinSizeQuerier
{
public:
    LocalDBNetspaceMinSizeQuerier(LocalSQLiteStorage& db, int fork_height)
        : db_(db)
        , fork_height_(fork_height)
    {
    }

    uint64_t operator()(int pass_hours, int best_height) const
    {
        if (pass_hours == 0) {
            return db_.QueryMinNetspace(fork_height_, best_height);
        } else {
            int num_heights = db_.QueryNumHeightsByTimeRange(pass_hours, fork_height_);
            return db_.QueryMinNetspace(num_heights, best_height);
        }
    }

private:
    LocalSQLiteStorage& db_;
    int fork_height_;
};

#endif
