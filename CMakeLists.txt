#
#	DiscordCoreAPI, A bot library for Discord, written in C++, and featuring explicit multithreading through the usage of custom, asynchronous C++ CoRoutines.
#
#	Copyright 2021, 2022, 2023 Chris M. (RealTimeChris)
#
#	This library is free software; you can redistribute it and/or
#	modify it under the terms of the GNU Lesser General Public
#	License as published by the Free Software Foundation; either
#	version 2.1 of the License, or (at your option) any later version.
#
#	This library is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#	Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public
#	License along with this library; if not, write to the Free Software
#	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
#	USA
#
# CMakeLists.txt - The CMake script for building this library.
# May 13, 2021
# https://discordcoreapi.com

set(VCPKG_ROOT_DIR "C:/vcpkg")
#set(Jsonifier_DIR "C:/Users/Chris/source/repos/Jsonifier/Install/Windows-Release/share/jsonifier")

if (WIN32)
	set(OS "windows")
elseif(UNIX)
	set(OS "linux")
elseif(APPLE)
	set(OS "osx")
endif()
if (EXISTS "$ENV{VCPKG_INSTALLATION_ROOT}")
	set(VCPKG_INSTALLATION_ROOT_NEW "$ENV{VCPKG_INSTALLATION_ROOT}")
elseif(EXISTS "${VCPKG_ROOT_DIR}")
	set(VCPKG_INSTALLATION_ROOT_NEW "${VCPKG_ROOT_DIR}")
endif()

if (EXISTS "${VCPKG_INSTALLATION_ROOT_NEW}")
	set(bson-1.0_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/bson-1.0")
	set(mongoc-1.0_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/mongoc-1.0")
	set(mongocxx_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/mongocxx")
	set(bsoncxx_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/bsoncxx")
	set(ZLIB_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/zlib")
	set(OPENSSL_ROOT_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/")
	#set(Jsonifier_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/jsonifier")
	set(Opus_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/opus")
	set(unofficial-sodium_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/unofficial-sodium")
	set(DiscordCoreAPI_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/discordcoreapi")
	include("${VCPKG_INSTALLATION_ROOT_NEW}/scripts/buildsystems/vcpkg.cmake")
else()
	message(WARNING "No Vcpkg root folder found, Please make sure that you properly set the library folders.")
endif()

cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME "Bot-Template-For-DiscordCoreAPI")
set(PRODUCT_VERSION "1.0.0.9")
set(DESCRIPTION "A Discord bot, written in C++, using the DiscordCoreAPI Discord bot library.")

project(
	"${PROJECT_NAME}"
	VERSION "${PRODUCT_VERSION}"
	LANGUAGES CXX
	HOMEPAGE_URL "https://discordcoreapi.com"
	DESCRIPTION "${DESCRIPTION}"
)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/CMake/") 
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}")

add_subdirectory("Executable")