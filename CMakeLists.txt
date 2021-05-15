project(phonebook)

cmake_minimum_required(VERSION 2.6)

# ============== GTK ==============
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

message(STATUS "Include: ${GTK3_INCLUDE_DIRS}\n LIB_DIR: ${GTK3_LIBRARY_DIRS} \n CFLAGS_OTHERS: ${GTK3_CFLAGS_OTHER} \n LIBS: ${GTK3_LIBRARIES}")
# ============== GTK ==============

# add_executable(phonebook src/phonebook-main.c  inc/db-main.c)
# target_link_libraries(phonebook ${GTK3_LIBRARIES} sqlite3 pthread dl)

# ================= SQL =============
add_library(sqlite3 STATIC inc/sqlite/sqlite3.c)
include_directories(inc/sqlite)
# ================= SQL =============

add_library(utility src/utility.c)
add_library(db inc/db-main.c)

add_executable(phonebook-list src/phonebook-list.c)
target_link_libraries(phonebook-list ${GTK3_LIBRARIES} utility db sqlite3 pthread dl)



