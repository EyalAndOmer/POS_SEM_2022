cmake_minimum_required(VERSION 3.18)
project(download_manager)
set(CMAKE_C_STANDARD 99)

add_executable(client ftp.c http.c logger.c https.c)

target_link_libraries(client pthread)

# Add openSSL libraries
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

target_link_libraries(client ${OPENSSL_LIBRARIES})

# Add custom command to clean out execution data
add_custom_target(clean_everything
        COMMAND ${CMAKE_BUILD_TOOL} clean
        COMMAND ${CMAKE_COMMAND} -P clean-all.cmake
        )
