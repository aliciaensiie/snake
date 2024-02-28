cmake_minimum_required(VERSION 3.10)
project(game4)
link_directories(snake PUBLIC "../SFML-2.5.1/lib")
include_directories(snake PRIVATE "../SFML-2.5.1/include")

add_executable(snake src/snake.cpp src/fruit.cpp src/fruits.cpp src/main.cpp)

if(UNIX)
    target_compile_options(snake PUBLIC -Wall -g)
endif()

target_compile_definitions(snake PUBLIC MACRO_NAME="/usr/share/ressources")

if(UNIX)
    target_link_libraries(snake -lsfml-graphics -lsfml-window -lsfml-system)
elseif (WIN32)
    target_link_libraries(snake sfml-graphics sfml-window sfml-system)
endif()

set(CPACK_GENERATOR "DEB;NSIS")
set(CPACK_PACKAGE_NAME "Snake")
set(CPACK_PACKAGE_VERSION "1.0.0")  
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Snake game")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Arry")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-graphics2.5")

install(TARGETS snake RUNTIME DESTINATION bin)
install(DIRECTORY "/home/arry/Documents/projetc++/project4/ressources" DESTINATION share USE_SOURCE_PERMISSIONS)


configure_file("/home/arry/Documents/projetc++/project4/snake.desktop.in" "/home/arry/Documents/projetc++/project4/snake.desktop")
install(FILES "/home/arry/Documents/projetc++/project4/snake.desktop" DESTINATION share/applications)


include(CPack)