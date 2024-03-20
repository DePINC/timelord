#ifndef VDF_PERSIST_DATA_H
#define VDF_PERSIST_DATA_H

#include "vdf_record.h"

template <typename Storage> class LocalDatabaseKeeper
{
public:
    explicit LocalDatabaseKeeper(Storage& storage)
        : storage_(storage)
    {
    }

    void Save(VDFRecordPack const& pack)
    {
        storage_.Save(pack);
    }

    void AppendRecord(VDFRecord const& record)
    {
        storage_.AppendRecord(record);
    }

    void AppendRequest(VDFRequest const& request)
    {
        storage_.AppendRequest(request);
    }

private:
    Storage& storage_;
};

#endif
