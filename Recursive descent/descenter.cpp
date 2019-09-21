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

			printf_s ("|  %d\n", nodeRunner (tree_.head_));

			delete[] data;

			tree_.swap ();
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
		curr.str_ = std::string ();
		curr.num_ = 0;
		curr.type_ = lexema::type::none;
		curr.position_ = P;

		if ('0' <= input[P] && input[P] <= '9')
		{
			while ('0' <= input[P] && input[P] <= '9')
			{
				curr.type_ = lexema::type::num;
				curr.num_ = curr.num_ * 10 + input[P] - '0';
				P++;
			}

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
	descTree::descNode * retVal = new descTree::descNode (lexema ("+", lexema::type::op));

	retVal->children_.push_back ({ "", GetT () });

	while (S[P].ch () == '+' || S[P].ch () == '-')
	{
		char op = S[P].ch ();
		P++;

		if (op == '-')
			retVal->children_.push_back ({ "-", GetT () });
		else
			retVal->children_.push_back ({ "+", GetT () });
	}

	if (S[P].ch () == ';')
		P++;

	return retVal;
}

descTree::descNode * descenter::GetT ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema ("*", lexema::type::op));
	retVal->children_.push_back ({ "", GetS () });

	while (S[P].ch () == '*' || S[P].ch () == '/')
	{
		char op = S[P].ch ();
		P++;

		if (op == '*')
			retVal->children_.push_back ({ "*", GetS () });
		else
			retVal->children_.push_back ({ "/", GetS () });
	}

	return retVal;
}

descTree::descNode * descenter::GetS ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema ("^", lexema::type::op));
	retVal->children_.push_back ({ "", GetV () });

	if (S[P].ch () == '^')
	{
		P++;

		retVal->children_.push_back ({ "", GetV () });
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
	descTree::descNode * retVal = new descTree::descNode (lexema ("ID", lexema::type::op));

	retVal->children_.push_back ({ "", new descTree::descNode (lexema (S[P].str_, lexema::type::str)) });

	P++;

	return retVal;
}

descTree::descNode * descenter::GetR ()
{
	descTree::descNode * retVal = nullptr;

	if (P < S.size() - 1 && S[P + 1].str_ == "=")
	{
		retVal = new descTree::descNode (lexema ("=", lexema::type::op));

		std::string ID = S[P].str_;

		retVal->children_.push_back ({ "", new descTree::descNode (lexema (ID, lexema::type::str)) });

		P += 2;

		retVal->children_.push_back ({ "", GetE () });
	}
	else
	{
		retVal = GetID ();
	}

	return retVal;
}

descTree::descNode * descenter::GetV ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema ("comp", lexema::type::op));

	retVal->children_.push_back ({ "", GetP () });

	if (S[P].ch () == '>' || S[P].ch () == '<' || S[P].str_ == "=>" || S[P].str_ == "<=" || S[P].str_ == "==")
	{
		std::string oper = S[P].str_;
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

	retVal = new descTree::descNode (lexema ("if", lexema::type::op));

	retVal->children_.push_back ({ "", GetR () });

	IC_SYNTAXSERT (S[P++].str_ == ")");

	while (S[P].str_ != "else")
	{
		retVal->children_.push_back ({ "true", GetE () });
	}

	P++;

	while (S[P].str_ != "end")
	{
		retVal->children_.push_back ({ "false", GetE () });
	}

	P++;

	return retVal;
}

descTree::descNode * descenter::GetLOOP ()
{
	descTree::descNode * retVal = new descTree::descNode (lexema ("loop", lexema::type::op));

	IC_SYNTAXSERT (S[P++].str_ == "loop");
	IC_SYNTAXSERT (S[P++].str_ == "(");

	retVal->children_.push_back ({ "statement", GetE () });
	
	IC_SYNTAXSERT (S[P++].str_ == ")");

	while (S[P].str_ != "end")
		retVal->children_.push_back ({ "body", GetE () });

	P++;


	return retVal;
}



int descenter::nodeRunner (descTree::descNode * node)
{
	if (node->data.type_ == lexema::type::num)
	{
		return node->data.num_;
	}
	else if (node->data.str_ == "+")
	{
		int value = 0;

		for (auto && el : node->children_)
		{
			if (el.oper == "-")
				value -= nodeRunner (el.ptr);
			else
				value += nodeRunner (el.ptr);
		}

		return value;
	}
	else if (node->data.str_ == "*")
	{
		int value = 1;

		for (auto && el : node->children_)
		{
			if (el.oper == "/")
				value /= nodeRunner (el.ptr);
			else
				value *= nodeRunner (el.ptr);
		}

		return value;
	}
	else if (node->data.str_ == "^")
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
	else if (node->data.str_ == "=")
	{
		int value = nodeRunner (node->children_[1].ptr);
		ids_.insert_or_assign (node->children_[0].ptr->data.str_, value);

		return value;
	}
	else if (node->data.str_ == "comp")
	{
		if (node->children_.size () == 1)
			return nodeRunner (node->children_[0].ptr);
		else
		{
			if (node->children_[1].oper == ">")
				return nodeRunner (node->children_[0].ptr) > nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == "<")
				return nodeRunner (node->children_[0].ptr) < nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == ">=")
				return nodeRunner (node->children_[0].ptr) >= nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == "<=")
				return nodeRunner (node->children_[0].ptr) <= nodeRunner (node->children_[1].ptr);
			else if (node->children_[1].oper == "==")
				return nodeRunner (node->children_[0].ptr) == nodeRunner (node->children_[1].ptr);
		}
	}
	else if (node->data.str_ == "if")
	{
		int retVal = 0;

		if (nodeRunner (node->children_[0].ptr))
		{
			for (auto && el : node->children_)
			{
				if (el.oper == "true")
					retVal = nodeRunner (el.ptr);
			}
		}
		else
		{
			for (auto && el : node->children_)
			{
				if (el.oper == "false")
					retVal = nodeRunner (el.ptr);
			}
		}

		return retVal;
	}
	else if (node->data.str_ == "loop")
	{
		int retVal = 0;

		while (nodeRunner (node->children_[0].ptr))
		{
			for (auto && el : node->children_)
				retVal = nodeRunner (el.ptr);
		}

		return retVal;
	}
	else if (node->data.str_ == "ID")
	{
		assert (ids_.find (node->children_[0].ptr->data.str_) != ids_.end ());
		return ids_[node->children_[0].ptr->data.str_];
	}
	else
		return nodeRunner (node->children_[0].ptr);

}


