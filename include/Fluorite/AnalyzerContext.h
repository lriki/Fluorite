
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
	@brief	�\����͂̃��[�g�I�u�W�F�N�g
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
