#include "lexema.h"


lexema::lexema () :
	num_ (),
	str_ (),
	type_ (none),
	position_ (0)
{
}

lexema::lexema (int num) :
	num_ (num),
	str_ (),
	type_ (type::num),
	position_ (0)
{
}

lexema::lexema (std::string str, lexema::type type) :
	num_ (),
	str_ (str),
	type_ (type),
	position_ (0)
{

}

char lexema::ch ()
{
	return str_[0];
}