
#pragma once

namespace fl {

/**
	@brief	
*/
class InputFile
	: public Object
{
public:
	InputFile(const PathNameA& filePath);
	InputFile(const PathNameA& filePath, const char* code, int length);
	~InputFile() = default;

private:
	PathNameA	m_filePath;
	StringA		m_code;
};

/**
	@brief	構造解析のルートオブジェクト
*/
class AnalyzerContext
{
public:
	AnalyzerContext();
	virtual ~AnalyzerContext();

	void RegisterInputFile(const PathNameA& filePath);
	void RegisterInputMemoryCode(const PathNameA& filePath, const char* code, int length = -1);

	void Analyze();

private:
	List<RefPtr<InputFile>>	m_inputFileList;
};

} // namespace fl
