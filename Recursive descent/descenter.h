#pragma once
#include <cassert>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "D:/TX/TX/TXLib.h"

#define IC_SYNTAXSERT(a)     if (!(a)) { $r printf_s ("Seems like you have a compilation error:\n" "%s\n" "%*s^\n", input, S[P].position_, ""); $d throw "syntax error"; }
#define IC_RUNTIMEASSERT(a)  if (!(a)) { $r printf_s ("Seems like you have a runtime error:\n"     "%s\n" "%*s^\n", input, S[P].position_, ""); $d throw "runtime error"; }

#include "descTree.h"

using nametable_t = std::unordered_map<std::string, int>;

class descenter
{
public:
	descenter (const char * data = "");
	void parser ();
	void analyse ();
	void run ();

	descTree::descNode * GetG ();
	descTree::descNode * GetE ();
	descTree::descNode * GetT ();
	descTree::descNode * GetS ();
	descTree::descNode * GetN ();
	descTree::descNode * GetP ();
	descTree::descNode * GetID ();
	descTree::descNode * GetR ();
	descTree::descNode * GetV ();
	descTree::descNode * GetIF ();
	descTree::descNode * GetLOOP ();

	int nodeRunner (descTree::descNode * node);

private:
	const char * input;
	int P;
	std::vector <lexema> S;
	nametable_t ids_;
	descTree tree_;
};

