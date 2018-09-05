#include "lexemanalyzerexception.h"

using namespace asl;
using namespace script;

LexemAnalyzerException::LexemAnalyzerException(const String &s, int line, int pos):
	MessageException(s + " on line " + String::fromInt(line) + " in position " + String::fromInt(pos))
{
}

