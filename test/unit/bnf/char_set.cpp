//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/bnf/char_set.hpp>

#include <boost/url/string.hpp>
#include "test_suite.hpp"
#include "test_bnf.hpp"

namespace boost {
namespace urls {
namespace bnf {

BOOST_STATIC_ASSERT(
    detail::is_char_set_pred<
        all_chars>::value);

BOOST_STATIC_ASSERT(
    detail::is_char_set_pred<
        alnum_chars>::value);

BOOST_STATIC_ASSERT(
    detail::is_char_set_pred<
        alpha_chars>::value);

BOOST_STATIC_ASSERT(
    detail::is_char_set_pred<
        digit_chars>::value);

BOOST_STATIC_ASSERT(
    detail::is_char_set_pred<
        hexdig_chars>::value);

class char_set_test
{
public:
    struct test_chars
    {
        std::size_t& n0;
        std::size_t& n1;

        bool operator()(
            char c) const noexcept
        {
            return c == 'x';
        }

        char const*
        find_if(
            char const* first,
            char const* last) const noexcept
        {
            ++n0;
            while(first != last)
            {
                if(*first == 'x')
                    break;
                ++first;
            }
            return first;
        }

        char const*
        find_if_not(
            char const* first,
            char const* last) const noexcept
        {
            ++n1;
            while(first != last)
            {
                if(*first != 'x')
                    break;
                ++first;
            }
            return first;
        }
    };

    BOOST_STATIC_ASSERT(
        detail::has_find_if<
            test_chars>::value);

    BOOST_STATIC_ASSERT(
        detail::has_find_if_not<
            test_chars>::value);

    void
    run()
    {
        std::size_t n0 = 0;
        std::size_t n1 = 0;
        test_char_set(
            test_chars{n0, n1}, "x");
        BOOST_TEST(n0 > 0);
        BOOST_TEST(n1 > 0);

        {
            all_chars cs;
            for(std::size_t i = 0;
                i < 256; ++i)
                BOOST_TEST(cs(static_cast<
                    char>(i)));
        }

        test_char_set(
            alnum_chars{},
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz");

        test_char_set(
            alpha_chars{},
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz");

        test_char_set(
            digit_chars{},
            "0123456789");

        test_char_set(
            hexdig_chars{},
            "0123456789"
            "ABCDEF"
            "abcdef");

        for_each_char(
        [](char c)
        {
            if(hexdig_chars{}(c))
                BOOST_TEST(
                    hexdig_value(c) != -1);
            else
                BOOST_TEST(
                    hexdig_value(c) == -1);
        });
    }
};

TEST_SUITE(
    char_set_test,
    "boost.url.char_set");

} // bnf
} // urls
} // boost
