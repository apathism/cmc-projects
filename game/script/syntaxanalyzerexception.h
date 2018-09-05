#ifndef __SCRIPT_SYNTAXANALYZEREXCEPTION
#define __SCRIPT_SYNTAXANALYZEREXCEPTION

#include "lexem.h"
#include "asl/messageexception.h"

namespace script
{
	class SyntaxAnalyzerException: public asl::MessageException
	{
	public:
		SyntaxAnalyzerException(const Lexem&, const asl::String&);
	};
}

#endif
