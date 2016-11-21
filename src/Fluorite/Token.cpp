
#include "Internal.h"
#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/Token.h>

namespace fl
{

//==============================================================================
// Token
//==============================================================================

//------------------------------------------------------------------------------
Token::Token()
	: m_locBegin(0)
	, m_locEnd(0)
	, m_firstLineNumber(0)
	, m_firstColumn(0)
	, m_lastLineNumber(0)
	, m_lastColumn(0)
	, m_group(TokenGroup::Unknown)
	, m_tokenType(0)
	, m_valid(false)
{
}

//------------------------------------------------------------------------------
Token::Token(TokenGroup group, SourceLocation locBegin, SourceLocation locEnd)
	: Token()
{
	m_locBegin = locBegin;
	m_locEnd = locEnd;
	m_group = group;
}

//------------------------------------------------------------------------------
Token::Token(TokenGroup group, SourceLocation locBegin, SourceLocation locEnd, int tokenType)
	: Token()
{
	m_locBegin = locBegin;
	m_locEnd = locEnd;
	m_group = group;
	m_tokenType = tokenType;
}

//------------------------------------------------------------------------------
Token::~Token()
{
}

//------------------------------------------------------------------------------
StringA Token::GetString(InputFile* file) const
{
	const char* begin = (const char*)file->GetCodeBuffer()->GetConstData();
	return StringA(begin + m_locBegin, m_locEnd - m_locBegin);
}

} // namespace fl

