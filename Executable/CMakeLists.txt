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
# Set this value...
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(DiscordCoreAPI_DIR "C:/Users/Chris/source/repos/DiscordCoreAPI/Install/windows-debug-dev/share/discordcoreapi/")
else()
	set(DiscordCoreAPI_DIR "C:/Users/Chris/source/repos/DiscordCoreAPI/Install/windows-release-dev/share/discordcoreapi/")
endif()

include("${CMAKE_SOURCE_DIR}/CMake/GenerateProductVersion.cmake")
generate_product_version(
	PRODUCT_INFO
	NAME "${PROJECT_NAME}"
	ICON "${CMAKE_SOURCE_DIR}/THeLogo.ico"
	ORIGINAL_FILENAME "${PROJECT_NAME}-Cpp.exe"
	VERSION_MAJOR "1"
	VERSION_MINOR "0"
	VERSION_PATCH "0"
	VERSION_REVISION "9"
)

add_executable(
	"${PROJECT_NAME}"
	"../main.cpp"
	"${PRODUCT_INFO}"
)

target_compile_options(
	"${PROJECT_NAME}" PUBLIC
	"$<$<CXX_COMPILER_ID:GNU>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:/fsanitize=address>>"
	"$<$<CXX_COMPILER_ID:MSVC>:/Zi>"
	"$<$<CXX_COMPILER_ID:MSVC>:/DEBUG>"
	"$<$<CXX_COMPILER_ID:MSVC>:/MP${THREAD_COUNT}>"
)

target_link_options(
	"${PROJECT_NAME}" PUBLIC
	"$<$<CXX_COMPILER_ID:GNU>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:-fsanitize=address>>"
	"$<$<CXX_COMPILER_ID:MSVC>:/INCREMENTAL:NO>"
	"$<$<CXX_COMPILER_ID:MSVC>:/DEBUG>"
)
 
set_target_properties(
	"${PROJECT_NAME}" PROPERTIES 
	"$<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Release>:COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}>>"
	"$<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Release>:COMPILE_PDB_NAME ${PROJECT_NAME}>>"
)

if (LOCAL)
	find_package(unofficial-sodium CONFIG REQUIRED)
	find_package(DiscordCoreAPI CONFIG REQUIRED)
	find_package(mongocxx CONFIG REQUIRED)
	find_package(Opus CONFIG REQUIRED)
	find_package(OpenSSL REQUIRED)
else()
	include(FetchContent)
		FetchContent_Declare(
		DiscordCoreAPI
		GIT_REPOSITORY https://github.com/RealTimeChris/DiscordCoreAPI
		GIT_TAG "${BRANCH}"
	)
	FetchContent_MakeAvailable(DiscordCoreAPI)

	FetchContent_Declare(
		Jsonifier
		GIT_REPOSITORY https://github.com/RealTimeChris/Jsonifier
		GIT_TAG "${BRANCH}"
	)
	FetchContent_MakeAvailable(Jsonifier)

	FetchContent_Declare(
		unofficial-sodium
		GIT_REPOSITORY https://github.com/jedisct1/libsodium
		GIT_TAG master
	)
	FetchContent_MakeAvailable(unofficial-sodium)

	FetchContent_Declare(
		Opus
		GIT_REPOSITORY https://github.com/xiph/opus
		GIT_TAG master
	)
	FetchContent_MakeAvailable(Opus)

	FetchContent_Declare(
		OpenSSL
		GIT_REPOSITORY https://github.com/openssl/openssl
		GIT_TAG master
	)
	FetchContent_MakeAvailable(OpenSSL)

	FetchContent_Declare(
		mongocxx 
		GIT_REPOSITORY https://github.com/mongodb/mongo-cxx-driver
		GIT_TAG master
	)
	FetchContent_MakeAvailable(mongocxx)
endif()

target_link_libraries(
	"${PROJECT_NAME}" PUBLIC
	$<$<TARGET_EXISTS:Threads::Threads>:Threads::Threads>
	DiscordCoreAPI::DiscordCoreAPI
	unofficial-sodium::sodium
	mongo::mongocxx_shared
	OpenSSL::Crypto
	OpenSSL::SSL
	Opus::opus
)

set_target_properties(
	"${PROJECT_NAME}" PROPERTIES 
	OUTPUT_NAME "${PROJECT_NAME}-Cpp"
)

if (WIN32)
	install(
		FILES 
		"$<TARGET_PDB_FILE:${PROJECT_NAME}>"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)
	install(
		FILES 
		"$<IF:$<CONFIG:Debug>,${DEBUG_PDB_FILE_PATH},${RELEASE_PDB_FILE_PATH}>" 
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)	
	install(
		FILES 
		"$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
		DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)
endif()

install(
	FILES 
	"$<TARGET_FILE:${PROJECT_NAME}>"
	DESTINATION "./"
)