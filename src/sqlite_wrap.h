#ifndef SQLITE_H
#define SQLITE_H

#include <string_view>

#include "sqlite_stmt_wrap.h"

struct sqlite3;

class SQLite
{
public:
    enum class Status { Error, Created, Opened };

    explicit SQLite(std::string_view file_path);

    ~SQLite();

    SQLite(SQLite const& rhs) = delete;

    SQLite& operator=(SQLite const& rhs) = delete;

    SQLite(SQLite&& rhs) noexcept;

    SQLite& operator=(SQLite&& rhs) noexcept;

    Status GetStatus() const;

    void ExecuteSQL(std::string_view sql);

    SQLiteStmt Prepare(std::string_view sql);

    void BeginTransaction();

    void CommitTransaction();

private:
    sqlite3* sql3_ { nullptr };
    Status status_ { Status::Error };
};

#endif
