# Set this value...
set(VCPKG_ROOT_DIR "")
# OR this value...
set(CMAKE_CONFIG_FILE_DIR "C:/users/chris/cmake/discordcoreapi/share/discordcoreapi")
# AND these values.
set(FFMPEG_DIR "C:/Vcpkg/installed/x64-windows/share/ffmpeg") # Set this one to the folder location of the file "FindFFMPEG.cmake".
set(Opus_DIR "C:/Vcpkg/installed/x64-windows/share/opus") # Set this one to the folder location of the file "OpusConfig.cmake".
set(simdjson_DIR "C:/Vcpkg/installed/x64-windows/share/simdjson") # Set this one to the folder location of the file "simdjson-config.cmake".
set(unofficial-sodium_DIR "C:/Vcpkg/installed/x64-windows/share/unofficial-sodium") # Set this one to the folder location of the file "unofficial-sodiumConfig.cmake".
set(OPENSSL_ROOT_DIR "C:/Vcpkg/installed/x64-windows/") # Set this one to the folder location of the include folder and library folders of OpenSSL.
# Also, set this value to install the output files to a particular location.
set(CMAKE_INSTALL_PREFIX)

cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME "DiscordCoreAPI-Bot")
set(PRODUCT_VERSION "1.0.0.9")
set(DESCRIPTION "A Discord bot, written in C++, using the DiscordCoreAPI Discord bot library.")
set(DiscordCoreAPI_DIR "${CMAKE_CONFIG_FILE_DIR}")
project(
	"${PROJECT_NAME}"
	VERSION "${PRODUCT_VERSION}"
	LANGUAGES CXX
	HOMEPAGE_URL "https://discordcoreapi.com"
	DESCRIPTION "${DESCRIPTION}"
)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${FFMPEG_DIR}") 
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}")

if (WIN32)
	if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		cmake_path(GET CMAKE_ROOT ROOT_PATH ROOT_DRIVE)
		set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/Output Files/" CACHE PATH "The CMake install prefix." FORCE)
	endif()
elseif(UNIX)
	if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/Output Files/" CACHE PATH "The CMake install prefix." FORCE)
	endif()
else()
		message(FATAL_ERROR "Sorry, but your OS is not supported!")
endif()

if(NOT EXISTS "${VCPKG_ROOT_DIR}" AND EXISTS "${CMAKE_CONFIG_FILE_DIR}")
elseif(EXISTS "${VCPKG_ROOT_DIR}")
	include("${VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake")
	string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}" OS_NAME)
	set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT_DIR}/installed/x64-${OS_NAME}")
else()
	message(FATAL_ERROR "Please, set the VCPKG base directory, or the location of the DiscordCoreAPIConfig.cmake file!")
endif()

find_package(DiscordCoreAPI CONFIG REQUIRED)

add_subdirectory(Executable)