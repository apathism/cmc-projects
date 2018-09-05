#ifndef __SCRIPT_SYNTAXANALYZER
#define __SCRIPT_SYNTAXANALYZER

#include "asl/array.h"
#include "asl/pointerarray.h"
#include "lexem.h"
#include "polizitem.h"

namespace script
{
	class SyntaxAnalyzer
	{
		int clex;
		const asl::Array<Lexem> *l;

		bool Finished() const;
		void nextLexem();
		const Lexem& getLexem() const;

		asl::PointerArray poliz;

		asl::Array<asl::String> varnames;
		asl::Array<asl::String> varvalues;
		int getVariableID(const asl::String&);
		int getNamelessVariable();

		asl::Array<asl::String> priorityStack;
		int getPriority(const asl::String&);
		void pushPriorityStack(const asl::String&);
		void popPriorityStack();
		void cleanPriorityStack();

		void RuleS();
		void RuleA();
		void RuleE();
		void RuleO();
		void RuleX(const asl::String&);
		void RuleY();
		void RuleC();
		void RuleW();
		void RuleU();
		int  RuleUIf(int);
		void RuleK();
	public:
		SyntaxAnalyzer(const asl::Array<Lexem>&);
		asl::PointerArray& getPoliz();
		asl::Array<asl::String>& getVariables();
		~SyntaxAnalyzer();
	};
}

#endif
