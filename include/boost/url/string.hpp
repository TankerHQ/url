//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAllinace/url
//

#ifndef BOOST_URL_STRING_HPP
#define BOOST_URL_STRING_HPP

#include <boost/url/detail/config.hpp>
#ifndef BOOST_URL_USE_STD_STRING_VIEW
#include <boost/utility/string_view.hpp>
#else
#include <string_view>
#endif

#include <string>

namespace boost {
namespace urls {

/** The type of string_view used by the library
 */
#ifndef BOOST_URL_USE_STD_STRING_VIEW
using string_view = boost::string_view;
#else
using string_view = std::string_view;
#endif

/// The string alias template return type for allocating member functions.
template <class Allocator>
using string_type = std::basic_string<char, std::char_traits<char>, Allocator>;

// cpp17 does not have std::string_view::starts_with
constexpr bool starts_with(string_view v, string_view::value_type c) {
  return !v.empty() && string_view::traits_type::eq(c, v.front());
}

constexpr bool starts_with(string_view v, string_view s) {
  return v.size() >= s.size() &&
         string_view::traits_type::compare(v.data(), s.data(), s.size());
}

} // namespace urls
} // namespace boost

#endif
