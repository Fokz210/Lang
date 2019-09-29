#include "descenter.h"

descenter::descenter (const char * data) :
	input (data),
	P (0),
	S (0),
	ids_ (),
	tree_ ()
{

}

void descenter::parser ()
{
	if (!strcmp (input, ""))
	{
		while (true)
		{
			P = 0;
			char * data = new char[256] {};

			printf_s ("|ChereLang> ");
			gets_s (data, 256);

			input = data;

			if (!strcmp (input, "exit"))
			{
				delete[] data;

				return;
			}

			analyse ();

			tree_.head_ = GetG ();

			printf_s ("|  %f\n", nodeRunner (tree_.head_));

			delete[] data;

			{
				descTree newTree;
				tree_.swap(newTree);
			}
		}
	}
	else
	{
		analyse ();
		tree_.head_ = GetG ();
		printf_s ("%d\n", nodeRunner (tree_.head_));
	}
}

void descenter::analyse ()
{
	S.clear ();
	P = 0;

	lexema curr;
	while (input[P] != '\0')
	{
		curr.str_ = std::string();
		curr.num_ = 0;
		curr.type_ = lexema::type::none;
		curr.position_ = P;

		if ('0' <= input[P] && input[P] <= '9')
		{
			int afterdot = 0;
			while (('0' <= input[P] && input[P] <= '9') || input[P] == '.')
			{
				if (input[P] == '.')
				{
					afterdot = P;
					P++;
				}

				curr.type_ = lexema::type::num;
				curr.num_ = curr.num_ * 10 + input[P] - '0';
				P++;
			}

			int diff = 1;
			if (afterdot) diff = 10 * (P - afterdot - 1);

			curr.num_ /= diff;

			S.push_back (curr);
		}
		else if (input[P] == ' ')
			P++;
		else if (('a' <= input[P] && input[P] <= 'z') || ('A' <= input[P] && input[P] <= 'Z'))
		{
			while (('a' <= input[P] && input[P] <= 'z') || ('A' <= input[P] && input[P] <= 'Z') || ('0' <= input[P] && input[P] <= '9'))
			{
				curr.type_ = lexema::type::str;
				curr.str_ += input[P];
				P++;
			}

			S.push_back (curr);
		}
		else
		{
			
			curr.type_ = lexema::type::op;
			curr.str_ += input[P];
			P++;

			while (!('0' <= input[P] && input[P] <= '9') && input[P] != ' ' && curr.str_ != "(" && curr.str_ != ")")
			{
				curr.str_ += input[P];
				P++;
			}


			S.push_back (curr);
		}
	}

	S.push_back (lexema ());

	P = 0;
}


descTree::descNode * descenter::GetG ()
{
	return GetE ();
}

descTree::descNode * descenter::GetE ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema (OP_SUM));

	retVal->children_.push_back ({ OP_NONE, GetT () });

	while (S[P].ch () == '+' || S[P].ch () == '-')
	{
		char op = S[P].ch ();
		P++;

		if (op == '-')
			retVal->children_.push_back ({ OP_SUB, GetT () });
		else
			retVal->children_.push_back ({ OP_SUM, GetT () });
	}

	if (S[P].ch () == ';')
		P++;

	return retVal;
}

descTree::descNode * descenter::GetT ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema (OP_MUL));
	retVal->children_.push_back ({ OP_NONE, GetS () });

	while (S[P].ch () == '*' || S[P].ch () == '/')
	{
		char op = S[P].ch ();
		P++;

		if (op == '*')
			retVal->children_.push_back ({ OP_MUL, GetS () });
		else
			retVal->children_.push_back ({ OP_DIV, GetS () });
	}

	return retVal;
}

descTree::descNode * descenter::GetS ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema (OP_POW));
	retVal->children_.push_back ({OP_NONE , GetV () });

	if (S[P].ch () == '^')
	{
		P++;

		retVal->children_.push_back ({ OP_NONE, GetV () });
	}

	return retVal;
}

descTree::descNode * descenter::GetN ()
{
	IC_SYNTAXSERT (S[P].type_ == lexema::type::num);
	descTree::descNode * retVal = new descTree::descNode (lexema (S[P].num_));

	P++;

	return retVal;
}

descTree::descNode * descenter::GetP ()
{
	descTree::descNode * retVal = nullptr;

	if (S[P].ch () == '(')
	{
		P++;
		retVal = GetE ();
		IC_SYNTAXSERT (S[P].ch () == ')');
		P++;
	}
	else if (S[P].str_ == "if")
		retVal = GetIF ();
	else if (S[P].str_ == "loop")
		retVal = GetLOOP ();
	else if (S[P].type_ == lexema::type::num)
		retVal = GetN ();
	else
		retVal = GetR ();

	return retVal;
}

descTree::descNode * descenter::GetID ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema (OP_ID));

	retVal->children_.push_back ({ OP_NONE, new descTree::descNode (lexema (S[P].str_, lexema::type::str)) });

	P++;

	return retVal;
}

descTree::descNode * descenter::GetR ()
{
	descTree::descNode * retVal = nullptr;

	if (P < S.size() - 1 && S[P + 1].str_ == "=")
	{
		retVal = new descTree::descNode (lexema (OP_EQ));

		std::string ID = S[P].str_;

		retVal->children_.push_back ({ OP_NONE, new descTree::descNode (lexema (ID, lexema::type::str)) });

		P += 2;

		retVal->children_.push_back ({ OP_NONE, GetE () });
	}
	else
	{
		retVal = GetID ();
	}

	return retVal;
}

descTree::descNode * descenter::GetV ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema (OP_COMP));

	retVal->children_.push_back ({OP_NONE, GetP () });

	if (S[P].ch () == '>' || S[P].ch () == '<' || S[P].str_ == "==")
	{
		op_t oper = OP_NONE;

		if (S[P].str_ == ">")
			oper = OP_GREATER;
		else if (S[P].str_ == "<")
			oper = OP_LESS;
		else if (S[P].str_ == ">=")
			oper = OP_GOE;
		else if (S[P].str_ == "<=")
			oper = OP_LOE;
		else if (S[P].str_ == "==")
			oper = OP_EQ;

		P++;
		
		retVal->children_.push_back ({ oper, GetP () });
	}

	return retVal;
}

descTree::descNode * descenter::GetIF ()
{
	descTree::descNode * retVal = nullptr;

	IC_SYNTAXSERT (S[P++].str_ == "if");
	IC_SYNTAXSERT (S[P++].str_ == "(");

	retVal = new descTree::descNode (lexema (OP_IF));

	retVal->children_.push_back ({ OP_EQ, GetR () });

	IC_SYNTAXSERT (S[P++].str_ == ")");

	while (S[P].str_ != "else")
	{
		retVal->children_.push_back ({ OP_IF, GetE () });
	}

	P++;

	while (S[P].str_ != "end")
	{
		retVal->children_.push_back ({ OP_NONE, GetE () });
	}

	P++;

	return retVal;
}

descTree::descNode * descenter::GetLOOP ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema (OP_LOOP));

	IC_SYNTAXSERT (S[P++].str_ == "loop");
	IC_SYNTAXSERT (S[P++].str_ == "(");

	retVal->children_.push_back ({ OP_NONE, GetE () });
	
	IC_SYNTAXSERT (S[P++].str_ == ")");

	while (S[P].str_ != "end")
		retVal->children_.push_back ({ OP_NONE, GetE () });

	P++;


	return retVal;
}



double descenter::nodeRunner (descTree::descNode * node)
{
	if (node->data.type_ == lexema::type::num)
	{
		return node->data.num_;
	}
	else if (node->data.op_ == OP_SUM)
	{
		double value = 0;

		for (auto && el : node->children_)
		{
			if (el.oper == OP_SUB)
				value -= nodeRunner (el.ptr);
			else
				value += nodeRunner (el.ptr);
		}

		return value;
	}
	else if (node->data.op_ == OP_MUL)
	{
		double value = 1;

		for (auto && el : node->children_)
		{
			if (el.oper == OP_DIV)
				value /= nodeRunner (el.ptr);
			else
				value *= nodeRunner (el.ptr);
		}

		return value;
	}
	else if (node->data.op_ == OP_POW)
	{
		if (node->children_.size () == 1)
		{
			return nodeRunner (node->children_[0].ptr);
		}
		else
		{
			return pow (nodeRunner (node->children_[0].ptr), nodeRunner (node->children_[1].ptr));
		}
	}
	else if (node->data.op_ == OP_EQ)
	{
		double value = nodeRunner (node->children_[1].ptr);
		ids_.insert_or_assign (node->children_[0].ptr->data.str_, value);

		return value;
	}
	else if (node->data.op_ == OP_COMP)
	{
		if (node->children_.size () == 1)
			return nodeRunner (node->children_[0].ptr);
		else
		{
			if (node->children_[1].oper == OP_GREATER)
				return nodeRunner (node->children_[0].ptr) > nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == OP_LESS)
				return nodeRunner (node->children_[0].ptr) < nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == OP_GOE)
				return nodeRunner (node->children_[0].ptr) >= nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == OP_LOE)
				return nodeRunner (node->children_[0].ptr) <= nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == OP_EQ)
				return nodeRunner (node->children_[0].ptr) == nodeRunner (node->children_[1].ptr);
		}
	}
	else if (node->data.op_ == OP_IF)
	{
		double retVal = 0;

		if (nodeRunner (node->children_[0].ptr))
		{
			for (auto && el : node->children_)
			{
				if (el.oper == OP_IF)
					retVal = nodeRunner (el.ptr);
			}
		}
		else
		{
			for (auto && el : node->children_)
			{
				if (el.oper == OP_NONE)
					retVal = nodeRunner (el.ptr);
			}
		}

		return retVal;
	}
	else if (node->data.op_ == OP_LOOP)
	{
		double retVal = 0;

		while (nodeRunner (node->children_[0].ptr))
		{
			for (auto && el : node->children_)
				retVal = nodeRunner (el.ptr);
		}

		return retVal;
	}
	else if (node->data.op_ == OP_ID)
	{
		assert (ids_.find (node->children_[0].ptr->data.str_) != ids_.end ());
		return ids_[node->children_[0].ptr->data.str_];
	}
	else
		return nodeRunner (node->children_[0].ptr);

}


