#pragma once

#include <gtest/gtest.h>
#include <memory>
#include <LuminoCore.h>
#include <Lumino/Testing/TestUtils.h>
//#include "../src/Parser/Frontend/Cpp/CppLexer.h"
//#include "../src/Parser/DiagnosticsManager.h"
//#include "../src/Parser/Frontend/RPNParser.h"
//#include "../src/Parser/UnitFile.h"
using namespace ln;

#include <Fluorite/Common.h>
#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/Diagnostics.h>
using namespace fl;

#define LOCALFILE(fileName) TestUtils::GetFilePath(__FILE__, fileName).c_str()
#define LOCALFILEA(fileName) TestUtils::GetFilePathA(__FILE__, fileName).c_str()
#define LOCALFILEW(fileName) TestUtils::GetFilePathW(__FILE__, fileName).c_str()
