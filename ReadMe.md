# Bot-Template-for-DiscordCoreAPI
A template for a bot, written in C++ using DiscordCoreAPI.
## Build Instructions - With VCPKG
----
1. Make sure you've run `vcpkg integrate install`.
2. Set the `VCPKG_ROOT_DIR` in [CMakeLists.txt](https://github.com/RealTimeChris/Bot-Template-for-DiscordCoreAPI/blob/main/CMakeLists.txt#L2) to the root directory of your VCPKG installation, and make sure you've installed DiscordCoreAPI with it - then start up a project in your IDE, set the C++ standard to 20 or newer, and `#include <discordcoreapi/Index.hpp>`. Add main.cpp to it.
3. Run in a terminal from within the same folder as the top-level [CMakeLists.txt](https://github.com/RealTimeChris/Bot-Template-for-DiscordCoreAPI/blob/main/CMakeLists.txt), `cmake -S . --preset Linux_OR_Windows-Debug_OR_Release`.
4. Then run `cmake --build --preset Linux_OR_Windows-Debug_OR_Release`. The resultant executable will be within a folder called Output Files/Debug_OR_Release, in the top-level folder of the repo.
## Build Instructions - Without VCPKG
1. Build and install the [dependencies](https://github.com/RealTimeChris/DiscordCoreAPI#dependencies).
2. Build and install the [DiscordCoreAPI library](https://github.com/RealTimeChris/DiscordCoreAPI#build-instructions-non-vcpkg---the-library) with CMAKE.
3. Set, in [CMakeLists.txt](https://github.com/RealTimeChris/Bot-Template-for-DiscordCoreAPI/blob/main/CMakeLists.txt#L6-L18), the `<DEPENDENCY>_INCLUDE_DIR`, `<DEPENDENCY>_DEBUG_ROOT` and `<DEPENDENCY>_RELEASE_ROOT` paths to wherever each of the respective dependencies are located - setting the `<DEPENDENCY>_DEBUG_ROOT` folder to wherever the debug version of the .lib file is, the `<DEPENDENCY>_RELEASE_ROOT` folder to wherever the release version of the .lib file for the library is, and the `<DEPENDENCY>_INCLUDE_DIR` to whichever directory contains the public headers for the library.
4. Set, in [CMakeLists.txt](https://github.com/RealTimeChris/Bot-Template-for-DiscordCoreAPI/blob/main/CMakeLists.txt#L4), the `CMAKE_CONFIG_FILE_DIR` to whichever directory contains the DiscordCoreAPIConfig.cmake file that was created by the installation phase of building the library.
5. Run in a terminal from within the same folder as the top-level [CMakeLists.txt](https://github.com/RealTimeChris/Bot-Template-for-DiscordCoreAPI/blob/main/CMakeLists.txt), `cmake -S . --preset Linux_OR_Windows-Debug_OR_Release`.
7. Then run `cmake --build --preset Linux_OR_Windows-Debug_OR_Release`.
8. Run within the same terminal and folder `cmake --install ./Build/Debug_OR_Release`. The default installation paths are: Windows = "ROOT_DRIVE:/Users/USERNAME/CMake/Bot-Template-For-DiscordCoreAPI", Linux = "/home/USERNAME/CMake/Bot-Template-For-DiscordCoreAPI"
