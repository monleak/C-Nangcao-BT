project(phonebook)

cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

message(STATUS "Include: ${GTK3_INCLUDE_DIRS}\n LIB_DIR: ${GTK3_LIBRARY_DIRS} \n CFLAGS_OTHERS: ${GTK3_CFLAGS_OTHER} \n LIBS: ${GTK3_LIBRARIES}")

add_executable(phonebook src/phonebook-main.c)
target_link_libraries(phonebook ${GTK3_LIBRARIES})

add_executable(gtk-test src/gtk-test.c)
target_link_libraries(gtk-test ${GTK3_LIBRARIES})

add_executable(sorted-filted src/sorted-filted.c)
target_link_libraries(sorted-filted ${GTK3_LIBRARIES})