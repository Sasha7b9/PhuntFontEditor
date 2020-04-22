cmake_minimum_required(VERSION 3.15)

project (Editor)

include_directories(src
                    ../../../ThirdParty/wxWidgets/include
                    ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DWXUSINGDLL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")


add_compile_options(/wd4355)    # 'this': used in base member initializer list
add_compile_options(/wd4514)    # 'xxx': unreferenced inline function has been removed
add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified


set_source_files_properties(src/_temp/Font.cpp
    PROPERTIES HEADER_FILE_ONLY TRUE)


source_group(src               REGULAR_EXPRESSION src/*)
source_group(_temp             REGULAR_EXPRESSION src/_temp/*)
source_group(Dialogs           REGULAR_EXPRESSION src/Dialogs/*)
source_group(Dialogs\\Controls REGULAR_EXPRESSION src/Disalogs/Controls/*)
source_group(Font              REGULAR_EXPRESSION src/Font/*)

file(GLOB SOURCES src/*
                  src/_temp/*
                  src/Dialogs/*
                  src/Dialogs/Controls/*
                  src/Font/*
                  )

add_executable(Editor ${SOURCES})

target_link_libraries(Editor ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(Editor ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)
target_link_libraries(Editor ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud_xml)

target_link_libraries(Editor WinMM.lib)
target_link_libraries(Editor version.lib)
target_link_libraries(Editor Imm32.Lib)
target_link_libraries(Editor Setupapi.lib)

