//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_IP_LITERAL_HPP
#define BOOST_URL_RFC_IP_LITERAL_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error.hpp>
#include <boost/url/string.hpp>

namespace boost {
namespace urls {
namespace rfc {

/** BNF for IP-literal

    @par BNF
    @code
    IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
    @endcode

    @see
        https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2
*/
class ip_literal
{
public:
    struct value_type
    {
    };

    value_type const&
    value() const noexcept
    {
        return v_;
    }

    BOOST_URL_DECL
    char const*
    parse(
        char const* const start,
        char const* const end,
        error_code& ec);

private:
    value_type v_;
};

} // rfc
} // urls
} // boost

#endif