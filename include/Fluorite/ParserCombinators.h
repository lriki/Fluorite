/*
	一番シンプル
		static Result<Data> Statement(ParserContext& parser)
		{
			auto t1 = parser.Eval(Token(ln::parser::CommonTokenType::Identifier));
			auto t2 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
			auto t3 = parser.Eval(Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)));
			auto t4 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
			return parser.Success(Data{ t1.ToString(), t3.ToString() }, parser.GetPosition());
		}
	↑のをちゃんとした Parser を返すようにしたもの
		static Parser<Data> Statement(ParserContext& parser)
		{
			return [](ParserContext& parser)
			{
				auto t1 = parser.Eval(Token(ln::parser::CommonTokenType::Identifier));
				auto t2 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
				auto t3 = parser.Eval(Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)));
				auto t4 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
				return parser.Success(Data{ t1.ToString(), t3.ToString() }, parser.GetPosition());
			};
		}
	本当に関数型っぽくするならこうなる 1
		static Parser<Data> Statement(ParserContext& parser)
		{
			Parser<Token> p, t1, t2, t3, t4;
			p = (t1 = Token(ln::parser::CommonTokenType::Identifier))
			&&	(t2 = Token(ln::parser::CommonTokenType::Operator)))
			&&	(t3 = Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)))
			&&	(t4 = Token(ln::parser::CommonTokenType::Operator));
			return [p, t1, t3](ParserContext& parser) { return p(parser).Then( Data(t1.GetValue, t2.GetValue()) ) };
		}
	
	本当に関数型っぽくするならこうなる 2
		static Parser<Data> Statement(ParserContext& parser)	// 値渡しにするか、呼び出し元を DoParse とか1つかませないとだめ
		{
			return
				(parser[0] = Token(ln::parser::CommonTokenType::Identifier))
			|	(parser[1] = Token(ln::parser::CommonTokenType::Operator)))
			|	(parser[2] = Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)))
			|	(parser[3] = Token(ln::parser::CommonTokenType::Operator));
			->	[](ParserContext& parser) { return Data(parser[0].GetValue(), parser[1].GetValue()) };
		}
	
	一番シンプルなのをマクロ使うようにすると、例外longjmpしなくて済む
		static Result<Data> Statement(ParserContext& parser)
		{
			LN_PARSE(t1, Token(ln::parser::CommonTokenType::Identifier));
			LN_PARSE(t2, Token(ln::parser::CommonTokenType::Operator));
			LN_PARSE(t3, Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)));
			LN_PARSE(t4, Token(ln::parser::CommonTokenType::Operator));
			return parser.Success(Data{ t1.ToString(), t3.ToString() });
		}
*/

#pragma once

#define LN_PARSE(result, parser) \
	auto result##_ = (parser)(input); \
	if (result##_.IsFailed()) return input.Fail(result##_); \
	input.Next(result##_); \
	auto result = result##_.GetValue();

#define LN_PARSE_RESULT(result, parser) \
	auto result = (parser)(input); \
	if (result.IsFailed()) return input.Fail(result##_); \
	input.Next(result);

//#define LN_PARSE_SUCCESS(value)	\
//	Success(value, input);

namespace fl {
namespace combinators {

namespace detail {

template<typename TCursor>
class ParserFailure
{
public:
	ParserFailure(const TCursor& cursor, const flStringRef& message_)
		: remainder(cursor)
		, message(message_)
	{}

	TCursor		remainder;
	flStringRef	message;
};

} // namespace detail


// ParserResult の決まりごと
//	- 成否 (true/false) を持つ
//	- 値を持つ (失敗の場合は不正値)
//	- 次の読み取り位置を持つ (remainder)
// T はパーサ関数の戻す値
template<typename T, typename TCursor>
class GenericParserResult
{
public:
	static GenericParserResult<T, TCursor> Success(const T& value, int matchBegin, int matchEnd, const TCursor& remainder)
	{
		return GenericParserResult<T, TCursor>(value, matchBegin, matchEnd, remainder, true, flString::GetEmpty());
	}
	static GenericParserResult<T, TCursor> Fail(const TCursor& remainder, const flStringRef& message)
	{
		return GenericParserResult<T, TCursor>(T(), 0, 0, remainder, false, message);
	}

	const T& GetValue() const { return m_value; }
	const TCursor& GetRemainder() const { return m_remainder; }	// 評価後の次の読み取り位置
	int GetRemainderPosition() const { return m_remainder.GetPosition(); }
	bool IsSucceed() const { return m_isSuccess; }
	bool IsFailed() const { return !m_isSuccess; }
	int GetMatchBegin() const { return m_matchBegin; }
	int GetMatchEnd() const { return m_matchEnd; }
	const flString& GetMessage() const { return m_message; }

	// Parser 関数の return で Fail() によって、Parse 失敗であることを伝え、Value を捨てるために使う
	GenericParserResult(const detail::ParserFailure<TCursor>& failer)
		: m_value()
		, m_matchBegin(0)
		, m_matchEnd(0)
		, m_remainder(failer.remainder)
		, m_isSuccess(false)
		, m_message(failer.message)
	{
	}
	
private:
	GenericParserResult(const T& value, int matchBegin, int matchEnd, const TCursor& remainder, bool isSuccess, const flStringRef& message)
		: m_value(value)
		, m_matchBegin(matchBegin)
		, m_matchEnd(matchEnd)
		, m_remainder(remainder)
		, m_isSuccess(isSuccess)
		, m_message(message)
	{
	}

	T			m_value;
	int			m_matchBegin;
	int			m_matchEnd;
	TCursor		m_remainder;
	bool		m_isSuccess;
	flString	m_message;
};

template<typename T>
class Option
{
public:
	// TODO: move

	static Option Some(const T& value) { return Option(value); }
	static Option None() { return Option(); }

	bool IsEmpty() const { return m_empty; }
	const T& GetValue() const { return m_value; }

private:
	Option(const T& value)
		: m_vale(value)
		, m_empty(false)
	{}
	Option()
		: m_vale()
		, m_empty(true)
	{}

	T		m_vale;
	bool	m_empty;
};

struct ParserCursorConditional
{
	struct Always
	{
		template<typename T>
		bool operator()(const T& value)
		{
			return true;
		}
	};
};

template<typename TTokenFilter = ParserCursorConditional::Always>
class GenericParserCursor
{
public:
	GenericParserCursor()
		: m_tokenList(nullptr)
		, m_position(0)
	{}

	// パース開始時の初期化用
	GenericParserCursor(const TokenList* tokenList)
		: m_tokenList(tokenList)
		, m_position(0)
	{
	}

	// パース開始時の初期化用
	GenericParserCursor(const TokenList* tokenList, int position)
		: m_tokenList(tokenList)
		, m_position(position)
	{
	}

	GenericParserCursor(const GenericParserCursor& obj)
		: m_tokenList(obj.m_tokenList)
		, m_position(obj.m_position)
	{
	}

	GenericParserCursor& operator=(const GenericParserCursor& obj)
	{
		m_tokenList = obj.m_tokenList;
		m_position = obj.m_position;
		return *this;
	}

	const Token& GetCurrentValue() const
	{
		return m_tokenList->GetAt(m_position);
	}

	int GetPosition() const
	{
		return m_position;
	}

	GenericParserCursor Cuing() const
	{
		TTokenFilter cond;
		int pos = m_position;
		while (!cond(m_tokenList->GetAt(pos)))
		{
			++pos;
		};
		return GenericParserCursor(m_tokenList, pos);
	}

	GenericParserCursor Advance() const
	{
		if (m_position == m_tokenList->GetCount())
		{
			LN_THROW(0, ln::InvalidOperationException, "end of source.");
		}

		TTokenFilter cond;
		int pos = m_position;
		do
		{
			++pos;
		} while (pos < m_tokenList->GetCount() && !cond(m_tokenList->GetAt(pos)));

		return GenericParserCursor(m_tokenList, pos);
	}

private:
	const TokenList*	m_tokenList;
	int					m_position;
};


template<typename TCursor>
class GenericParserContext
{
public:
	GenericParserContext(const TCursor& input)
		: m_start(input)
		, m_current(input)
	{
	}

	GenericParserContext(const GenericParserContext& obj)
		: m_start(obj.m_current)
		, m_current(obj.m_current)
	{
	}

	TCursor GetNext() const
	{
		return m_current.Advance();
		//m_current = m_current.Advance();
		//return m_current;
	}

	const Token& GetCurrentValue() const
	{
		return m_current.GetCurrentValue();
	}

	const TCursor& GetStartCursor() const
	{
		return m_start;
	}

	int GetStartPosition() const
	{
		return m_start.GetPosition();
	}

	int GetLastMatchEndPosition() const
	{
		return m_last.GetPosition() + 1;	// 最後のマッチ位置の次
	}

	const TCursor& GetCurrentCursor() const
	{
		return m_current;
	}

	template<typename T>
	void Next(const GenericParserResult<T, TCursor>& result/*const TCursor& newPos*/)
	{
		m_last = m_current;
		m_current = result.GetRemainder();
	}

	template<typename T>
	GenericParserResult<T, TCursor> Success(const T& value)
	{
		return GenericParserResult<T, TCursor>::Success(value, GetStartPosition(), GetLastMatchEndPosition(), m_current);
	}

	template<typename TResult>
	detail::ParserFailure<TCursor> Fail(const TResult& result)
	{
		return detail::ParserFailure<TCursor>(GetCurrentCursor(), result.GetMessage());
	}

private:
	TCursor		m_start;
	TCursor		m_current;
	TCursor		m_last;
};


template<typename TValue, typename TCursor, typename TContext>
class GenericParser : public std::function<GenericParserResult<TValue, TCursor>(TContext)>//public ln::Delegate<GenericParserResult<TValue, TCursor>(TContext)>
{
public:
	template<typename TParserFunc>
	GenericParser(TParserFunc func)
		: std::function<GenericParserResult<TValue, TCursor>(TContext)>(func)
	{
	}

	GenericParser operator||(const GenericParser& second) const
	{
		return ParseLib<TCursor>::Or(*this, second);
	}
};




template<typename TTokenFilter = ParserCursorConditional::Always>
class ParseLib
{
public:
	using ValueT = Token;

	using ParserContext = GenericParserContext<GenericParserCursor<TTokenFilter>>;

	template<typename TValue>
	using Parser = GenericParser<TValue, GenericParserCursor<TTokenFilter>, ParserContext>;

	template<typename TValue>
	using ParserResult = GenericParserResult<TValue, GenericParserCursor<TTokenFilter>>;

	using ParserCursor = GenericParserCursor<TTokenFilter>;


	/** 指定した1文字のトークンにマッチする */
	static Parser<ValueT> TokenChar(char ch)
	{
		return [ch](ParserContext input)
		{
			auto& tok = input.GetCurrentValue();
			if (tok.EqualChar(ch))
				return ParserResult<ValueT>::Success(tok, input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor(), flString::Format("Unexpected token \"{0}\". expected \"{1}\"", tok.GetString(), ch));
		};
	}

	static Parser<ValueT> TokenString(const char* str_)
	{
		String str = str_;
		return [str](ParserContext input)
		{
			if (input.GetCurrentValue().EqualString(str.c_str(), str.GetLength()))
				return ParserResult<ValueT>::Success(input.GetCurrentValue(), input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor());	// TODO: メッセージあるとよい
		};
	}

	static Parser<ValueT> Token(TokenGroup type)
	{
		return [type](ParserContext input)
		{
			if (input.GetCurrentValue().GetCommonType() == type)
				return ParserResult<ValueT>::Success(input.GetCurrentValue(), input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor());	// TODO: メッセージあるとよい
		};
	}

	static Parser<ValueT> Token(TokenGroup type, char ch)
	{
		return [type, ch](ParserContext input)
		{
			if (input.GetCurrentValue().GetCommonType() == type && input.GetCurrentValue().EqualChar(ch))
				return ParserResult<ValueT>::Success(input.GetCurrentValue(), input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor());
		};
	}

	static Parser<ValueT> Token(TokenGroup type, const char* string, int len)
	{
		return [type, string, len](ParserContext input)
		{
			if (input.GetCurrentValue().GetCommonType() == type && input.GetCurrentValue().EqualString(string, len))
				return ParserResult<ValueT>::Success(input.GetCurrentValue(), input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor());
		};
	}

	// 0回以上の繰り返し
	template<typename T>
	static Parser<List<T>> Many(const Parser<T>& parser)
	{
		return [parser](ParserContext input)
		{
			List<T> list;
			auto r = parser.Call(input);
			//input = r.GetRemainder();

			while (r.IsSucceed())
			{
				list.Add(r.GetValue());
				r = parser.Call(r.GetRemainder());
				//input = r.GetRemainder();
			}
			return ParserResult<List<T>>::Success(list, input.GetStartPosition(), r.GetMatchEnd(), r.GetRemainder());
		};
	}

	template<typename T>
	static Parser<T> Or(const Parser<T>& first, const Parser<T>& second)
	{
		//Parser<T> second(second_);
		return [first, second](ParserContext input)
		{
			auto fr = first.Call(input);
			if (fr.IsFailed())
			{
				return second.Call(input);
			}
			return fr;
		};
	}

	template<typename T>
	static Parser<Option<T>> Optional(const Parser<T>& parser)
	{
		return [parser](ParserContext input)
		{
			auto r = parser.Call(input);
			if (r.IsSucceed())
			{
				return ParserResult<Option<T>>::Success(Option<T>::Some(r.GetValue()), input.GetStartPosition(), r.GetMatchEnd(), r.GetRemainder());
			}
			return ParserResult<Option<T>>::Success(Option<T>::None(), input.GetStartPosition(), input.GetStartPosition(), input.GetStartCursor());
		};
	}

	// term までをマッチの範囲とし、
	// ターミネータを result に含む
	template<typename T>
	static Parser<List<T>> UntilMore(const Parser<T>& term)
	{
		return [term](ParserContext input)
		{
			List<T> list;
			auto r = term.Call(input);
			auto lastResult = r;

			while (r.IsFailed())
			{
				list.Add(r.GetValue());
				lastResult = r;
				r = term.Call(r.GetRemainder().Advance());
			}

			// TODO: ストリーム末尾までfailedだったらパース失敗

			return ParserResult<List<T>>::Success(list, input.GetStartPosition(), r.GetMatchEnd(), r.GetRemainder());
		};
	}

	template<typename T>
	static ParserResult<T> TryParse(const Parser<T>& parser, const TokenList* tokenList)
	{
		ParserCursor input(tokenList);
		ParserResult<T> result = parser(input.Cuing());
		return result;
	}

	template<typename T>
	static ParserResult<T> TryParse(ParserResult<T>(*parser)(ParserContext), const TokenList* tokenList)
	{
		ParserCursor input(tokenList);
		ParserResult<T> result = parser(input.Cuing());
		return result;
	}
};


//struct ParserCursorCondition_SkipSpace
//{
//	bool operator()(const Token& token)
//	{
//		return
//			token.EqualChar('<') || token.EqualChar('>') || token.EqualChar('{') || token.EqualChar('}') ||
//			token.EqualString("sampler_state", 13) ||
//			token.EqualString("technique", 9) ||
//			token.EqualString("pass", 4) ||
//			token.IsEof();	// TODO: これが無くてもいいようにしたい。今はこれがないと、Many中にEOFしたときOutOfRangeする
//	}
//};
//
//using ParserCursor_SkipSpace = combinators::GenericParserCursor<ParserCursorCondition_SkipSpace>;

} // namespace combinators
} // namespace fl
