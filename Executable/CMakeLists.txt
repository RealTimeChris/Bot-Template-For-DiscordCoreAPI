include(GenerateProductVersion)
generate_product_version(
	PRODUCT_INFO
	NAME "${PROJECT_NAME}"
	ICON "${CMAKE_SOURCE_DIR}/TheLogo.ico"
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

if (DEFINED ENV{EnableASAN})
	if(MSVC)
		target_compile_options("${PROJECT_NAME}" PUBLIC "/fsanitize=address" "/Zi" "/Debug")
		target_link_options("${PROJECT_NAME}" PUBLIC "/incremental:no" "/Debug")	
	else()
		target_compile_options("${PROJECT_NAME}" PUBLIC "-fsanitize=address")
		target_link_options("${PROJECT_NAME}" PUBLIC "-fsanitize=address")
	endif()
endif()

find_package(FFMPEG MODULE REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Opus CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)

target_link_libraries(
	"${PROJECT_NAME}" PUBLIC
	${FFMPEG_LIBRARIES}
	DiscordCoreAPI::DiscordCoreAPI
	$<$<TARGET_EXISTS:OpenSSL::SSL>:OpenSSL::SSL>
	$<$<TARGET_EXISTS:OpenSSL::Crypto>:OpenSSL::Crypto>
	$<$<TARGET_EXISTS:Opus::opus>:Opus::opus>
	$<$<TARGET_EXISTS:simdjson::simdjson>:simdjson::simdjson>
	$<$<TARGET_EXISTS:unofficial-sodium::sodium>:unofficial-sodium::sodium>
	$<$<TARGET_EXISTS:Threads::Threads>:Threads::Threads>
)

set_target_properties(
	"${PROJECT_NAME}" PROPERTIES 
	OUTPUT_NAME "${PROJECT_NAME}-Cpp"
	VS_GLOBAL_VcpkgEnabled true
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/$<CONFIG>"
)

if (WIN32 AND NOT EXISTS "${VCPKG_ROOT}")
	install(
		FILES 
		"$<TARGET_PDB_FILE:${PROJECT_NAME}>"
		DESTINATION "$<CONFIG>"
	)
	install(
		FILES 
		"$<IF:$<CONFIG:Debug>,${DEBUG_PDB_FILE_PATH},${RELEASE_PDB_FILE_PATH}>" 
		DESTINATION "$<CONFIG>"
	)	
	install(
		FILES 
		"$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
		DESTINATION "$<CONFIG>"
	)
endif()

install(
	FILES 
	"$<TARGET_FILE:${PROJECT_NAME}>"
	DESTINATION "$<CONFIG>"
)
