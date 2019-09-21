
#define _TX_ALLOC_BREAK 299

#include "D:/TX/TX/TXLib.h"
#include <string>
#include "descenter.h"
#include "list.h"
#include "unordered_map.h"

 //(1 + 3 * (2 - 3)), (2 + 2 / 2) * 3
 //(), -3, * 4, 3 (2

 //G :: = R

 //R :: = {ID '=' E} | E

 //E :: = T{ ['+' '-'] T } *

 //T :: = S{ ['*' '/'] S } *

 //S :: = P{ '^' P } *

 //N :: = ['0' - '9'] +
	
 //P :: = '(' E + ')' | N | ID

 //ID :: = {['a' - 'z'] | ['A' - 'Z']} {['a' - 'z'] | ['A' - 'Z'] ['0' - '9'] }*

std::string ArgLine (int argv, char** argc);

int main (int argv, char **argc)
{
	std::string string = ArgLine (argv, argc).c_str ();

	descenter desc (string.c_str());

	desc.parser ();
}

std::string ArgLine (int argv, char** argc)
{	

	std::string data;

	if (argv == 1)
		return data;

	for (int i = 1; i < argv; i++)
	{
		data += std::string (argc[i]);
	}

	return data;
}

