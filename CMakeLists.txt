cmake_minimum_required(VERSION 3.2)

project(csptr C)

enable_testing()

set(PROJECT_VERSION "0.0.1-0")
set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})

if (NOT  MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g -Wno-unused-result -Wno-missing-field-initializers")
endif()

if (WIN32 AND NOT MSVC)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -W1,-no-undefined")
endif()

option(LIBCSPTR_TESTS "Turn on tests" ON)

find_package(Check)
if (CHECK_FOUND AND LIBCSPTR_TESTS AND NOT LIBCSPTR_DISABLE_TESTS)
	add_subdirectory(check)
endif()

option(FIXED_ALLOCATOR "Define if malloc should always be used" OFF)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/csptr/config.h"
)

set(SOURCE_FILES
	src/mman.c
	src/mman.h
	src/smart_ptr.c
)

set(INTERFACE_FILES
	include/csptr/config.h
	include/csptr/smalloc.h
	include/csptr/smart_ptr.h
	include/csptr/common.h
)

set(MAN_FILES doc/csptr.3)

if (NOT WIN32)
	add_definitions("-fPIC")
endif()

include_directories(include/csptr src)
add_library(csptr STATIC ${SOURCE_FILES} ${INTERFACE_FILES})

install(FILES ${INTERFACE_FILES} DESTINATION include/csptr)
install(FILES ${MAN_FILES} DESTINATION share/man/man3)
install(TARGETS csptr
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

add_custom_target(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)
