#include <string>

#include <cxxopts.hpp>

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include "local_sqlite_storage.h"
#include "standard_status_querier.h"

#include "block_info_range_rpc_querier.hpp"
#include "block_info_range_sqlite_querier.hpp"

#include "block_info_sqlite_saver.hpp"
#include "last_block_info_querier.hpp"
#include "local_db_netspace_size_querier.hpp"
#include "missing_block_importer.hpp"
#include "netspace_querier.hpp"
#include "num_heights_by_hours_querier.hpp"
#include "pledge_info_rpc_querier.hpp"
#include "recently_netspace_size_rpc_querier.hpp"
#include "rpc_rank_querier.hpp"
#include "supply_rpc_querier.hpp"
#include "vdf_pack_by_challenge_querier.hpp"
#include "vdf_proof_submitter.hpp"

#include "accumulated_amounts_querier.hpp"
#include "accumulated_blocks_count_querier.hpp"

#include "vdf_web_service.h"

#include "timelord.h"

char const* SZ_APP_NAME = "Timelord";

char const* SZ_FORK_HEIGHT_TESTNET = "200000";
char const* SZ_FORK_HEIGHT_MAINNET = "860130";

int main(int argc, char* argv[])
{
    cxxopts::Options opts(SZ_APP_NAME);
    opts.add_options() // All options here
            ("help,h", "Show help document") // --help
            ("logfile", "Store logs into file", cxxopts::value<std::string>()->default_value("./timelord.log")) // --logfile
            ("verbose,v", "Show more logs") // --verbose
            ("mainnet", "Use mainnet fork height") // --mainnet
            ("bind", "Listening to address", cxxopts::value<std::string>()->default_value("127.0.0.1")) // --bind
            ("port", "Listening on this port", cxxopts::value<unsigned short>()->default_value("19191")) // --port
            ("vdf_client-path", "The full path to `vdf_client'", cxxopts::value<std::string>()->default_value("$HOME/vdf_client")) // --vdf_client-path
            ("vdf_client-addr", "vdf_client will listen to this address", cxxopts::value<std::string>()->default_value("127.0.0.1")) // --vdf_client-addr
            ("vdf_client-port", "vdf_client will listen to this port", cxxopts::value<unsigned short>()->default_value("29292")) // --vdf_client-port
            ("db", "store vdf and related information to this file", cxxopts::value<std::string>()->default_value("./timelord.sqlite3")) // --db
            ("web_service-prefix", "The prefix of the api url path", cxxopts::value<std::string>()->default_value("")) // --web_service-path_prefix
            ("web_service-addr", "Web service will listen to this address", cxxopts::value<std::string>()->default_value("127.0.0.1")) // --web_service-addr
            ("web_service-port", "Web service will listen to this port", cxxopts::value<uint16_t>()->default_value("39393")) // --web_service-port
            ("rpc", "The endpoint of btchd core", cxxopts::value<std::string>()->default_value("http://127.0.0.1:18732")) // --rpc
            ("use-cookie", "Use cookie file to login") // --use-cookie
            ("cookie", "Full path to the `.cookie` file generated by btchd core", cxxopts::value<std::string>()->default_value("$HOME/.btchd/testnet3/.cookie")) // --cookie
            ("rpc-user", "The username to identify RPC connection", cxxopts::value<std::string>()) // --rpc-user
            ("rpc-password", "The password to verify RPC connection", cxxopts::value<std::string>()) // --rpc-password
            ("skip-import-check", "The importing procedure will import all blocks from the chain since min-height") // --skip-import-check
            ("skip-host-detection", "Do not detect the host cause sometimes you are under the stupid GTFW") // --skip-host-detection
            ;
    auto parse_result = opts.parse(argc, argv);
    if (parse_result.count("help")) {
        std::cout << SZ_APP_NAME << ", run vdf and returns proofs";
        std::cout << opts.help();
        return 0;
    }

    std::string logfile = ExpandEnvPath(parse_result["logfile"].as<std::string>());
    plog::RollingFileAppender<plog::TxtFormatter> rollingfile_appender(logfile.c_str(), static_cast<int>(1024 * 1024 * 10), 10);

    bool verbose = parse_result.count("verbose") > 0;
    plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
    plog::init((verbose ? plog::Severity::debug : plog::Severity::info), &console_appender).addAppender(&rollingfile_appender);

    PLOGD << "debug mode";

    try {
        std::string timelord_addr = parse_result["bind"].as<std::string>();
        unsigned short timelord_port = parse_result["port"].as<unsigned short>();

        std::string vdf_client_path = ExpandEnvPath(parse_result["vdf_client-path"].as<std::string>());
        std::string vdf_client_addr = parse_result["vdf_client-addr"].as<std::string>();
        unsigned short vdf_client_port = parse_result["vdf_client-port"].as<unsigned short>();
        std::string db_path = parse_result["db"].as<std::string>();
        std::string web_service_prefix = parse_result["web_service-prefix"].as<std::string>();
        std::string web_service_addr = parse_result["web_service-addr"].as<std::string>();
        uint16_t web_service_port = parse_result["web_service-port"].as<uint16_t>();
        std::string url = parse_result["rpc"].as<std::string>();
        std::string cookie_path;
        std::string rpc_user;
        std::string rpc_password;
        bool use_cookie = parse_result.count("use-cookie") > 0;
        if (use_cookie) {
            cookie_path = ExpandEnvPath(parse_result["cookie"].as<std::string>());
        } else {
            rpc_user = parse_result["rpc-user"].as<std::string>();
            rpc_password = parse_result["rpc-password"].as<std::string>();
        }

        bool mainnet = parse_result.count("mainnet") > 0;

        asio::io_context ioc;
        PLOGI << "initializing timelord...";
        PLOGI << "network: " << (mainnet ? "mainnet" : "testnet3");
        PLOGI << "url: " << url;
        PLOGI << "cookie: " << cookie_path;
        PLOGI << "use_cookie: " << (use_cookie ? "yes" : "no");
        PLOGI << "vdf: " << vdf_client_path;

        // prepare local database
        PLOGI << "database: " << db_path;
        LocalSQLiteStorage db(db_path);
        LocalSQLiteDatabaseKeeper persist_operator(db);

        int fork_height = std::atoi(mainnet ? SZ_FORK_HEIGHT_MAINNET : SZ_FORK_HEIGHT_TESTNET);

        // prepare RPC login
        RPCLogin login = use_cookie ? RPCLogin(cookie_path) : RPCLogin(rpc_user, rpc_password);
        RPCClient rpc(true, url, std::move(login));
        Timelord timelord(ioc, rpc, vdf_client_path, vdf_client_addr, vdf_client_port, fork_height, persist_operator, db, VDFProofSubmitter(rpc));

        // before starting services, we need to import the missing blocks
        bool force_from_min_height = parse_result.count("skip-import-check") > 0;
        PLOGD << "Import missing blocks...";
        int num_imported = ImportMissingBlocks(BlockInfoRangeLocalDBQuerier(db), BlockInfoRangeRPCQuerier(rpc), BlockInfoSQLiteSaver(db), fork_height, force_from_min_height);
        PLOGI << tinyformat::format("total %d blocks are imported", num_imported);

        // prepare status querier
        bool skip_host_detection = parse_result.count("skip-host-detection") > 0;
        StandardStatusQuerier status_querier(LastBlockInfoQuerier(rpc), VDFPackByChallengeQuerier(db), LocalDBNetspaceMaxSizeQuerier(db, fork_height), timelord, !skip_host_detection);

        // start web service
        PLOGI << tinyformat::format("web-service is listening on %s:%d", web_service_addr, web_service_port);
        VDFWebService web_service(ioc, web_service_addr, web_service_port, 30, web_service_prefix, fork_height, NumHeightsByHoursQuerier(db, fork_height), BlockInfoRangeLocalDBQuerier(db), NetspaceSQLiteQuerier(db, true), status_querier, RPCRankQuerier(&rpc, fork_height), SupplyRPCQuerier(rpc), PledgeInfoRPCQuerier(rpc), RecentlyNetspaceSizeRPCQuerier(rpc), AccumulatedAmountsQuerier(&rpc), AccumulatedBlocksCountQuerier(&rpc));
        web_service.Run();

        // start timelord
        PLOGI << tinyformat::format("timelord is listening on %s:%d", timelord_addr, timelord_port);
        timelord.Run(timelord_addr, timelord_port);
        ioc.run();
        PLOGD << "exit.";
    } catch (std::exception const& e) {
        PLOGE << e.what();
    }
}