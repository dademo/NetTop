cmake_minimum_required(VERSION 3.1)

project(NetTop)

include (cmake/FindMicroHttpd.cmake)

SET(EXEC_NAME ${PROJECT_NAME})

SET(CMAKE_MODULE_PATH cmake)

# Options

# Debug
OPTION(DEBUG, "To enable debug on the executable. Default: OFF" OFF)

SET(
    CONFIG_MODULES
    router
    tools
    config
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
    in_mods_sources
    src/engine/in_mods/*/*.c
)

FILE(
    GLOB
    in_mods_headers
    src/engine/in_mods/*/*.h
)

# Ajout des modules système
FOREACH(CONFIG_MODULE ${CONFIG_MODULES})
    # Recherche des fichiers pour le module
    FILE(
        GLOB_RECURSE
        ${CONFIG_MODULE}_sources
        src/engine/${CONFIG_MODULE}/*.c
    )

    FILE(
        GLOB_RECURSE
        ${CONFIG_MODULE}_headers
        src/engine/${CONFIG_MODULE}/*.h
    )

    MESSAGE(STATUS "CONFIG_MODULE: ${CONFIG_MODULE}")
    MESSAGE(STATUS "sources: ${${CONFIG_MODULE}_sources}")
    MESSAGE(STATUS "headers: ${${CONFIG_MODULE}_headers}")

    SET(
        config_modules_sources
        ${config_modules_sources}
        ${${CONFIG_MODULE}_sources}
    )

    SET(
        config_modules_headers
        ${config_modules_headers}
        ${${CONFIG_MODULE}_headers}
    )
ENDFOREACH(CONFIG_MODULE)

MESSAGE(STATUS "--")
MESSAGE(STATUS "config_modules_sources: ${config_modules_sources}")
MESSAGE(STATUS "config_modules_headers: ${config_modules_headers}")
MESSAGE(STATUS "--")

SET( ALL_SRC
    ${base_sources}
    ${base_headers}
    ${in_mods_sources}
    ${in_mods_headers}
    ${web_sources}
    ${web_headers}
    ${config_modules_sources}
    ${config_modules_headers}
)


add_executable(${EXEC_NAME} "${ALL_SRC}")

#EXECUTE_PROCESS(COMMAND scripts/web_modules.bash OUTPUT_FILE src/web/config.h)

# Pthread
# https://stackoverflow.com/questions/1620918/cmake-and-libpthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${EXEC_NAME} Threads::Threads)

find_package (GnuTLS REQUIRED)
target_link_libraries(${EXEC_NAME} gnutls)

find_package (Ldap REQUIRED)
target_link_libraries(${EXEC_NAME} ldap)

find_package (LibXml2 REQUIRED)
target_link_libraries(${EXEC_NAME} xml2)


IF(MICROHTTPD_FOUND)
    target_include_directories(${EXEC_NAME} PUBLIC ${MICROHTTPD_INCLUDE_DIR})
    target_link_libraries (${EXEC_NAME} ${MICROHTTPD_LIBRARY})
ELSE()
    MESSAGE(FATAL_ERROR "microhttpd not found !")
ENDIF()


IF(LIBXML2_FOUND)
    INCLUDE_DIRECTORIES(BEFORE ${LIBXML2_INCLUDE_DIR})
    MESSAGE(STATUS "libxml2: ${LIBXML2_INCLUDE_DIR}")
ELSE()
    MESSAGE(FATAL_ERROR "libxml2 not found !")
ENDIF(LIBXML2_FOUND)


IF(DEBUG)
    TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC DEBUG)
    MESSAGE(STATUS "Debug enabled")
ENDIF(DEBUG)
