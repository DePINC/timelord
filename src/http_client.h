#ifndef CHIAPOS_MINER_HTTP_CLIENT_H
#define CHIAPOS_MINER_HTTP_CLIENT_H

#include <curl/curl.h>

#include <string>
#include <tuple>

#include "types.h"

class HTTPClient {
public:
    HTTPClient(HTTPClient const&) = delete;
    HTTPClient& operator=(HTTPClient const&) = delete;

    HTTPClient(HTTPClient&&) = delete;
    HTTPClient& operator=(HTTPClient&&) = delete;

    HTTPClient(std::string url, std::string user, std::string passwd, bool no_proxy);

    ~HTTPClient();

    std::tuple<int, std::string> Send(std::string const& buff);

    Bytes GetReceivedData() const;

private:
    void AppendRecvData(char const* ptr, size_t total);

    static size_t RecvCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

    static size_t SendCallback(char* buffer, size_t size, size_t nitems, void* userdata);

private:
    CURL* m_curl;
    std::string m_url;
    std::string m_user;
    std::string m_passwd;
    bool m_no_proxy;
    Bytes m_send_data;
    size_t m_send_data_offset{0};
    Bytes m_recv_data;
};

#endif
