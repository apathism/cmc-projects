#ifndef __SCRIPT_LEXEMANALYZEREXCEPTION
#define __SCRIPT_LEXEMANALYZEREXCEPTION

#include "asl/messageexception.h"

namespace script
{
	class LexemAnalyzerException: public asl::MessageException
	{
	public:
		LexemAnalyzerException(const asl::String&, int line, int pos);
	};
}

#endif
