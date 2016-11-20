
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

	// RPN ��́E�]��
	RpnEvaluatorGroup = 0x00020000,
	RpnEvaluator_InvalidNumericLiteral		= Severity_Error | RpnEvaluatorGroup | 1,
	RpnEvaluator_UnexpectedToken			= Severity_Error | RpnEvaluatorGroup | 2,	/**< �\�����Ȃ��g�[�N����������܂����B*/
	RpnEvaluator_InsufficientToken			= Severity_Error | RpnEvaluatorGroup | 3,	/**< ����]�����邽�߂̃g�[�N�����s�����Ă��܂��B(�����������Ă��Ȃ�) */
	RpnEvaluator_InvalidFuncCallArgsCount	= Severity_Error | RpnEvaluatorGroup | 4,	/**< �֐��Ăяo���̈���������܂���B*/
	RpnEvaluator_InvalidOperatorSide		= Severity_Error | RpnEvaluatorGroup | 5,	/**< ���Z�q�̕ӂɑ΂���I�y�����h������܂���B*/
	RpnEvaluator_InvalidNumericType			= Severity_Error | RpnEvaluatorGroup | 6,	/**< �����Ȑ��l�^�̃g�[�N�������͂���܂����B*/
	RpnEvaluator_OperatorInvalidType		= Severity_Error | RpnEvaluatorGroup | 7,	/**< ���Z�q�̌^�ɖ����Ȍ^({0})�̃I�y�����h���w�肳��Ă��܂��B*/

	// �v���v���Z�b�T
	PreprocessorGroup = 0x00030000,
	Preprocessor_UnexpectedDirectiveToken	= Severity_Error | PreprocessorGroup | 1,	/**< �\�����Ȃ��v���v���Z�b�T�f�B���N�e�B�u�g�[�N����������܂����B{0} */
	Preprocessor_SyntaxError				= Severity_Error | PreprocessorGroup | 2,	/**< �v���v���Z�b�T�f�B���N�e�B�u�̍\���G���[�ł��B*/
	Preprocessor_InvalidConstantExpression	= Severity_Error | PreprocessorGroup | 4,	/**< �����萔���������ł��B*//* C1017 invalid integer constant expression */
	Preprocessor_UnexpectedElse				= Severity_Error | PreprocessorGroup | 5,	/**< �\�����Ȃ� #else ��������܂����B*/
	Preprocessor_UnexpectedEndif			= Severity_Error | PreprocessorGroup | 6,	/**< �\�����Ȃ� #endif ��������܂����B*/
	Preprocessor_UnexpectedElif				= Severity_Error | PreprocessorGroup | 6,	/**< �\�����Ȃ� #elif ��������܂����B*/
	Preprocessor_NoExistsEndif				= Severity_Error | PreprocessorGroup | 7,	/**< ���̃f�B���N�e�B�u�� #endif ������܂���B*/
	Preprocessor_ExpectedDefinedId			= Severity_Error | PreprocessorGroup | 8,	/**< defined �̌�Ɏ��ʎq���K�v�ł��B*//* C2003 expected 'defined id' */
	Preprocessor_InvalidHeaderName			= Severity_Error | PreprocessorGroup | 9,	/**< �w�b�_���������ł��B*/
	Preprocessor_IncludeFileNotFount		= Severity_Error | PreprocessorGroup | 10,	/**< include �t�@�C�����J���܂���B{0} */
};

enum class SeverityLevel
{
	Note,
	Warning,
	Error,
	Fatal,
};

/**
	@brief	�f�f���1����
*/
class DiagnosticsItem
{
public:
	DiagnosticsItem() = default;
	~DiagnosticsItem() = default;

	/** �G���[�R�[�h */
	DiagnosticsCode GetCode() const { return m_code; }

	/** �G���[�̒ǉ����B�G���[���ɂ���Ċi�[�����l�͈قȂ� */
	const flStringArray& GetOptions() const { return m_options; }

private:
	friend class DiagnosticsItemSet;
	//DiagnosticsItem();
	//PathName	m_filePath;		// �ł��邾�� DiagnosticsManager ���璼�ڑ������悤�ɂ��A�����������L�ł���悤�ɂ���
	DiagnosticsCode	m_code;
	int				m_lineNumber;
	int				m_columnNumber;
	flStringArray	m_options;
};

/**
	@brief	�t�@�C��1�P�ʂ��O���[�v����������

	@note	�t�@�C���p�X�ȂǕ����̃G���[���������܂Ƃ߂����́B
			�t�@�C���p�X�p�̃������� Item ���ƂɎ������Ȃ��悤�ɂ���̂��ړI��1�B
			���񏈗�����Ƃ����A�G���[���|�[�g�̂��тɃ��b�N����̂͂�낵���Ȃ��B
*/
class DiagnosticsItemSet
{
public:
	DiagnosticsItemSet() = default;
	~DiagnosticsItemSet() = default;

	const List<DiagnosticsItem>* GetItems() { return &m_items; }

	bool HasError() const { return !m_items.IsEmpty(); }

LN_INTERNAL_ACCESS:
	// ��͒��Ɏg�p������
	int SetCurrentLineNumber(int line) { m_currentLineNumber = line; }
	int SetCurrentColumnNumber(int column) { m_currentColumnNumber = column; }

	void ClearItems() { m_items.Clear(); }
	void Report(DiagnosticsCode code);
	void Report(DiagnosticsCode code, flString option1);

private:
	PathName	m_filePath;
	int			m_currentLineNumber;
	int			m_currentColumnNumber;
	List<DiagnosticsItem>	m_items;
};

#define LN_DIAG_REPORT_ERROR(x, errorCode, ...)	\
	if (!(x)) { \
		m_diag->Report(errorCode); \
		return ResultState::Error; \
	}

} // namespace fl

