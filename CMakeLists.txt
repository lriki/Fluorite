cmake_minimum_required (VERSION 3.0.0)
project (Fluorite)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
set(LN_CORE_INCLUDE_DIR "../Lumino.Core/include" CACHE STRING "")
option(LN_CORE_INCLUDE_DIR "" "")

set(LN_CORE_LIB_PATH "../Lumino.Core/include" CACHE STRING "")
option(LN_CORE_LIB_PATH "" "")

set(LN_FL_LIB_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/lib" CACHE STRING "")
option(LN_FL_LIB_OUTPUT_DIR "" "")

set(LN_FL_LIB_POSTFIX "" CACHE STRING "")
option(LN_FL_LIB_POSTFIX "" "")

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

set(LN_FL_SOURCES
	include/Fluorite/Token.h
	src/Fluorite/Token.cpp
)

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/external/Lumino.Core/include
)

#-------------------------------------------------------------------------------
# Configure
#-------------------------------------------------------------------------------
#include(${LN_CORE_ROOT_DIR}/cmake/LNCommon.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
link_libraries(LN_CORE_LIB_PATH)

#-------------------------------------------------------------------------------
# Outputs
#-------------------------------------------------------------------------------
# Fluorite
add_library(Fluorite STATIC ${LN_FL_SOURCES})
set_target_properties(Fluorite PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_FL_LIB_OUTPUT_DIR}")
set_target_properties(Fluorite PROPERTIES OUTPUT_NAME "Fluorite${LN_FL_LIB_POSTFIX}")

# test
add_subdirectory(test)


