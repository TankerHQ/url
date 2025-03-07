//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAllinace/url
//

#ifndef BOOST_URL_IMPL_IPV6_ADDRESS_IPP
#define BOOST_URL_IMPL_IPV6_ADDRESS_IPP

#include <boost/url/ipv6_address.hpp>
#include <boost/url/ipv4_address.hpp>
#include <boost/url/detail/except.hpp>
#include <boost/url/bnf/parse.hpp>
#include <boost/url/rfc/ipv6_address_bnf.hpp>
#include <cstring>

namespace boost {
namespace urls {

ipv6_address::
ipv6_address() noexcept
    : addr_()
{
}

ipv6_address::
ipv6_address(
    bytes_type const& bytes) noexcept
{
    std::memcpy(&addr_,
        bytes.data(), 16);
}

ipv6_address::
ipv6_address(
    ipv6_address const& other) noexcept
    : addr_(other.addr_)
{
}

ipv6_address&
ipv6_address::
operator=(
    ipv6_address const& other) noexcept
{
    addr_ = other.addr_;
    return *this;
}


auto
ipv6_address::
to_bytes() const noexcept ->
    bytes_type
{
    return addr_;
}

string_view
ipv6_address::
to_buffer(
    char* dest,
    std::size_t dest_size) const
{
    if(dest_size < max_str_len)
        detail::throw_length_error(
            "ipv6_address::to_buffer",
            BOOST_CURRENT_LOCATION);
    auto n = print_impl(dest);
    return string_view(dest, n);
}

bool
ipv6_address::
is_loopback() const noexcept
{
    return *this == loopback();
}

bool
ipv6_address::
is_unspecified() const noexcept
{
    return *this == ipv6_address();
}

bool
ipv6_address::
is_v4_mapped() const noexcept
{
    return
        addr_[ 0] == 0 && addr_[ 1] == 0 &&
        addr_[ 2] == 0 && addr_[ 3] == 0 &&
        addr_[ 4] == 0 && addr_[ 5] == 0 &&
        addr_[ 6] == 0 && addr_[ 7] == 0 &&
        addr_[ 8] == 0 && addr_[ 9] == 0 &&
        addr_[10] == 0xff &&
        addr_[11] == 0xff;
}

bool
operator==(
    ipv6_address const& a1,
    ipv6_address const& a2) noexcept
{
    return a1.addr_ == a2.addr_;
}

ipv6_address
ipv6_address::
loopback() noexcept
{
    ipv6_address a;
    a.addr_[15] = 1;
    return a;
}

std::size_t
ipv6_address::
print_impl(
    char* dest) const noexcept
{
    auto const count_zeroes =
    []( unsigned char const* first,
        unsigned char const* const last)
    {
        std::size_t n = 0;
        while(first != last)
        {
            if( first[0] != 0 ||
                first[1] != 0)
                break;
            n += 2;
            first += 2;
        }
        return n;
    };
    auto const print_hex =
    []( char* dest,
        unsigned short v)
    {
        char const* const dig =
            "0123456789abcdef";
        if(v >= 0x1000)
        {
            *dest++ = dig[v>>12];
            v &= 0x0fff;
        }
        if(v >= 0x100)
        {
            *dest++ = dig[v>>8];
            v &= 0x0ff;
        }
        if(v >= 0x10)
        {
            *dest++ = dig[v>>4];
            v &= 0x0f;
        }
        *dest++ = dig[v];
        return dest;
    };
    auto const dest0 = dest;
    // find longest run of zeroes
    std::size_t best_len = 0;
    int best_pos = -1;
    auto it = addr_.data();
    auto const v4 =
        is_v4_mapped();
    auto const end = v4 ?
        (it + addr_.size() - 4)
        : it + addr_.size();
    while(it != end)
    {
        auto n = count_zeroes(
            it, end);
        if(n == 0)
        {
            it += 2;
            continue;
        }
        if(n > best_len)
        {
            best_pos = static_cast<
                int>(it - addr_.data());
            best_len = n;
        }
        it += n;
    }
    it = addr_.data();
    if(best_pos != 0)
    {
        unsigned short v =
            (it[0] * 256U) + it[1];
        dest = print_hex(dest, v);
        it += 2;
    }
    else
    {
        *dest++ = ':';
        it += best_len;
        if(it == end)
            *dest++ = ':';
    }
    while(it != end)
    {
        *dest++ = ':';
        if(it - addr_.data() ==
            best_pos)
        {
            it += best_len;
            if(it == end)
                *dest++ = ':';
            continue;
        }
        unsigned short v =
            (it[0] * 256U) + it[1];
        dest = print_hex(dest, v);
        it += 2;
    }
    if(v4)
    {
        ipv4_address::bytes_type bytes;
        bytes[0] = it[0];
        bytes[1] = it[1];
        bytes[2] = it[2];
        bytes[3] = it[3];
        ipv4_address a(bytes);
        *dest++ = ':';
        dest += a.print_impl(dest);
    }
    return dest - dest0;
}

std::ostream&
operator<<(
    std::ostream& os,
    ipv6_address const& addr)
{
    char buf[ipv6_address::max_str_len];
    auto const s = addr.to_buffer(
        buf, sizeof(buf));
    os.write(s.data(), s.size());
    return os;
}

BOOST_URL_DECL
ipv6_address
make_ipv6_address(
    string_view s,
    error_code& ec) noexcept
{
    ipv6_address a;
    using bnf::parse_string;
    if(! parse_string(s, ec,
            ipv6_address_bnf{a}))
        return {};
    return a;
}

/** Return an IPv6 address from an IP address string
*/
BOOST_URL_DECL
ipv6_address
make_ipv6_address(
    string_view s)
{
    error_code ec;
    auto a = make_ipv6_address(
        s, ec);
    detail::maybe_throw(ec,
        BOOST_CURRENT_LOCATION);
    return a;
}

ipv6_address
make_ipv6_address(
    ipv4_address const& a) noexcept
{
    auto const v = a.to_bytes();
    ipv6_address::bytes_type bytes = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0xff, 0xff, v[0], v[1], v[2], v[3] } };
    return ipv6_address(bytes);
}

} // urls
} // boost

#endif
