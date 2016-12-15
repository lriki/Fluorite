﻿
#include "Internal.h"
#include <Lumino/IO/FileSystem.h>
#include <Fluorite/Diagnostics.h>
#include <Fluorite/AnalyzerContext.h>
#include "Lexer/CppLexer.h"

namespace fl
{

//==============================================================================
// InputFile
//==============================================================================

//------------------------------------------------------------------------------
InputFile::InputFile(const PathNameA& filePath)
	: m_lang(Language::Cpp11)
	, m_category(InputFileCategory::CompileUnit)
	, m_filePath(filePath)
	, m_codeRead(false)
	, m_diag(nullptr)
{
}

//------------------------------------------------------------------------------
InputFile::InputFile(const PathNameA& filePath, const char* code, int length)
	: m_lang(Language::Cpp11)
	, m_filePath(filePath)
	, m_code(code, (length < 0) ? strlen(code) : length)
	, m_codeRead(true)
	, m_diag(nullptr)
{
}

//------------------------------------------------------------------------------
void InputFile::ReadFile()
{
	if (!m_codeRead)
	{
		// TODO: 文字コード変換
		m_code = FileSystem::ReadAllBytes(m_filePath);
		m_codeRead = true;
	}
}

//------------------------------------------------------------------------------
ByteBuffer* InputFile::GetCodeBuffer()
{
	ReadFile();
	return &m_code;
}

//==============================================================================
// AnalyzerContext
//==============================================================================

//------------------------------------------------------------------------------
AnalyzerContext::AnalyzerContext()
{
	m_diagnosticsManager = RefPtr<DiagnosticsManager>::MakeRef();
}

//------------------------------------------------------------------------------
AnalyzerContext::~AnalyzerContext()
{
}

//------------------------------------------------------------------------------
InputFile* AnalyzerContext::RegisterInputFile(const PathNameA& filePath)
{
	auto ptr = RefPtr<InputFile>::MakeRef(filePath);
	m_inputFileList.Add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
InputFile* AnalyzerContext::RegisterInputMemoryCode(const PathNameA& filePath, const char* code, int length)
{
	LN_CHECK_ARG(code != nullptr);
	auto ptr = RefPtr<InputFile>::MakeRef(filePath, code, length);
	m_inputFileList.Add(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
void AnalyzerContext::RemoveAllInputFile()
{
	m_inputFileList.Clear();
}

//------------------------------------------------------------------------------
void AnalyzerContext::LexAll()
{
	for (InputFile* file : m_inputFileList)
	{
		LexFile(file);
	}
}

//------------------------------------------------------------------------------
void AnalyzerContext::LexFile(InputFile* file)
{
	LN_FAIL_CHECK_ARG(file != nullptr) return;
	ResetFileDiagnostics(file);
	auto lexer = CreateLexer(file);
	lexer->Tokenize(file);
}

//------------------------------------------------------------------------------
void AnalyzerContext::PreprocessAll()
{
	for (InputFile* file : m_inputFileList)
	{
		PreprocessFile(file);
	}
}

//------------------------------------------------------------------------------
void AnalyzerContext::PreprocessFile(InputFile* file)
{
	LN_FAIL_CHECK_ARG(file != nullptr) return;
	LN_FAIL_CHECK_ARG(file->GetCategory() != InputFileCategory::CompileUnit) return;

	LN_NOTIMPLEMENTED();
}

//------------------------------------------------------------------------------
void AnalyzerContext::ResetFileDiagnostics(InputFile* file)
{
	if (file->GetDiag() != nullptr)
	{
		file->GetDiag()->ClearItems();
	}
	else
	{
		file->SetDiag(m_diagnosticsManager->CreateItemSet(file->GetRelativeFilePath()));
	}
}

//------------------------------------------------------------------------------
RefPtr<AbstractLexer> AnalyzerContext::CreateLexer(InputFile* file)
{
	switch (file->GetLanguage())
	{
	case Language::Cpp11:
		return RefPtr<AbstractLexer>::StaticCast(RefPtr<CppLexer>::MakeRef());
	default:
		assert(0);
		break;
	}
	return nullptr;
}

} // namespace fl


