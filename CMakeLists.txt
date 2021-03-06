﻿cmake_minimum_required (VERSION 3.0.0)
project (Fluorite)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

set(LN_FL_SOURCES_Common
	include/Fluorite/AnalyzerContext.h
	include/Fluorite/Common.h
	include/Fluorite/Diagnostics.h
	include/Fluorite/Entity.h
	include/Fluorite/EntityDatabase.h
	include/Fluorite/ParserCombinators.h
	include/Fluorite/Token.h
	include/Fluorite/Reference.h
	include/Fluorite/TranslationUnit.h
	include/Fluorite/Utilities.h
	src/Fluorite/AnalyzerContext.cpp
	src/Fluorite/Diagnostics.cpp
#	src/Fluorite/Entity.cpp
#	src/Fluorite/EntityDatabase.cpp
	src/Fluorite/Internal.h
#	src/Fluorite/Lexer.cpp
#	src/Fluorite/Lexer.h
	src/Fluorite/Preprocessor.cpp
	src/Fluorite/Preprocessor.h
#	src/Fluorite/Reference.cpp
#	src/Fluorite/TranslationUnit.cpp
	src/Fluorite/Token.cpp
	src/Fluorite/Rpn/RpnParser.cpp
	src/Fluorite/Rpn/RpnParser.h
)

set(LN_FL_SOURCES_Lexer
	src/Fluorite/Lexer/AbstractLexer.cpp
	src/Fluorite/Lexer/AbstractLexer.h
	src/Fluorite/Lexer/CppLexer.cpp
	src/Fluorite/Lexer/CppLexer.h
)

set(LN_FL_SOURCES
	${LN_FL_SOURCES_Common}
	${LN_FL_SOURCES_Lexer}
)

source_group("Fluorite"		FILES ${LN_FL_SOURCES_Common})
source_group("Lexer"		FILES ${LN_FL_SOURCES_Lexer})

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


