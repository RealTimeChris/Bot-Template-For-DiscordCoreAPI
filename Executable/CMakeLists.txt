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

find_package(DiscordCoreAPI CONFIG REQUIRED)

target_link_libraries(
	"${PROJECT_NAME}" PUBLIC
	DiscordCoreAPI::DiscordCoreAPI
)

set_target_properties(
	"${PROJECT_NAME}" PROPERTIES 
	OUTPUT_NAME "${PROJECT_NAME}-Cpp"
	VS_GLOBAL_VcpkgEnabled true
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Output Files/$<CONFIG>"
)

if (WIN32 AND NOT EXISTS "${VCPKG_ROOT}")
	add_custom_command(
		TARGET "${PROJECT_NAME}" PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "$<IF:$<CONFIG:Debug>,${DEBUG_PDB_FILE_PATH},${RELEASE_PDB_FILE_PATH}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	)
	add_custom_command(
		TARGET "${PROJECT_NAME}" PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
		COMMAND_EXPAND_LISTS
	)
endif()
