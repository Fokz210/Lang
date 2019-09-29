#pragma once
#include <string>

enum op_t
{
	OP_NONE,
	OP_SUM,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_EQ,
	OP_POW,
	OP_LOOP,
	OP_IF,
	OP_ID,
	OP_COMP,
	OP_GREATER,
	OP_LESS,
	OP_GOE,
	OP_LOE,
	OP_E	
};

class lexema
{
public:
	enum type
	{
		none,
		num,
		op,
		str,
		ID
	};

	lexema ();
	lexema (double num);
	lexema (std::string str, lexema::type type);
	lexema (op_t op);

	char ch ();

	std::string str_;
	op_t op_;
	double num_;
	type type_;

	int position_;
};
