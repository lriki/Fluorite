
#include "Internal.h"
#include <Fluorite/AnalyzerContext.h>

namespace fl
{

//==============================================================================
// InputFile
//==============================================================================

//------------------------------------------------------------------------------
InputFile::InputFile(const PathNameA& filePath)
	: m_filePath(filePath)
{
}

//------------------------------------------------------------------------------
InputFile::InputFile(const PathNameA& filePath, const char* code, int length)
	: m_filePath(filePath)
	, m_code(code, length)
{
}

//==============================================================================
// AnalyzerContext
//==============================================================================

//------------------------------------------------------------------------------
AnalyzerContext::AnalyzerContext()
{
}

//------------------------------------------------------------------------------
AnalyzerContext::~AnalyzerContext()
{
}

//------------------------------------------------------------------------------
void AnalyzerContext::RegisterInputFile(const PathNameA& filePath)
{
	m_inputFileList.Add(RefPtr<InputFile>::MakeRef(filePath));
}

//------------------------------------------------------------------------------
void AnalyzerContext::RegisterInputMemoryCode(const PathNameA& filePath, const char* code, int length)
{
	m_inputFileList.Add(RefPtr<InputFile>::MakeRef(filePath, code, length));
}

} // namespace fl


