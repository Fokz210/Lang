#include "lexema.h"


lexema::lexema () :
	num_ (),
	str_ (),
	type_ (none),
	position_ (0),
	op_ (OP_NONE)
{
}

lexema::lexema (double num) :
	num_ (num),
	str_ (),
	type_ (type::num),
	position_ (0),
	op_(OP_NONE)
{
}

lexema::lexema (std::string str, lexema::type type) :
	num_ (),
	str_ (str),
	type_ (type),
	position_ (0),
	op_(OP_NONE)
{

}

lexema::lexema(op_t op) :
	num_ (),
	str_ (),
	type_ (type::op),
	op_ (op)
{
}

char lexema::ch()
{
	return str_[0];
}