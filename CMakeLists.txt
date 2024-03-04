cmake_minimum_required(VERSION 3.10)
project(game4)

#set PATH
if(UNIX)
	set(SFML_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../SFML-2.5.1)
elseif(WIN32)
	set(SFML_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../SFML-2.6.0)
	set(SFML_WINDOWS C:/Users/alici/OneDrive/Documents/project4/DLLwindow)
endif()

#SFML2.5 for linux and SFML2.6 for window
if(UNIX)
	link_directories(snake PUBLIC "${SFML_PATH}/lib")
	include_directories(snake PRIVATE "${SFML_PATH}/include")
elseif (WIN32)
	link_directories(snake PUBLIC "${SFML_PATH}/lib")
	include_directories(snake PRIVATE "${SFML_PATH}/include")
endif()

#executable
if(UNIX)
	add_executable(snake src/snake.cpp src/fruit.cpp src/fruits.cpp src/main.cpp)
elseif (WIN32)
	#executable like an application (WIN32) and file with path to the icon(Appicon.rc)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	add_executable(snake WIN32 src/snake.cpp src/fruit.cpp src/fruits.cpp src/main.cpp Appicon.rc)
endif()

#compile option and defintion macro path to ressource
if(UNIX)
	target_compile_options(snake PUBLIC -Wall -g)
	target_compile_definitions(snake PUBLIC MACRO_NAME="/usr/share/ressources")
elseif(WIN32)
	target_compile_definitions(snake PUBLIC MACRO_NAME="./ressources")
endif()

#link libraries
if(UNIX)
    target_link_libraries(snake -lsfml-graphics -lsfml-window -lsfml-system)
elseif (WIN32)
    target_link_libraries(snake sfml-graphics sfml-window sfml-system)
endif()

#generator package
if(UNIX)
	set(CPACK_GENERATOR "DEB")
elseif (WIN32)
	set(CPACK_GENERATOR "NSIS")
endif()

#cpack package
set(CPACK_PACKAGE_NAME "Snake")
set(CPACK_PACKAGE_VERSION "1.0.0")  
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Snake game")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Arry")
set(CPACK_NSIS_DISPLAY_NAME "Snake")
#desktop application windows
set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut \\\"$DESKTOP\\\\snake.lnk\\\" \\\"$INSTDIR\\\\bin\\\\snake.exe\\\" \\\"-foo -bar\\\" "
)
#icon windows
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/ressources/icon.ico")


#sfml library
if(UNIX)
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-graphics2.5")
elseif (WIN32)
	file(GLOB SFML_DLL "${SFML_PATH}/bin/*.dll")
	install(FILES ${SFML_DLL} DESTINATION bin)
endif()

#dll window
if(WIN32)
	file (GLOB WINDOWS_DLL "{SFML_WINDOWS}/*.dll")
	install(FILES ${WINDOWS_DLL} DESTINATION bin)
endif()


install(TARGETS snake RUNTIME DESTINATION bin)

#install ressources + permissons
if (UNIX)
	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ressources" DESTINATION share FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE)
elseif (WIN32)
	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ressources" DESTINATION . )
	set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "AccessControl::GrantOnFile \\\".\\\\ressources\\\" \\\"(BU)\\\" \\\"FullAccess\\\"")
endif()

#desktop linux
if (UNIX)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/snake.desktop.in" "${CMAKE_CURRENT_SOURCE_DIR}/snake.desktop")
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/snake.desktop" DESTINATION share/applications)
endif()

include(CPack)
