#pragma once
#include <string>

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
	lexema (int num);
	lexema (std::string str, lexema::type type);

	char ch ();

	std::string str_;
	int num_;
	type type_;

	int position_;
};
