#ifndef __SCRIPT_LEXEM
#define __SCRIPT_LEXEM

#include "asl/string.h"

namespace script
{
	struct Lexem
	{
		int line, begin, end;
		asl::String value;

		enum LexemType
		{
			variable,
			function,
			string,
			sign,
			integer,
			delimiter,
			eof
		} type;

		static asl::String getLexemTypeString(LexemType);
	};
};

#endif

