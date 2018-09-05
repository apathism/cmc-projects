#include "asl/messageexception.h"
#include "lexemanalyzerexception.h"
#include "lexemanalyzer.h"

#define ATHROW(x) throw LexemAnalyzerException((x), line, end)
static const char *errUnexpectedSymbol = "Unexpected symbol in input stream";
static const char *errBufferOverflow = "LexemAnalyzer buffer overflow";

using namespace script;
using namespace asl;

LexemAnalyzer::LexemAnalyzer()
{
	line = 1;
	lastchar = begin = end = 0;
	lastlexem.begin = -1;
	state = 'H';
}

bool LexemAnalyzer::isLexem() const
{
	return lastlexem.begin != -1;
}

void LexemAnalyzer::makeLexem(Lexem::LexemType t)
{
	if (lastlexem.begin != -1)
		throw MessageException(errBufferOverflow);
	lastlexem.begin = begin;
	lastlexem.end = end;
	lastlexem.line = line;
	lastlexem.type = t;
	lastlexem.value = value;
	begin = end;
	value = String();
}

LexemAnalyzer::CharCode LexemAnalyzer::getCode(char c)
{
	if (!c) return eof;
	if (c >= '0' && c <= '9') return number;
	if (c >= 'A' && c <= 'Z') return c_letter;
	if (c >= 'a' && c <= 'z') return letter;
	if (c == '"') return quote;
	if (c == '<') return less;
	if (c == '>') return greater;
	if (c == '=') return equal;
	if (c == ':') return colon;
	if (c == '!') return exclamation;
	if (c == '[' || c == ']' || c == '@' || c == ';') return delimiter;
	if
		(c == '+' || c == '-' || c == '*' || c == '/' ||
		c == '=' || c == '&' || c == '|')
			return operation;
	if (c == '\n') return newline;
	if (c == '\t' || c == '\r' || c == ' ') return space;
	return unknown;
}

void LexemAnalyzer::putChar(char c)
{
	if (lastchar) throw MessageException(errBufferOverflow);
	switch (state)
	{
		case 'H':
			++end;
			if (getCode(c)!=space && getCode(c)!=eof && getCode(c)!=newline) value.Push(c);
			switch (getCode(c))
			{
				case number: state = 'I'; break;
				case c_letter: state = 'F'; break;
				case letter: state = 'V'; break;
				case quote: state = 'T'; break;
				case less: state = 'L'; break;
				case greater: state = 'G'; break;
				case exclamation: state = 'N'; break;
				case colon: state = 'A'; break;
				case equal:
				case operation:
					 makeLexem(Lexem::sign);
					 break;
				case delimiter: makeLexem(Lexem::delimiter); break;
				case eof:
					state = 'S';
					++line;
					begin = end = 0;
					makeLexem(Lexem::eof);
					break;
				case newline:
					++line;
					end = 0;
				case space:
					begin = end;
					break;
				default:
					ATHROW(errUnexpectedSymbol);
			}
			break;
		case 'I':
			if (getCode(c) == number)
			{
				value.Push(c);
				++end;
			}
			else
			{
				makeLexem(Lexem::integer);
				state = 'H';
				lastchar = c;
			}
			break;
		case 'F':
			switch (getCode(c))
			{
				case letter:
				case c_letter:
				case number:
					value.Push(c);
					++end;
					break;
				default:
					makeLexem(Lexem::function);
					state = 'H';
					lastchar = c;
			}
			break;
		case 'V':
			switch (getCode(c))
			{
				case letter:
				case c_letter:
				case number:
					value.Push(c);
					++end;
					break;
				default:
					makeLexem(Lexem::variable);
					state = 'H';
					lastchar = c;
			}
			break;
		case 'T':
			++end;
			switch (getCode(c))
			{
				case quote:
					state = 'H';
					value.Push(c);
					makeLexem(Lexem::string);
					break;
				case eof:
					ATHROW("Unexpected end of file");
				default: value.Push(c); break;
			}
			break;
		case 'A':
			++end;
			switch (getCode(c))
			{
				case equal: value.Push(c); break;
				case eof: ATHROW(errUnexpectedSymbol);
				default: ATHROW(errUnexpectedSymbol);
			}
			state = 'H';
			makeLexem(Lexem::sign);
			break;
		case 'G':
		case 'L':
		case 'N':
			if (getCode(c) == equal)
			{
				value.Push(c);
				++end;
			}
			else
				lastchar = c;
			state = 'H';
			makeLexem(Lexem::sign);
			break;
		case 'S':
			ATHROW("Unexpected symbol after end of file");
		default:
			throw MessageException("Internal error: wrong state of the LexemAnalyzer");
	}
}

Lexem LexemAnalyzer::getLexem()
{
	Lexem temp = lastlexem;
	lastlexem.begin = -1;
	if (lastchar)
	{
		char tempc = lastchar;
		lastchar = 0;
		putChar(tempc);
	}
	return temp;
}
