#ifndef __SCRIPT_LEXEMANALYZER
#define __SCRIPT_LEXEMANALYZER

#include "asl/string.h"
#include "lexem.h"

namespace script
{
	class LexemAnalyzer
	{
		char state, lastchar;
		int line, begin, end;
		asl::String value;
		Lexem lastlexem;

		enum CharCode
		{
			unknown,
			eof,
			number,
			letter,
			c_letter,
			quote,
			less,
			greater,
			exclamation,
			equal,
			colon,
			operation,
			delimiter,
			newline,
			space
		};

		static CharCode getCode(char c);
		void makeLexem(Lexem::LexemType t);

	public:
		LexemAnalyzer();
		void putChar(char c);
		bool isLexem() const;
		Lexem getLexem();
	};
};

#endif
