//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_SCHEME_HPP
#define BOOST_URL_SCHEME_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/string.hpp>

namespace boost {
namespace urls {

/** Identifies a known URL scheme
*/
enum class scheme : unsigned char
{
    /** Indicates that no scheme is present
    */
    none = 0,

    /** Indicates the scheme is not a well-known scheme
    */
    unknown,

    /**
     * File Transfer Protocol (FTP)

       FTP is a standard communication protocol
       used for the transfer of computer files
       from a server to a client on a computer
       network.

       @par Specification
       @li <a href="https://datatracker.ietf.org/doc/html/draft-yevstifeyev-ftp-uri-scheme">
           The 'ftp' URI Scheme</a>
    */
    ftp,

    /** File URI Scheme

        The File URI Scheme is typically used
        to retrieve files from within one's
        own computer.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc8089">
            The "file" URI Scheme (rfc8089)</a>
    */
    file,

    /** The Hypertext Transfer Protocol URI Scheme

        URLs of this type indicate a resource which
        is interacted with using the HTTP protocol.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc7230">
            Hypertext Transfer Protocol (HTTP/1.1) (rfc7230)</a>
    */
    http,

    /** The Secure Hypertext Transfer Protocol URI Scheme

        URLs of this type indicate a resource which
        is interacted with using the Secure HTTP
        protocol.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc7230">
            Hypertext Transfer Protocol (HTTP/1.1) (rfc7230)</a>
    */
    https,

    /** The WebSocket URI Scheme

        URLs of this type indicate a resource which
        is interacted with using the WebSocket protocol.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc6455">
            The WebSocket Protocol (rfc6455)</a>
    */
    ws,

    /** The Secure WebSocket URI Scheme

        URLs of this type indicate a resource which
        is interacted with using the Secure WebSocket
        protocol.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc6455">
            The WebSocket Protocol (rfc6455)</a>
    */
    wss
};

/** Return the known scheme for a non-normalized string, if known

    If the string does not identify a known
    scheme, the value @ref scheme::unknown is
    returned.

    @return The known scheme

    @param s The string holding the scheme

*/
BOOST_URL_DECL
scheme
string_to_scheme(string_view s) noexcept;

/** Return the normalized string for a known scheme

    @return A string representing the known scheme

    @param s The known scheme constant
*/
BOOST_URL_DECL
string_view
to_string(scheme s) noexcept;

} // urls
} // boost

#endif
