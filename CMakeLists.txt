cmake_minimum_required(VERSION 3.1.3)

project(test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D'__LOGS__=\"${CMAKE_SOURCE_DIR}/logs\"'")

if (MSVC)
	add_compile_options(/O2 /J /Wall /std:c11 /GS)	
else()
	add_compile_options(-g -O2 -pipe 
						-D_FORTIFY_SOURCE=2 
						-Werror=implicit-function-declaration -Wall
						-funsigned-char -std=c11 -fms-extensions)
endif()

set(INCLUDE_FOLDER include)
set(TYPES_FOLDER ${INCLUDE_FOLDER}/Types)
set(INTTYPES_FOLDER ${TYPES_FOLDER}/Ints)
set(FLOATTYPES_FOLDER ${TYPES_FOLDER}/Floats)
set(INTERFACES_FOLDER ${INCLUDE_FOLDER}/Interfaces)
set(LEAKCHECK_FOLDER ${INCLUDE_FOLDER}/Leakcheck)

set(DIRS 
	${INCLUDE_FOLDER} 
	${TYPES_FOLDER}
	${INTTYPES_FOLDER}
	${FLOATTYPES_FOLDER}
	${LEAKCHECK_FOLDER}
	${INTERFACES_FOLDER}
)

add_library(base STATIC
	${INCLUDE_FOLDER}/Selectors.c 
	${INCLUDE_FOLDER}/Object.c
	${INCLUDE_FOLDER}/Interface.c
	${INCLUDE_FOLDER}/ReferenceCounter.c
	${INCLUDE_FOLDER}/TypeClass.c
	${INCLUDE_FOLDER}/Exception.c
	${INCLUDE_FOLDER}/ExceptionObject.c
	${INCLUDE_FOLDER}/IO.c
	${INCLUDE_FOLDER}/Any.c
	${INCLUDE_FOLDER}/GenericType.c
	${INCLUDE_FOLDER}/Utils.c
	${INCLUDE_FOLDER}/Array.c
	${INCLUDE_FOLDER}/List.c
	${INCLUDE_FOLDER}/String.c
)
target_link_libraries(base types interfaces)

add_library(interfaces STATIC
	${INTERFACES_FOLDER}/IOInterface.c
	${INTERFACES_FOLDER}/ScalarOperatorsInterface.c
	${INTERFACES_FOLDER}/SortInterface.c
	${INTERFACES_FOLDER}/OperatorsInterface.c
	${INTERFACES_FOLDER}/CollectionInterface.c

)
target_link_libraries(interfaces base)

add_library(leakcheck STATIC
	${LEAKCHECK_FOLDER}/stb_leakcheck.c
)

add_library(types STATIC
	${TYPES_FOLDER}/Ints/IntTypes.c
	${TYPES_FOLDER}/Floats/FloatTypes.c
	${TYPES_FOLDER}/Matrix.c
	${TYPES_FOLDER}/Bitset.c
)
target_link_libraries(types base interfaces)

target_include_directories(base PUBLIC ${DIRS})
target_include_directories(types PUBLIC ${DIRS})
target_include_directories(interfaces PUBLIC ${DIRS})

add_executable(main main.c)
target_link_libraries(main
					  base types interfaces)
