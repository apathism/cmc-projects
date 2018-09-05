#include "syntaxanalyzer.h"
#include "syntaxanalyzerexception.h"

#include "polizclean.h"
#include "poliznop.h"
#include "polizconst.h"
#include "polizmarker.h"
#include "polizjump.h"
#include "polizifjump.h"
#include "polizvar.h"
#include "polizoperation.h"
#include "polizfunction.h"

using namespace asl;
using namespace script;

static const char *errUnexpectedLexem = "unexpected type of lexem in expression";
static const char *errUnexpectedEOF = "unexpected end of file";
static const char *errUnexpectedDelimiter = "unexpected type of delimeter, ";
#define ATHROW(x) throw SyntaxAnalyzerException(getLexem(),(x))
#define DTHROW(x) ATHROW(errUnexpectedDelimiter+String(x))
#define PADD(x) poliz.Push(x);

SyntaxAnalyzer::SyntaxAnalyzer(const Array<Lexem> &l)
{
	clex = 0;
	this->l = &l;
	RuleS();
	if (!Finished())
		ATHROW("Unexpected lexems after the end of the file");
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

int SyntaxAnalyzer::getVariableID(const String &s)
{
	for (int j=0; j<varnames.Length(); ++j)
		if (varnames[j]==s)
			return j;
	varnames.Push(s);
	varvalues.Push(String());
	return varnames.Length()-1;
}

int SyntaxAnalyzer::getNamelessVariable()
{
	varnames.Push(String());
	varvalues.Push(String());
	return varnames.Length()-1;
}

int SyntaxAnalyzer::getPriority(const String &l)
{
	if (l == "[")
		return 0;
	if (l == ":=")
		return 1;
	if (l == "&" || l == "|")
		return 2;
	if (l == "<" || l == ">")
		return 3;
	if (l == "<=" || l == ">=")
		return 3;
	if (l == "=" || l == "!=")
		return 3;
	if (l == "+" || l == "-")
		return 4;
	if (l == "*" || l == "/")
		return 5;
	if (l == "-u" || l == "!u")
		return 6;
	return -1;
}

void SyntaxAnalyzer::popPriorityStack()
{
	PADD(PolizOperation(priorityStack[priorityStack.Length()-1]));
	priorityStack.Pop();
}

void SyntaxAnalyzer::cleanPriorityStack()
{
	int l;
	while ((l = priorityStack.Length()) && (getPriority(priorityStack[l-1])))
		popPriorityStack();
	if (l) priorityStack.Pop();
}

void SyntaxAnalyzer::pushPriorityStack(const String &s)
{
	int p = getPriority(s), l;
	if (p == -1)
		ATHROW("unexpected type of operator here");
	if (!p)
	{
		priorityStack.Push(s);
		return;
	}
	if (p <= 1 || p == 6)
	{
		while ((l = priorityStack.Length()) && (getPriority(priorityStack[l-1]) > p))
			popPriorityStack();
	}
	else
	{
		while ((l = priorityStack.Length()) && (getPriority(priorityStack[l-1]) >= p))
			popPriorityStack();
	}
	priorityStack.Push(s);
}

PointerArray& SyntaxAnalyzer::getPoliz()
{
	return poliz;
}

Array<String>& SyntaxAnalyzer::getVariables()
{
	return varvalues;
}

const Lexem& SyntaxAnalyzer::getLexem() const
{
	return (*l)[clex];
}

bool SyntaxAnalyzer::Finished() const
{
	return getLexem().type == Lexem::eof;
}

void SyntaxAnalyzer::nextLexem()
{
	if (!Finished()) ++clex;
}

void SyntaxAnalyzer::RuleS()
{
	if (getLexem().type == Lexem::eof)
		ATHROW(errUnexpectedEOF);
	RuleE();
	RuleA();
}

void SyntaxAnalyzer::RuleA()
{
	if (getLexem().type != Lexem::delimiter) return;
	if (getLexem().value != ";") return;
	PADD(PolizClean());
	nextLexem();
	RuleS();
}

void SyntaxAnalyzer::RuleE()
{
	switch (getLexem().type)
	{
		case Lexem::eof:
			ATHROW(errUnexpectedEOF);
		case Lexem::delimiter:
			if (getLexem().value == "@")
			{
				nextLexem();
				if (getLexem().type != Lexem::variable)
					ATHROW("label name expected after \"@\"");
				PADD(PolizNop());
				int vid = getVariableID(getLexem().value);
				varvalues[vid] = String::fromInt(poliz.Length()-1);
				nextLexem();
			}
			else if (getLexem().value == "[")
				RuleO();
			else
				DTHROW("only \"@\" or \"[\" are acceptable here");
			break;
		default:
			RuleO();
	}
}

void SyntaxAnalyzer::RuleO()
{
	pushPriorityStack("[");
	String unary;
	switch (getLexem().type)
	{
		case Lexem::eof:
			ATHROW(errUnexpectedEOF);
		case Lexem::sign:
			unary = getLexem().value + "u";
			nextLexem();
			RuleX(unary);
			break;
		case Lexem::delimiter:
			if (getLexem().value != "[")
				DTHROW("only \"[\" is acceptable here");
		default:
			RuleX(String());
	}
	cleanPriorityStack();
}

void SyntaxAnalyzer::RuleX(const String &u)
{
	switch (getLexem().type)
	{
		case Lexem::integer:
		case Lexem::string:
			RuleK();
			break;
		case Lexem::function:
			RuleC();
			break;
		case Lexem::variable:
			RuleW();
			break;
		case Lexem::delimiter:
			if (getLexem().value != "[")
				DTHROW("only \"[\" is acceptable here");
			nextLexem();
			RuleS();
			if (getLexem().type != Lexem::delimiter)
				ATHROW("expected \"]\"");
			if (getLexem().value != "]")
				DTHROW("only \"]\" is acceptable here");
			nextLexem();
			break;
		default:
			ATHROW(errUnexpectedLexem);
	}
	if (u.Length())
		pushPriorityStack(u);
	RuleY();
}

void SyntaxAnalyzer::RuleY()
{
	if (getLexem().type != Lexem::sign)
		return;
	pushPriorityStack(getLexem().value);
	nextLexem();
	RuleX(String());
}

void SyntaxAnalyzer::RuleC()
{
	if (getLexem().type != Lexem::function)
		ATHROW("function name expected");
	String fname = getLexem().value;
	if (fname == "If")
	{
		nextLexem();
		RuleUIf(0);
		return;
	}
	if (fname == "Goto")
	{
		nextLexem();
		RuleU();
		PADD(PolizJump());
		return;
	}
	PADD(PolizMarker());
	nextLexem();
	RuleU();
	PADD(PolizFunction(fname));
}

void SyntaxAnalyzer::RuleW()
{
	if (getLexem().type != Lexem::variable)
		ATHROW("variable name expected");
	PADD(PolizMarker());
	int vid = getVariableID(getLexem().value);
	nextLexem();
	RuleU();
	PADD(PolizVar(vid));
}

int SyntaxAnalyzer::RuleUIf(int state)
{
	if (getLexem().type != Lexem::delimiter || getLexem().value != "[")
	{
		if (state < 2) ATHROW("too few If arguments");
		if (state == 2) PADD(PolizNop());
		return poliz.Length()-1;
	}
	if (state > 2) ATHROW("too many If arguments");

	nextLexem();
	RuleS();

	if (getLexem().type != Lexem::delimiter)
		ATHROW("delimiter expected");
	if (getLexem().value != "]")
		DTHROW("only \"]\" is acceptable here");
	nextLexem();

	int tid = (state < 2) ? getNamelessVariable() : 0;
	switch(state)
	{
		case 0:
			PADD(PolizMarker());
			PADD(PolizVar(tid));
			PADD(PolizIfJump());
			break;
		case 1:
			PADD(PolizMarker());
			PADD(PolizVar(tid));
			PADD(PolizJump());
		default:
			break;
	}
	int end = poliz.Length()-1;
	int nextend = RuleUIf(state+1);
	if (state < 2)
		varvalues[tid] = String::fromInt(nextend+1);
	return end;
}

void SyntaxAnalyzer::RuleU()
{
	if (getLexem().type != Lexem::delimiter) return;
	if (getLexem().value != "[") return;
	nextLexem();
	RuleS();
	if (getLexem().type != Lexem::delimiter)
		ATHROW("delimiter expected");
	if (getLexem().value != "]")
		DTHROW("only \"]\" is acceptable here");
	nextLexem();
	RuleU();
}

void SyntaxAnalyzer::RuleK()
{
	String temp;
	switch (getLexem().type)
	{
		case Lexem::string:
			temp = getLexem().value;
			temp = temp.subString(1, temp.Length()-1);
			break;
		case Lexem::integer:
			temp = getLexem().value;
			break;
		default:
			ATHROW("integer or string constant expected");
	}
	nextLexem();
	PADD(PolizConst(temp));
}
