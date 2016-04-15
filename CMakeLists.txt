cmake_minimum_required (VERSION 3.0.0)
project (Fluorite)


#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

set(LN_SOURCES_src
	src/Parser/UnitFile.h
	src/Parser/CacheBuffer.h
	src/Parser/Common.h
	src/Parser/Context.cpp
	src/Parser/Context.h
	src/Parser/DiagnosticsManager.cpp
	src/Parser/DiagnosticsManager.h
	src/Parser/EntityDatabase.cpp
	src/Parser/EntityDatabase.h
	src/Parser/GenericParserTypedef.h
	src/Parser/IdentifierMap.h
	src/Parser/Internal.h
	src/Parser/ObjectFactory.cpp
	src/Parser/ObjectFactory.h
	src/Parser/ParserUtils.cpp
	src/Parser/ParserUtils.h
	src/Parser/Token.cpp
	src/Parser/Token.h
	src/Parser/TokenBuffer.cpp
	src/Parser/TokenBuffer.h
	src/Parser/TokenList.h
	src/Parser/UnitFile.cpp
	src/Parser/Frontend/RpnParser.h
	src/Parser/Frontend/Lexer.cpp
	src/Parser/Frontend/Lexer.h
	src/Parser/Frontend/RpnParser.cpp
	src/Parser/Frontend/Cpp/CppLexer.cpp
	src/Parser/Frontend/Cpp/CppLexer.h
	src/Parser/Frontend/Cpp/Preprocessor.cpp
	src/Parser/Frontend/Cpp/Preprocessor.h
)

source_group("Fluorite"		FILES ${LN_SOURCES_src})

set(LN_SOURCES
	${LN_SOURCES_src}
)

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/external/Lumino.Core/include
)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
include(external/Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
add_subdirectory(external/Lumino.Core)
link_libraries(LuminoCore_Static)

#-------------------------------------------------------------------------------
# Outputs
#-------------------------------------------------------------------------------
ln_add_library(${PROJECT_NAME})

# test
add_subdirectory(test)


