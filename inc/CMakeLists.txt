project(sqlite-lib
        LANGUAGES C)
cmake_minimum_required(VERSION 2.8.12)

add_library(sqlite3 STATIC sqlite/sqlite3.c)

include_directories(sqlite)

# foreach (example insert select )
#   target_link_libraries(${example} sqlite3 pthread dl)
# endforeach(example)

add_executable(db-main db-main.c)
target_link_libraries(db-main sqlite3 pthread dl)
