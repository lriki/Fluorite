
#pragma once
#include <Lumino/Base/Collection.h>

namespace ln
{
namespace fl
{


class Token
{

};

class TokenManager
{
public:
	TokenManager();
	virtual ~TokenManager();

	Token* CreateToken();

private:
	//Array<Token*>	m_tokenBuffer;
	//Stack<int>		m_indexStack;
};


/**
	@brief	
*/
class TokenList
	: public RefObject
	, public Collection<Token*>
{
public:
	TokenList() {}
	~TokenList() {}

	String ToString(int begin, int end) const
	{
		StringBuilder sb;
		for (int i = begin; i < end; ++i)
		{
			sb.Append(GetAt(i).GetBegin(), GetAt(i).GetLength());
		}
		return sb.ToString();
	}

	String ToStringValidCode() const
	{
		StringBuilder sb;
		for (int i = 0; i < GetCount(); ++i)
		{
			if (GetAt(i).IsValid())
			{
				sb.Append(GetAt(i).GetBegin(), GetAt(i).GetLength());
			}
		}
		return sb.ToString();
	}
};

} // namespace fl
} // namespace ln
