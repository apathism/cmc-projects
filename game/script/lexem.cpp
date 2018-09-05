#include "lexem.h"

using namespace asl;
using namespace script;

String Lexem::getLexemTypeString(Lexem::LexemType t)
{
	static const char* strs[] = {
		"variable",
		"function",
		"string",
		"sign",
		"integer",
		"delimiter",
		"end-of-file"
	};
	return strs[t];
}
