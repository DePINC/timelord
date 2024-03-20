#ifndef HTTP_QUERIER_HPP
#define HTTP_QUERIER_HPP

#include <string_view>

#include "asio_defs.hpp"

std::string HttpsQuery(asio::io_context& ioc, std::string_view host, uint16_t port, std::string_view path, http::verb method = http::verb::get, int version = 11);

#endif
