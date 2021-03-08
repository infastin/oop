cmake_minimum_required(VERSION 3.1.3)

project(test C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (MSVC)
	add_compile_options(/O2 /J /Wall /std:c11 /GS)	
else()
	add_compile_options(-g -O2 -pipe 
						-D_FORTIFY_SOURCE=2 
						-Werror=implicit-function-declaration -Wall
						-funsigned-char -std=c11 -fms-extensions)
endif()

set(INCLUDE_FOLDER include)

add_library(obj STATIC 
			${INCLUDE_FOLDER}/new.c 
			${INCLUDE_FOLDER}/Point2D.c
			${INCLUDE_FOLDER}/Circle.c)
target_include_directories(obj PUBLIC ${INCLUDE_FOLDER})


add_executable(main main.c)
target_link_libraries(main obj)
