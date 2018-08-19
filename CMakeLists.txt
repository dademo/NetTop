cmake_minimum_required(VERSION 3.1)

project(TestC)

include (cmake/FindMicroHttpd.cmake)

SET(EXEC_NAME ${PROJECT_NAME})

FILE(
    GLOB
    base_sources
    src/*.c
    )

FILE(
    GLOB
    base_headers
    src/*.h
    )

FILE(
    GLOB_RECURSE
    web_sources
    src/web/*.c
    )

FILE(
    GLOB_RECURSE
    web_headers
    src/web/*.h
    )



SET( ALL_SRC
    ${base_sources}
    ${base_headers}
    ${web_sources}
    ${web_headers}
    )


add_executable(${EXEC_NAME} "${ALL_SRC}")


# Pthread
# https://stackoverflow.com/questions/1620918/cmake-and-libpthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${EXEC_NAME} Threads::Threads)

find_package (GnuTLS)
target_link_libraries(${EXEC_NAME} gnutls)


IF(MICROHTTPD_FOUND)
    target_include_directories(${EXEC_NAME} PUBLIC ${MICROHTTPD_INCLUDE_DIR})
    target_link_libraries (${EXEC_NAME} ${MICROHTTPD_LIBRARY})
ELSE()
    MESSAGE(FATAL_ERROR "microhttpd not found !")
ENDIF()
