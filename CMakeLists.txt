cmake_minimum_required (VERSION 3.0.0)
project (Fluorite)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

set(LN_FL_SOURCES
	include/Fluorite/AnalyzerContext.h
	include/Fluorite/Entity.h
	include/Fluorite/EntityDatabase.h
	include/Fluorite/Token.h
	include/Fluorite/Reference.h
	include/Fluorite/SourceFileEntity.h
	include/Fluorite/Utilities.h
	src/Fluorite/AnalyzerContext.cpp
	src/Fluorite/Entity.cpp
	src/Fluorite/EntityDatabase.cpp
	src/Fluorite/Internal.h
	src/Fluorite/Reference.cpp
	src/Fluorite/SourceFileEntity.cpp
	src/Fluorite/Token.cpp
)
source_group("Fluorite"		FILES ${LN_FL_SOURCES})

set(LN_FL_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Core/include
)

#-------------------------------------------------------------------------------
# Configure
#-------------------------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/external/Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
link_libraries(LuminoCore_Static)

#-------------------------------------------------------------------------------
# Outputs
#-------------------------------------------------------------------------------
# Fluorite
add_library(Fluorite STATIC ${LN_FL_SOURCES})
set_target_properties(Fluorite PROPERTIES OUTPUT_NAME "Fluorite${LN_LIB_POSTFIX}")
set_target_properties(Fluorite PROPERTIES INCLUDE_DIRECTORIES "${LN_FL_INCLUDE_DIRECTORIES}")
set_target_properties(Fluorite PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

# test
add_subdirectory(test)


