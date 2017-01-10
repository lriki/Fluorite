#include "TestConfig.h"
#include <Lumino/Xml/XmlWriter.h>
#include "../src/Fluorite/Lexer/CppLexer.h"

class Test_CppLexer : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
	}
	virtual void TearDown() {}

	AnalyzerContext	m_context;
};

	

//------------------------------------------------------------------------------
TEST_F(Test_CppLexer, Basic)
{
	// <Test> ���ʎq
	{
		DO_LEX("abc");
		ASSERT_EQ(2, tokens->GetCount());
		ASSERT_EQ(TokenGroup::Identifier, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(1).GetTokenGroup());
	}
	// <Test> �� (�A������󔒕�����1�̃g�[�N���ɂ܂Ƃ߂���)
	{
		DO_LEX(" \t\f\v");
		ASSERT_EQ(2, tokens->GetCount());
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(1).GetTokenGroup());
	}
	// <Test> ���s
	{
		DO_LEX("\n\r\r\n");
		ASSERT_EQ(4, tokens->GetCount());
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(1).GetTokenGroup());
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(2).GetTokenGroup());
	}
	// <Test> ���l���e���� (�T�t�B�b�N�X�Ȃ�)
	{
		DO_LEX("5 0x5 5.0 5.");
		ASSERT_EQ(8, tokens->GetCount());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(0).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Int32, tokens->GetAt(0).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(2).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Int32, tokens->GetAt(2).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(4).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Double, tokens->GetAt(4).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(6).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Double, tokens->GetAt(6).GetTokenType());
	}
	// <Test> �����T�t�B�b�N�X�̊m�F
	{
		DO_LEX("5u 5U    5l 5L    5ul 5Ul 5uL 5UL    5ll 5LL    5ull 5Ull 5uLL 5uLL");
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(0).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt32, tokens->GetAt(0).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(2).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt32, tokens->GetAt(2).GetTokenType());

		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(4).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Int32, tokens->GetAt(4).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(6).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Int32, tokens->GetAt(6).GetTokenType());

		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(8).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt32, tokens->GetAt(8).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(10).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt32, tokens->GetAt(10).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(12).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt32, tokens->GetAt(12).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(14).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt32, tokens->GetAt(14).GetTokenType());

		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(16).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Int64, tokens->GetAt(16).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(18).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Int64, tokens->GetAt(18).GetTokenType());

		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(20).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt64, tokens->GetAt(20).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(22).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt64, tokens->GetAt(22).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(24).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt64, tokens->GetAt(24).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(26).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_UInt64, tokens->GetAt(26).GetTokenType());
	}
	// <Test> �����T�t�B�b�N�X�̊m�F
	{
		DO_LEX("5f 5.F 5d 5.D");
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(0).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Float, tokens->GetAt(0).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(2).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Float, tokens->GetAt(2).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(4).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Double, tokens->GetAt(4).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(6).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Double, tokens->GetAt(6).GetTokenType());
	}
	// <Test> �w���\�L
	{
		DO_LEX("5e+03 5e+03F 5.e+03 5.0e+03");
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(0).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Double, tokens->GetAt(0).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(2).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Float, tokens->GetAt(2).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(4).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Double, tokens->GetAt(4).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(6).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Double, tokens->GetAt(6).GetTokenType());
	}
	// <Test> �������e����
	{
		DO_LEX("'a' '\\'' L'\\n'");
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(0).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Char, tokens->GetAt(0).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(2).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_Char, tokens->GetAt(2).GetTokenType());
		ASSERT_EQ(TokenGroup::ArithmeticLiteral, tokens->GetAt(4).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_WideChar, tokens->GetAt(4).GetTokenType());
	}

	// <Test> �����񃊃e����
	{
		DO_LEX("\"a\" \"\\\"\" L\"\"");
		ASSERT_EQ(TokenGroup::StringLiteral, tokens->GetAt(0).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_AsciiString, tokens->GetAt(0).GetTokenType());
		ASSERT_EQ(TokenGroup::StringLiteral, tokens->GetAt(2).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_AsciiString, tokens->GetAt(2).GetTokenType());
		ASSERT_EQ(TokenGroup::StringLiteral, tokens->GetAt(4).GetTokenGroup()); ASSERT_EQ(TT_NumericLitaralType_WideString, tokens->GetAt(4).GetTokenType());
	}
	// <Test> �u���b�N�R�����g
	{
		DO_LEX("/*a*//**/");
		ASSERT_EQ(TokenGroup::Comment, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Comment, tokens->GetAt(1).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(2).GetTokenGroup());
	}
	// <Test> �s�R�����g
	{
		DO_LEX("//\r\n//a\\\na");
		ASSERT_EQ(TokenGroup::Comment, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(1).GetTokenGroup());	// \r\n
		ASSERT_EQ(TokenGroup::Comment, tokens->GetAt(2).GetTokenGroup());	// \ ���s�̏I�[�ɂ���̂łЂƑ���
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(3).GetTokenGroup());
	}
	// <Test> ���Z�q
	{
		DO_LEX("/ /+++++");
		ASSERT_EQ(TokenGroup::Operator, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(1).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Operator, tokens->GetAt(2).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Operator, tokens->GetAt(3).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Operator, tokens->GetAt(4).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Operator, tokens->GetAt(5).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(6).GetTokenGroup());
	}
	// <Test> �s���G�X�P�[�v
	{
		DO_LEX("\\\n");
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(0).GetTokenGroup()); ASSERT_EQ(TT_EscapeNewLine, tokens->GetAt(0).GetTokenType());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(1).GetTokenGroup());
	
		// �R�����g�A��������̂��̂̓R�����g�A������Ƃ��Ĉ����B���̊O���̂��̂͋󔒂Ƃ��č������B
	}
	// �}���`�o�C�g��������
	{
		byte_t buf[] = { 0xE6, 0x95, 0xB0, 0x20, 0xE6, 0x95, 0xB0, 0x00 };
		DO_LEX((const char*)buf);
		ASSERT_EQ(TokenGroup::MbsSequence, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(1).GetTokenGroup());
		ASSERT_EQ(TokenGroup::MbsSequence, tokens->GetAt(2).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(3).GetTokenGroup());
	}

	// <Test> �v���v���Z�b�T
	{
		DO_LEX("#include <stdio.h>");
		ASSERT_EQ(TokenGroup::Operator, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Identifier, tokens->GetAt(1).GetTokenGroup());
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(2).GetTokenGroup());
		ASSERT_EQ(TokenGroup::StringLiteral, tokens->GetAt(3).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(4).GetTokenGroup());
	}
	// <Test> #pragma
	{
		DO_LEX("#pragma xxx yyy\n");
		ASSERT_EQ(TokenGroup::Operator, tokens->GetAt(0).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Identifier, tokens->GetAt(1).GetTokenGroup());
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(2).GetTokenGroup());
		ASSERT_EQ(TokenGroup::TextTokens, tokens->GetAt(3).GetTokenGroup()); ASSERT_EQ(TT_PPTokens, tokens->GetAt(3).GetTokenType());
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(4).GetTokenGroup());
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(5).GetTokenGroup());
	}
	// <Test> �L�[���[�h
	{
		DO_LEX("if");
		ASSERT_EQ(TokenGroup::Keyword, tokens->GetAt(0).GetTokenGroup());
	}
	// <Test> �擪�������I�ɃL�[���[�h�ł����Ⴂ���Ȃ�����
	{
		DO_LEX("ifdef");
		ASSERT_EQ(TokenGroup::Identifier, tokens->GetAt(0).GetTokenGroup());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_CppLexer, Location)
{
#define CHECK_LOC(tokenIndex, firstLine, firstCol, lastLine, lastCol) \
		ASSERT_EQ(firstLine, tokens->GetAt(tokenIndex).GetFirstLineNumber()); \
		ASSERT_EQ(firstCol,  tokens->GetAt(tokenIndex).GetFirstColumn()); \
		ASSERT_EQ(lastLine,  tokens->GetAt(tokenIndex).GetLastLineNumber()); \
		ASSERT_EQ(lastCol,   tokens->GetAt(tokenIndex).GetLastColumn());

	// <Test> �s�ԍ��A��ԍ�
	{
		DO_LEX("a\n bb\r\\\nd\r\n ");
		ASSERT_EQ(10, tokens->GetCount());
		ASSERT_EQ(TokenGroup::Identifier, tokens->GetAt(0).GetTokenGroup());	CHECK_LOC(0, 1, 1, 1, 1);	// "a"
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(1).GetTokenGroup());		CHECK_LOC(1, 1, 2, 1, 2);	// "\n"
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(2).GetTokenGroup());	CHECK_LOC(2, 2, 1, 2, 1);	// " "
		ASSERT_EQ(TokenGroup::Identifier, tokens->GetAt(3).GetTokenGroup());	CHECK_LOC(3, 2, 2, 2, 3);	// "bb"
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(4).GetTokenGroup());		CHECK_LOC(4, 2, 4, 2, 4);	// "\r"
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(5).GetTokenGroup());	CHECK_LOC(5, 3, 1, 3, 2);	// "\\\n"	EscNewLine
		ASSERT_EQ(TokenGroup::Identifier, tokens->GetAt(6).GetTokenGroup());	CHECK_LOC(6, 4, 1, 4, 1);	// "d"
		ASSERT_EQ(TokenGroup::NewLine, tokens->GetAt(7).GetTokenGroup());		CHECK_LOC(7, 4, 2, 4, 3);	// "\r\n"
		ASSERT_EQ(TokenGroup::SpaceSequence, tokens->GetAt(8).GetTokenGroup());	CHECK_LOC(8, 5, 1, 5, 1);	// " "
		ASSERT_EQ(TokenGroup::Eof, tokens->GetAt(9).GetTokenGroup());
}
	// <Test> �s�ԍ��A��ԍ� (�����s�R�����g)
	{
		DO_LEX("/*a\na*//*\nb\n*/");
		ASSERT_EQ(3, tokens->GetCount());
		ASSERT_EQ(TokenGroup::Comment, tokens->GetAt(0).GetTokenGroup());		CHECK_LOC(0, 1, 1, 2, 3);	// "/*a\na*/"
		ASSERT_EQ(TokenGroup::Comment, tokens->GetAt(1).GetTokenGroup());		CHECK_LOC(1, 2, 4, 4, 2);	// "/*\nb\n*/"
	}
	// <Test> �s�ԍ��A��ԍ� (�R�����g���� EscapeNewLine)
	{
		DO_LEX("/*a\\\na*/");
		ASSERT_EQ(2, tokens->GetCount());
		ASSERT_EQ(TokenGroup::Comment, tokens->GetAt(0).GetTokenGroup());		CHECK_LOC(0, 1, 1, 2, 3);
	}
	// <Test> �s�ԍ��A��ԍ� (�R�����g���� EscapeNewLine)
	{
		DO_LEX("\"a\\\na\"");
		ASSERT_EQ(2, tokens->GetCount());
		ASSERT_EQ(TokenGroup::StringLiteral, tokens->GetAt(0).GetTokenGroup());	CHECK_LOC(0, 1, 1, 2, 2);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_CppLexer, Diagnostics)
{
	// <Test> �u���b�N�R�����g�̓r����EOF
	{
		DO_LEX("/*");
		ASSERT_EQ(DiagnosticsCode::UnexpectedEOFInBlockComment, file->GetDiag()->GetItems()->GetLast().GetCode());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_CppLexer, Example)
{
	// �L�[���[�h�� <b> �ň͂�ł݂�
	AnalyzerContext ctx;
	InputFile* file = ctx.RegisterInputFile(LN_LOCALFILE("Test_CppLexer.cpp"));
	ctx.LexFile(file);
	const TokenList& tokens = *file->GetTokenList();
	tr::XmlFileWriter xml("test.html");
	xml.WriteStartDocument();
	xml.WriteStartElement(_T("body"));
	for (const Token& t : tokens)
	{
		if (t.GetTokenGroup() == TokenGroup::Keyword)
		{
			xml.WriteStartElement(_T("b"));
			StringA str = t.GetString(file);
			xml.WriteString(String::FromNativeCharString(str.c_str(), str.GetLength()));
			xml.WriteEndElement();
		}
		else if(t.GetTokenGroup() == TokenGroup::NewLine)
		{
			xml.WriteStartElement(_T("br"));
			xml.WriteEndElement();
		}
		else
		{
			StringA str = t.GetString(file);
			xml.WriteString(String::FromNativeCharString(str.c_str(), str.GetLength()));
		}
	}
	xml.WriteEndElement();
	xml.WriteEndDocument();
}
