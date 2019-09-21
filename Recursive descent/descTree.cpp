#include "descTree.h"

descTree::descTree () :
	head_ (nullptr)
{
}

descTree::~descTree ()
{
	if (!head_)
		return;

	head_->~descNode ();
	delete head_;
}

void descTree::swap (descTree & other)
{
	std::swap (other.head_, head_);
}

descTree::descNode::descNode (lexema data) :
	data (data),
	children_ ()
{
}

descTree::descNode::~descNode ()
{
	for (auto&& el : children_)
	{
		el.ptr->~descNode ();
		delete el.ptr;
	}
}
