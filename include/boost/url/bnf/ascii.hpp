//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_BNF_ASCII_HPP
#define BOOST_URL_BNF_ASCII_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/bnf/detail/char_set.hpp>
#include <boost/static_assert.hpp>

namespace boost {
namespace urls {
namespace bnf {

// Algorithms for interacting with ASCII
// characters and strings, for implementing
// semantics in RFCs. These routines do
// not use locale.

/** Return c converted to lower case

    This function returns the lowercase equivalent
    if a character is an upper-case letter, otherwise
    returns the same character.

    @par Exception Safety
    Does not throw.

    @return The converted character

    @param c The ascii character to convert
*/
inline
char
ascii_tolower(char c) noexcept
{
    if(c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
}

} // bnf
} // urls
} // boost

#endif
