cmake_minimum_required(VERSION 3.1)

project(NetTop)

include (cmake/FindMicroHttpd.cmake)

SET(EXEC_NAME ${PROJECT_NAME})

# Options

# Debug
OPTION(DEBUG, "To enable debug on the executable. Default: OFF" OFF)

SET(
    MODULES
    router
    tools
)

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
    GLOB
    web_sources
    src/web/*.c
    )

FILE(
    GLOB
    web_headers
    src/web/*.h
    )

FILE(
    GLOB
    web_in_mods_sources
    src/web/in_mods/*.c
)

FILE(
    GLOB
    web_in_mods_headers
    src/web/in_mods/*.h
)

# Ajout des modules système
FOREACH(MODULE ${MODULES})
    # Recherche des fichiers pour le module
    FILE(
        GLOB_RECURSE
        ${MODULE}_sources
        src/engine/*.c
    )

    FILE(
        GLOB_RECURSE
        ${MODULE}_headers
        src/engine/*.h
    )

    SET(
        modules_sources
        ${modules_sources}
        ${${MODULE}_sources}
    )

    SET(
        modules_headers
        ${modules_headers}
        ${${MODULE}_headers}
    )
ENDFOREACH(MODULE)


SET( ALL_SRC
    ${base_sources}
    ${base_headers}
    ${web_sources}
    ${web_headers}
    ${web_in_mods_sources}
    ${web_in_mods_headers}
    ${modules_sources}
    ${modules_headers}
    )


add_executable(${EXEC_NAME} "${ALL_SRC}")

IF(DEBUG)
    TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC DEBUG_ENABLED)
    MESSAGE(STATUS "Debug enabled")
ENDIF(DEBUG)

EXECUTE_PROCESS(COMMAND scripts/web_modules.bash OUTPUT_FILE src/web/config.h)

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
