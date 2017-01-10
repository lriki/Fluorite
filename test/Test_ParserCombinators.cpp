#include "TestConfig.h"
#include <Fluorite/ParserCombinators.h>

class Test_ParserCombinators : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#if 0
struct ParserCursorCondition_SkipSpace
{
	bool operator()(const Token& token)
	{
		return
			token.EqualChar(';') || token.EqualChar('{') || token.EqualChar('}') ||
			token.EqualString("LN_CLASS", 8) ||
			token.EqualString("LN_METHOD", 9) ||
			token.GetTokenGroup() == TokenGroup::Eof;	// TODO: これが無くてもいいようにしたい。今はこれがないと、Many中にEOFしたときOutOfRangeする
	}
};

using ParserCursor_SkipSpace = combinators::GenericParserCursor<ParserCursorCondition_SkipSpace>;


struct TokenParser : public combinators::ParseLib<ParserCursor_SkipSpace>
{
	struct Decl
	{
		String	type;
		int		begin;
		int		end;
	};

	static ParserResult<Decl> Parse_EmptyDecl(ParserContext input)
	{
		LN_PARSE_RESULT(r1, TokenChar(';'));
		return input.Success(Decl{ _T(""), r1.GetMatchBegin(), r1.GetMatchEnd() });
	}

	static ParserResult<Decl> Parse_LN_METHOD(ParserContext input)
	{
		LN_PARSE_RESULT(r1, TokenString("LN_METHOD"));
		LN_PARSE_RESULT(r2, UntilMore(TokenChar(';')));
		return input.Success(Decl{ _T("LN_METHOD"), r1.GetMatchBegin(), r2.GetMatchEnd() });
	}

	static ParserResult<Decl> Parse_LN_CLASS(ParserContext input)
	{
		LN_PARSE_RESULT(r1, TokenString("LN_CLASS"));
		LN_PARSE_RESULT(r2, TokenChar('{'));
		LN_PARSE_RESULT(r3, Many<Decl>(Parser<Decl>(Parse_EmptyDecl) || Parser<Decl>(Parse_LN_METHOD)));	// ネスト	TODO: できれば <Decl> はやめたい
		LN_PARSE_RESULT(r4, TokenChar('}'));
		return input.Success(Decl{ _T("LN_CLASS"), r1.GetMatchBegin(), r4.GetMatchEnd() });
	}

	static ParserResult<List<Decl>> Parse_File(ParserContext input)
	{
		LN_PARSE(r1, Many(Parser<Decl>(Parse_LN_CLASS)));
		return input.Success(r1);
	}
};

//-----------------------------------------------------------------------------
TEST_F(Test_ParserCombinators, Parse)
{
	DO_LEX("LN_CLASS class A { int a; LN_METHOD int Func(); };");

	auto result = TokenParser::TryParse(
		TokenParser::Parser<List<TokenParser::Decl>>(TokenParser::Parse_File), tokens);

	ASSERT_EQ(true, result.IsSucceed());

}
#endif

//-----------------------------------------------------------------------------
TEST_F(Test_ParserCombinators, AcceptsThatChar)
{
	struct TokenParser : public combinators::ParseLib<>
	{
		// 戻り値が ParserResult<>、引数が ParserContext である関数は Parser 関数。
		// LN_PARSE() はこの中に書くことができる。
		static ParserResult<int> Parse_String(ParserContext input)
		{
			LN_PARSE(r1, TokenChar('a'));
			return input.Success(100);
		}
	};

	{
		DO_LEX("a");
		auto result = TokenParser::TryParse(TokenParser::Parse_String, tokens);
		ASSERT_EQ(true, result.IsSucceed());
		ASSERT_EQ(100, result.GetValue());
		ASSERT_EQ(0, result.GetMatchBegin());
		ASSERT_EQ(1, result.GetMatchEnd());
	}
	{
		DO_LEX("a a");	// ※ "aa" だと 1 つの Token になるので分割されるようにする
		auto result = TokenParser::TryParse(TokenParser::Parse_String, tokens);
		ASSERT_EQ(true, result.IsSucceed());
		ASSERT_EQ(100, result.GetValue());
		ASSERT_EQ(0, result.GetMatchBegin());
		ASSERT_EQ(1, result.GetMatchEnd());		// 1つ目のトークンまでマッチ成功
	}
	{
		DO_LEX("b");
		auto result = TokenParser::TryParse(TokenParser::Parse_String, tokens);
		ASSERT_EQ(false, result.IsSucceed());
		ASSERT_EQ(true, result.GetMessage().IndexOf("Unexpected") >= 0);	// なんかエラーメッセージが入っているはず
	}
}
