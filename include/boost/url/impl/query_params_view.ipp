//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_QUERY_PARAMS_VIEW_IPP
#define BOOST_URL_IMPL_QUERY_PARAMS_VIEW_IPP

#include <boost/url/query_params_view.hpp>
#include <boost/url/error.hpp>
#include <boost/url/rfc/query_bnf.hpp>
#include <boost/url/rfc/query_params_bnf.hpp>
#include <boost/url/detail/except.hpp>
#include <iostream>

namespace boost {
namespace urls {

query_params_view::
query_params_view() noexcept
    : s_("")
    , n_(0)
{
}

query_params_view::
iterator::
iterator(
    string_view s)
    : next_(s.data())
    , end_(s.data() + s.size())
{
    if(next_ == end_)
    {
        next_ = nullptr;
        return;
    }
    error_code ec;
    query_param t;
    query_params_bnf::begin(
        next_, end_, ec, t);
    BOOST_ASSERT(! ec.failed());
    v_.k_ = t.key;
    v_.has_value_ = 
        t.has_value;
    if(v_.has_value_)
        v_.v_ = t.value;
    else
        v_.v_ = {};
}

auto
query_params_view::
iterator::
operator++() noexcept ->
    iterator&
{
    error_code ec;
    query_param t;
    query_params_bnf::increment(
        next_, end_, ec, t);
    if(ec == error::end)
    {
        next_ = nullptr;
        return *this;
    }
    BOOST_ASSERT(! ec.failed());
    v_.k_ = t.key;
    v_.has_value_ = 
        t.has_value;
    if(v_.has_value_)
        v_.v_ = t.value;
    else
        v_.v_ = {};
    return *this;
}

//------------------------------------------------

auto
query_params_view::
begin() const noexcept ->
    iterator
{
    return iterator(s_);
}

auto
query_params_view::
end() const noexcept ->
    iterator
{
    return iterator(
        s_.data() + s_.size());
}

bool
query_params_view::
contains(
    string_view key) const noexcept
{
    return find(key) != end();
}

std::size_t
query_params_view::
count(
    string_view key) const noexcept
{
    std::size_t n = 0;
    for(auto e : *this)
        if(detail::key_equal_encoded(
                key, e.k_))
            ++n;
    return n;
}

auto
query_params_view::
find(
    string_view key) const noexcept ->
        iterator
{
    auto it = begin();
    for(auto const last = end();
        it != last; ++it)
        if(detail::key_equal_encoded(
                key, it->k_))
            break;
    return it;
}

auto
query_params_view::
find(
    iterator after,
    string_view key) const noexcept ->
        iterator
{
    auto it = after;
    auto const last = end();
    if(it != last)
        while(++it != last)
            if(detail::key_equal_encoded(
                    key, it->k_))
                break;
    return it;
}

std::string
query_params_view::
operator[](string_view key) const
{
    auto const it = find(key);
    if(it == end())
        return {};
    return it->value();
}

//------------------------------------------------

std::ostream&
operator<<(
    std::ostream& os,
    query_params_view const& qpv)
{
    os.write(
        qpv.encoded_query().data(),
        qpv.encoded_query().size());
    return os;
}

//------------------------------------------------

query_params_view
parse_query_params(
    string_view s,
    error_code& ec) noexcept
{
    using bnf::parse_string;
    query_bnf t;
    if(! parse_string(s, ec, t))
        return {};
    return query_params_view(
        t.query, t.query_count);
}

query_params_view
parse_query_params(
    string_view s)
{
    error_code ec;
    auto qp = parse_query_params(s, ec);
    detail::maybe_throw(ec,
        BOOST_CURRENT_LOCATION);
    return qp;
}

} // urls
} // boost

#endif
