
#pragma once
#include <Fluorite/Token.h>

namespace fl {
class InputFile;
class DiagnosticsItemSet;
	
/**
	@brief
*/
class AbstractLexer
	: public Object
{
public:
	struct Range
	{
		const char* pos;
		const char* end;

		Range()
		{
		}

		Range(const char* begin, const char* end_)
		{
			pos = begin;
			end = end_;
		}
	};

public:
	AbstractLexer();
	virtual ~AbstractLexer();

	ResultState Tokenize(InputFile* file);

protected:
	ResultState Tokenize(const ByteBuffer* buffer, TokenList* outTokenList, DiagnosticsItemSet* diag);
	virtual int ReadToken(const Range& buffer, TokenList* list) = 0;
	virtual void PollingToken(const Token& newToken);

	void AddToken(TokenGroup group, const char* bufBegin, const char* bufEnd, int tokenType = 0);
	bool EqualsString(const Token& token, const char* str, int length) const;
	DiagnosticsItemSet* GetDiag() const { return m_diag; }

	static AlphaNumTypeFlags GetAlphaNumType(int ch);

	int ReadNewLine(const Range& buffer);
	static int IsNewLine(const Range& buffer);

	int ReadMBSSequence(const Range& buffer);

	virtual void OnStart();

private:
	InputFile*			m_inputFile;
	const ByteBuffer*	m_inputBuffer;
	TokenList*			m_tokenList;
	DiagnosticsItemSet*	m_diag;
	int					m_currentLineNumber;	// 0�`
	int					m_currentColumn;
};

} // namespace fl


