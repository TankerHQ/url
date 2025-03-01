#
# Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
# Copyright (c) 2021 DMitry Arkhipov (grisumbras@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/CPPAlliance/url
#

cmake_minimum_required(VERSION 3.8...3.16)

set(BOOST_URL_VERSION 2)
if(BOOST_SUPERPROJECT_VERSION)
    set(BOOST_URL_VERSION ${BOOST_SUPERPROJECT_VERSION})
endif()

project(boost_url VERSION "${BOOST_URL_VERSION}" LANGUAGES CXX)


set(BOOST_URL_IS_ROOT OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(BOOST_URL_IS_ROOT ON)
endif()

if(BOOST_URL_IS_ROOT)
    include(CTest)
endif()
if(NOT BOOST_SUPERPROJECT_VERSION)
    option(BOOST_URL_INSTALL "Install boost::url files" ON)
    option(BOOST_URL_BUILD_TESTS "Build boost::url tests" ${BUILD_TESTING})
else()
    set(BOOST_URL_BUILD_TESTS ${BUILD_TESTING})
endif()


include(GNUInstallDirs)
if(BOOST_URL_IS_ROOT)
    #
    # Building inside Boost tree, but as a separate project e.g. on Travis or
    # other CI, or when producing Visual Studio Solution and Projects. This is
    # when we have to add dependencies to the project manually.
    foreach(lib
        # direct dependencies
        align
        assert
        config
        container
        exception
        mp11
        optional
        system
        throw_exception
        utility

        # indirect dependencies
        container_hash
        core
        detail
        integer
        intrusive
        io
        move
        predef
        preprocessor
        smart_ptr
        static_assert
        tuple
        type_traits
        winapi
    )
        add_subdirectory(../${lib} libs/${lib} EXCLUDE_FROM_ALL)
        set(target boost_${lib})
        if(TARGET ${target})
          get_target_property(lib_type ${target} TYPE)
          if(NOT ${lib_type} STREQUAL INTERFACE_LIBRARY)
            set_property(TARGET ${target} PROPERTY FOLDER _deps/${lib})
          endif()
        endif()
    endforeach()
endif()


function(boost_url_setup_properties target)
    target_compile_features(${target} PUBLIC cxx_constexpr)
    target_compile_definitions(${target} PUBLIC BOOST_URL_NO_LIB=1)

    if(BOOST_SUPERPROJECT_VERSION)
        target_include_directories(${target} PUBLIC "${PROJECT_SOURCE_DIR}/include")
    else()
        target_include_directories(${target}
            PUBLIC
                "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        )
    endif()

    target_link_libraries(${target}
        PUBLIC
            Boost::align
            Boost::assert
            Boost::config
            Boost::exception
            Boost::mp11
            Boost::optional
            Boost::system
            Boost::throw_exception
            Boost::utility
    )
endfunction()


file(GLOB_RECURSE BOOST_URL_HEADERS CONFIGURE_DEPENDS
    include/boost/*.hpp
    include/boost/*.ipp
    include/boost/*.natvis
)

set(BOOST_URL_SOURCES src/src.cpp)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/boost PREFIX "" FILES ${BOOST_URL_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "" FILES ${BOOST_URL_SOURCES})


add_library(boost_url ${BOOST_URL_HEADERS} ${BOOST_URL_SOURCES})
add_library(Boost::url ALIAS boost_url)
boost_url_setup_properties(boost_url)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(boost_url PUBLIC BOOST_URL_DYN_LINK=1)
else()
    target_compile_definitions(boost_url PUBLIC BOOST_URL_STATIC_LINK=1)
endif()


if(BOOST_URL_INSTALL AND NOT BOOST_SUPERPROJECT_VERSION)
    install(TARGETS boost_url
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    )
endif()


if(BOOST_URL_BUILD_TESTS)
    add_subdirectory(test)
endif()
