cmake_minimum_required(VERSION 3.10)
project(SentinelVault C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

include_directories(${OPENSSL_INCLUDE_DIR} src/)

add_executable(sentinel_vault
    src/main.c src/crypto.c src/obfuscation.c
    src/fragments.c src/trigger.c src/ui_glfw.c
    src/integrity.c
)

target_link_libraries(sentinel_vault ${OPENSSL_LIBRARIES} ${GLFW_LIBRARIES})