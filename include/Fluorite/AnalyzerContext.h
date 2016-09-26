
#pragma once

namespace ln
{
namespace fl
{

/**
	@brief	構造解析のルートオブジェクト
*/
class AnalyzerContext
{
public:
	AnalyzerContext();
	virtual ~AnalyzerContext();

	void Analyze();

private:
};

} // namespace fl
} // namespace ln
