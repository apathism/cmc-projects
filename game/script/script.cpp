#include <fcntl.h>
#include <unistd.h>
#include "asl/syscallexception.h"
#include "asl/messageexception.h"
#include "script.h"
#include "lexemanalyzer.h"
#include "syntaxanalyzer.h"
#include "polizfunction.h"
#include "stdcallback.h"

using namespace asl;
using namespace script;

Script::Script(const String &filename)
{
	char* filestr = filename.cstring();
	fd = open(filestr,O_RDONLY);
	free(filestr);
	if (fd == -1) throw SyscallException("open");
}

void Script::Execute()
{
	if (fd == -1)
		throw MessageException("Script cannot be executed");
	// Lexem Analyzer Stage
	LexemAnalyzer la;
	Array<Lexem> lexems;
	char c;
	while (read(fd, &c, sizeof(c)))
	{
		la.putChar(c);
		while (la.isLexem())
			lexems.Push(la.getLexem());
	}
	la.putChar('\0');
	while (la.isLexem())
		lexems.Push(la.getLexem());
	// Syntax Analyzer State
	SyntaxAnalyzer sa(lexems);
	PointerArray &poliz = sa.getPoliz();
	Array<String> &vars = sa.getVariables();
	// Interpreter
	PolizFunction::RegisterCallback(new StdCallback());
	PointerArray stack;
	for (int pos = 0; 0 <= pos && pos < poliz.Length(); )
		pos = dynamic_cast<PolizItem&>(poliz[pos]).Evaluate(vars, stack, pos);
	// Finish
	PolizFunction::UnregisterCallbacks();
	fd = -1;
}

void Script::RegisterCallback(Callback* c)
{
	PolizFunction::RegisterCallback(c);
}
