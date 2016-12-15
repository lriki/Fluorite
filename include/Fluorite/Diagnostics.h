﻿
#pragma once
#include "Common.h"

namespace fl {

enum class DiagnosticsCode
{
	Severity_Note		= 0x00000000,
	Severity_Warning	= 0x10000000,
	Severity_Error		= 0x20000000,
	Severity_Fatal		= 0x30000000,

	CppLexerGroup = 0x00010000,
	//CppLexer_InvalidToken = Severity_Error | CppLexerGroup | 0x0001,
	UnexpectedEOFInBlockComment = Severity_Error | CppLexerGroup | 0x0001,

	// RPN 解析・評価
	RpnEvaluatorGroup = 0x00020000,
	RpnEvaluator_InvalidNumericLiteral		= Severity_Error | RpnEvaluatorGroup | 1,
	RpnEvaluator_UnexpectedToken			= Severity_Error | RpnEvaluatorGroup | 2,	/**< 予期しないトークンが見つかりました。*/
	RpnEvaluator_InsufficientToken			= Severity_Error | RpnEvaluatorGroup | 3,	/**< 式を評価するためのトークンが不足しています。(式が完結していない) */
	RpnEvaluator_InvalidFuncCallArgsCount	= Severity_Error | RpnEvaluatorGroup | 4,	/**< 関数呼び出しの引数が足りません。*/
	RpnEvaluator_InvalidOperatorSide		= Severity_Error | RpnEvaluatorGroup | 5,	/**< 演算子の辺に対するオペランドが足りません。*/
	RpnEvaluator_InvalidNumericType			= Severity_Error | RpnEvaluatorGroup | 6,	/**< 無効な数値型のトークンが入力されました。*/
	RpnEvaluator_OperatorInvalidType		= Severity_Error | RpnEvaluatorGroup | 7,	/**< 演算子の型に無効な型({0})のオペランドが指定されています。*/

	// プリプロセッサ
	PreprocessorGroup = 0x00030000,
	Preprocessor_UnexpectedDirectiveToken	= Severity_Error | PreprocessorGroup | 1,	/**< 予期しないプリプロセッサディレクティブトークンが見つかりました。{0} */
	Preprocessor_SyntaxError				= Severity_Error | PreprocessorGroup | 2,	/**< プリプロセッサディレクティブの構文エラーです。*/
	Preprocessor_InvalidConstantExpression	= Severity_Error | PreprocessorGroup | 4,	/**< 整数定数式が無効です。*//* C1017 invalid integer constant expression */
	Preprocessor_UnexpectedElse				= Severity_Error | PreprocessorGroup | 5,	/**< 予期しない #else が見つかりました。*/
	Preprocessor_UnexpectedEndif			= Severity_Error | PreprocessorGroup | 6,	/**< 予期しない #endif が見つかりました。*/
	Preprocessor_UnexpectedElif				= Severity_Error | PreprocessorGroup | 6,	/**< 予期しない #elif が見つかりました。*/
	Preprocessor_NoExistsEndif				= Severity_Error | PreprocessorGroup | 7,	/**< このディレクティブの #endif がありません。*/
	Preprocessor_ExpectedDefinedId			= Severity_Error | PreprocessorGroup | 8,	/**< defined の後に識別子が必要です。*//* C2003 expected 'defined id' */
	Preprocessor_InvalidHeaderName			= Severity_Error | PreprocessorGroup | 9,	/**< ヘッダ名が無効です。*/
	Preprocessor_IncludeFileNotFount		= Severity_Error | PreprocessorGroup | 10,	/**< include ファイルを開けません。{0} */
};

enum class SeverityLevel
{
	Note,
	Warning,
	Error,
	Fatal,
};

/**
	@brief	診断情報1項目
*/
class DiagnosticsItem
{
public:
	DiagnosticsItem() = default;
	~DiagnosticsItem() = default;

	/** エラーコード */
	DiagnosticsCode GetCode() const { return m_code; }

	/** エラーの追加情報。エラー情報によって格納される値は異なる */
	const flStringArray& GetOptions() const { return m_options; }

private:
	friend class DiagnosticsItemSet;
	//DiagnosticsItem();
	//PathName	m_filePath;		// できるだけ DiagnosticsManager から直接代入するようにし、メモリを共有できるようにする
	DiagnosticsCode	m_code;
	int				m_lineNumber;
	int				m_columnNumber;
	flStringArray	m_options;
};

/**
	@brief	ファイル1つ単位をグループ化したもの

	@note	ファイルパスなど複数のエラーが持つ情報をまとめたもの。
			ファイルパス用のメモリを Item ごとに持たせないようにするのも目的の1つ。
			並列処理するときも、エラーレポートのたびにロックするのはよろしくない。
*/
class DiagnosticsItemSet
	: public Object
{
public:
	DiagnosticsItemSet(const PathNameA& absFilePath);
	~DiagnosticsItemSet() = default;

	const List<DiagnosticsItem>* GetItems() { return &m_items; }

	bool HasError() const { return !m_items.IsEmpty(); }

LN_INTERNAL_ACCESS:

	// 解析中に使用する情報
	int SetCurrentLineNumber(int line) { m_currentLineNumber = line; }
	int SetCurrentColumnNumber(int column) { m_currentColumnNumber = column; }

	void ClearItems() { m_items.Clear(); }
	void Report(DiagnosticsCode code);
	void Report(DiagnosticsCode code, flString option1);

private:
	PathNameA	m_absFilePath;
	int			m_currentLineNumber;
	int			m_currentColumnNumber;
	List<DiagnosticsItem>	m_items;
};

#define LN_DIAG_REPORT_ERROR(x, errorCode, ...)	\
	if (!(x)) { \
		m_diag->Report(errorCode); \
		return ResultState::Error; \
	}


/**
	@brief	
*/
class DiagnosticsManager
	: public Object
{
public:
	DiagnosticsManager() = default;
	~DiagnosticsManager() = default;

	void Clear();
	DiagnosticsItemSet* CreateItemSet(const PathNameA& absFilePath);

private:
	List<RefPtr<DiagnosticsItemSet>>	m_itemSetList;
};

} // namespace fl

