# This file can be placed into an external project to help locate the minibot TCP API
# It should be include()ed prior to project()

if (DEFINED ENV{MINIBOT_TCP_API_PATH} AND (NOT MINIBOT_TCP_API_PATH))
    set(MINIBOT_TCP_API_PATH $ENV{MINIBOT_TCP_API_PATH})
    message("Using MINIBOT_TCP_API_PATH from environment ('${MINIBOT_TCP_API_PATH}')")
endif ()

if (DEFINED ENV{MINIBOT_TCP_API_FETCH_FROM_GIT} AND (NOT MINIBOT_TCP_API_FETCH_FROM_GIT))
    set(MINIBOT_TCP_API_FETCH_FROM_GIT $ENV{MINIBOT_TCP_API_FETCH_FROM_GIT})
    message("Using MINIBOT_TCP_API_FETCH_FROM_GIT from environment ('${MINIBOT_TCP_API_FETCH_FROM_GIT}')")
endif ()

if (DEFINED ENV{MINIBOT_TCP_API_FETCH_FROM_GIT_PATH} AND (NOT MINIBOT_TCP_API_FETCH_FROM_GIT_PATH))
    set(MINIBOT_TCP_API_FETCH_FROM_GIT_PATH $ENV{MINIBOT_TCP_API_FETCH_FROM_GIT_PATH})
    message("Using MINIBOT_TCP_API_FETCH_FROM_GIT_PATH from environment ('${MINIBOT_TCP_API_FETCH_FROM_GIT_PATH}')")
endif ()

set(MINIBOT_TCP_API_PATH "${MINIBOT_TCP_API_PATH}" CACHE PATH "Path to the Minibot TCP API")
set(MINIBOT_TCP_API_FETCH_FROM_GIT "${MINIBOT_TCP_API_FETCH_FROM_GIT}" CACHE BOOL "Set to ON to download the Minibot TCP API from git if unlocatable")
set(MINIBOT_TCP_API_FETCH_FROM_GIT_PATH "${MINIBOT_TCP_API_FETCH_FROM_GIT_PATH}" CACHE FILEPATH "location to download Minibot TCP API")

if (NOT MINIBOT_TCP_API_PATH)
    if (MINIBOT_TCP_API_FETCH_FROM_GIT)
        include(FetchContent)
        set(FETCHCONTENT_BASE_DIR_SAVE ${FETCHCONTENT_BASE_DIR})
        if (MINIBOT_TCP_API_FETCH_FROM_GIT_PATH)
            get_filename_component(FETCHCONTENT_BASE_DIR "${MINIBOT_TCP_API_FETCH_FROM_GIT_PATH}" REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}")
        endif ()

        FetchContent_Declare(
            minibottcpapi
            GIT_REPOSITORY https://github.com/kyordhel/minibot-tcp-api
            GIT_TAG master
        )

        if (NOT tcpMinibot)
            message("Downloading the Minibot TCP API")
            FetchContent_Populate(minibottcpapi)
            set(MINIBOT_TCP_API_PATH ${minibottcpapi_SOURCE_DIR})
        endif ()
        set(FETCHCONTENT_BASE_DIR ${FETCHCONTENT_BASE_DIR_SAVE})
    else ()
        message(FATAL_ERROR
                "Minibot TCP API location was not specified. Please set MINIBOT_TCP_API_PATH or set MINIBOT_TCP_API_FETCH_FROM_GIT to on to fetch from git."
                )
    endif ()
endif ()

get_filename_component(MINIBOT_TCP_API_PATH "${MINIBOT_TCP_API_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${MINIBOT_TCP_API_PATH})
    message(FATAL_ERROR "Directory '${MINIBOT_TCP_API_PATH}' not found")
endif ()

message("MINIBOT_TCP_API_PATH: ${MINIBOT_TCP_API_PATH}")

set(MINIBOT_TCP_API_IMPORT_CMAKE_FILE ${MINIBOT_TCP_API_PATH}/minibot-tcp-api_import.cmake)
if (NOT EXISTS ${MINIBOT_TCP_API_IMPORT_CMAKE_FILE})
    message(FATAL_ERROR "Directory '${MINIBOT_TCP_API_PATH}' does not appear to contain the Minibot TCP API")
endif ()

set(MINIBOT_TCP_API_PATH ${MINIBOT_TCP_API_PATH} CACHE PATH "Path to the Minibot TCP API" FORCE)

include(${MINIBOT_TCP_API_IMPORT_CMAKE_FILE})