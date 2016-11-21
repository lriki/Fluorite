
#pragma once
#include <Lumino/Base/Collection.h>
#include "Common.h"

namespace fl {
class InputFile;
using SourceLocation = uint32_t;

/**
	@brief	
*/
class Token
{
public:
	Token();
	Token(const Token& src) = default;
	Token& operator = (const Token& src) = default;
	~Token();

	Token(TokenGroup group, SourceLocation locBegin, SourceLocation locEnd);
	Token(TokenGroup group, SourceLocation locBegin, SourceLocation locEnd, int tokenType);

	TokenGroup GetTokenGroup() const { return m_group; }

	/** トークンの種別。値の意味はプログラム言語ごとに異なる */
	int GetTokenType() const { return m_tokenType; }

	int GetLength() const { return m_locEnd - m_locBegin; }
	SourceLocation GetBegin() const { return m_locBegin; }
	SourceLocation GetEnd() const { return m_locEnd; }

	int GetFirstLineNumber() const { return m_firstLineNumber; }
	int GetFirstColumn() const { return m_firstColumn; }
	int GetLastLineNumber() const { return m_lastLineNumber; }
	int GetLastColumn() const { return m_lastColumn; }

	StringA GetString(InputFile* file) const;

LN_INTERNAL_ACCESS:
	void SetFirstLineNumber(int lineNumber) { m_firstLineNumber = lineNumber; }
	void SetFirstColumn(int column) { m_firstColumn = column; }
	void SetLastLineNumber(int lineNumber) { m_lastLineNumber = lineNumber; }
	void SetLastColumn(int column) { m_lastColumn = column; }

private:

	// ポインタではなくオフセット値とし、シリアライズに備える
	SourceLocation	m_locBegin;			// トークンの開始位置
	SourceLocation	m_locEnd;			// トークンの終端位置 (最後の文字の次をさす)
	uint32_t		m_firstLineNumber;
	uint32_t		m_firstColumn;
	uint32_t		m_lastLineNumber;
	uint32_t		m_lastColumn;

	TokenGroup		m_group;
	int				m_tokenType;
	bool			m_valid;
};

/**
	@brief	
*/
class TokenList
	: public List<Token>
{
public:
	TokenList() {}
	~TokenList() {}

	//String ToString(int begin, int end) const
	//{
	//	StringBuilder sb;
	//	for (int i = begin; i < end; ++i)
	//	{
	//		sb.Append(GetAt(i).GetBegin(), GetAt(i).GetLength());
	//	}
	//	return sb.ToString();
	//}

	//String ToStringValidCode() const
	//{
	//	StringBuilder sb;
	//	for (int i = 0; i < GetCount(); ++i)
	//	{
	//		if (GetAt(i).IsValid())
	//		{
	//			sb.Append(GetAt(i).GetBegin(), GetAt(i).GetLength());
	//		}
	//	}
	//	return sb.ToString();
	//}
};

} // namespace fl
