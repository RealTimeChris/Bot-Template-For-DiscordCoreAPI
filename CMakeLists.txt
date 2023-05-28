set(LOCAL TRUE)

set(VCPKG_ROOT_DIR "C:/vcpkg")

if (ASAN_ENABLED)
	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(DiscordCoreAPI_DIR "C:/Users/Chris/source/repos/DiscordCoreAPI/Install/Windows-Debug-Asan/share/discordcoreapi/")
	else()
		set(DiscordCoreAPI_DIR "C:/Users/Chris/source/repos/DiscordCoreAPI/Install/Windows-Release-Asan/share/discordcoreapi/")
	endif()
else()
	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(DiscordCoreAPI_DIR "C:/Users/Chris/source/repos/DiscordCoreAPI/Install/Windows-Debug-Dev/share/discordcoreapi/")
	else()
		set(DiscordCoreAPI_DIR "C:/Users/Chris/source/repos/DiscordCoreAPI/Install/Windows-Release-Dev/share/discordcoreapi/")
	endif()
endif()

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
	set(OPENSSL_ROOT_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/")
	set(Jsonifier_DIR "${VCPKG_INSTALLATION_ROOT_NEW}/installed/x64-${OS}/share/jsonifier")
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