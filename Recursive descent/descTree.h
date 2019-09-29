#pragma once
#include <vector>
#include "lexema.h"

class descTree
{
public:
	class descNode
	{
	public:
		struct OPERPTR
		{
			op_t oper;
			descNode* ptr = nullptr;
		};

		descNode (lexema data);
		~descNode ();

		lexema data;
		std::vector <OPERPTR> children_;
	};

	descTree ();
	~descTree ();

	void swap (descTree & other);

	descNode* head_;
};


