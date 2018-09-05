#include "syntaxanalyzerexception.h"

using namespace asl;
using namespace script;

SyntaxAnalyzerException::SyntaxAnalyzerException(const Lexem &l, const String &s):
	MessageException(
		"On the lexem of type " +
		Lexem::getLexemTypeString(l.type) +
		" (" + String::fromInt(l.line) + "," +
		String::fromInt(l.begin) + "-" +
		String::fromInt(l.end) + "): " + s
	)
{
}

