
set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	Main.cpp
	TestConfig.h
	Test_CppLexer.cpp
	#Test_Parser_Preprocessor.cpp
	#Test_Parser_RpnEvaluator.cpp
	#Test_Parser_RpnParser.cpp
)

include_directories(
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/test"
	"${PROJECT_SOURCE_DIR}/test/gtest"
	"${PROJECT_SOURCE_DIR}/test/gtest/include"
	"${PROJECT_SOURCE_DIR}/test/gtest/src"
	"${LN_CORE_ROOT_DIR}/include"
)
	
link_libraries(Fluorite)
add_executable(Fluorite_Test ${LN_TEST_SOURCES})
