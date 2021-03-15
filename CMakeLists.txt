cmake_minimum_required(VERSION 3.1.3)

project(test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
set(GEOMETRY_FOLDER ${INCLUDE_FOLDER}/Geometry)
set(TYPES_FOLDER ${INCLUDE_FOLDER}/Types)

set(DIRS 
	${INCLUDE_FOLDER} 
	${TYPES_FOLDER} 
	${GEOMETRY_FOLDER})

add_library(base STATIC
			${INCLUDE_FOLDER}/Selectors.c 
			${INCLUDE_FOLDER}/Object.c
			${INCLUDE_FOLDER}/TypeClass.c
			${INCLUDE_FOLDER}/ReferenceCounter.c
			${INCLUDE_FOLDER}/ReferenceCounter.c
			${INCLUDE_FOLDER}/Exception.c
			${INCLUDE_FOLDER}/ExceptionObject.c)

add_library(geometry STATIC
			${GEOMETRY_FOLDER}/Circle.c
			${GEOMETRY_FOLDER}/Point2D.c)
target_link_libraries(geometry base)

add_library(types STATIC
			${TYPES_FOLDER}/IntType.c)
target_link_libraries(types base)

target_include_directories(base PUBLIC ${DIRS})
target_include_directories(geometry PUBLIC ${DIRS})
target_include_directories(types PUBLIC ${DIRS})

add_executable(main main.c)
target_link_libraries(main
					  base geometry types)
