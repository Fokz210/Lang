#pragma once
#include <cassert>
#include <iostream>

namespace icl
{
	template <class T>
	class list
	{
	public:
		class node
		{
		public:
			node (T data, node* prev = nullptr, node* next = nullptr);

			node* next_, * prev_;
			T data_;

		};

		list ();
		void push_back (const T & data);
		void push_front (const T & data);
		T pop_back ();
		T pop_front ();

		class iter
		{
		public:
			iter (node* ptr): 
				ptr_ (ptr)
			{
			}

			bool operator != (const iter& left) const 
			{
				return left.ptr_ != this->ptr_;
			}

			bool operator == (iter& left)
			{
				return left.ptr_ == ptr_;
			}

			iter& operator ++ ()
			{
				ptr_ = ptr_->next_;

				return *this;
			}

			T& operator * ()
			{
				assert (ptr_);

				return ptr_->data_;
			}

			node* ptr_;
		};

		iter begin ()
		{
			return iter (start_);
		}

	    iter end ()
		{
			return iter (nullptr);
		}

		node* start_, * end_;
		size_t size_;
	};

	template <class T>
	inline list<T>::list () :
		start_ (nullptr), end_ (nullptr),
		size_ (0)
	{

	}

	template <class T>
	inline list<T>::node::node (T data, node* prev, node* next) :
		data_ (data),
		prev_ (prev),
		next_ (next)
	{
	}
	
	template <class T>
	inline void list<T>::push_front (const T & data)
	{
		if (size_ == 0)
		{
			push_back (data);
		}
		else
		{
			node* newNode = new node (data, nullptr, start_);
			start_->prev_ = newNode;
			start_ = newNode;

			size_++;
		}
	}

	template <class T>
	inline void list<T>::push_back (const T & data)
	{
		if (size_ == 0)
		{
			node* newNode = new node (data);
			start_ = end_ = newNode;

			size_++;
		}
		else
		{
			node* newNode = new node (data, end_);
			end_->next_ = newNode;
			end_ = newNode;

			size_++;
		}
	}

	template <class T>
	inline T list<T>::pop_back ()
	{
		assert (size_);
		T buffer = T ();

		if (size_ == 1)
		{
			return pop_front ();
		}
		else 
		{
			buffer = end_->data_;

			end_ = end_->prev_;
			delete end_->next_;
			end_->next_ = nullptr;

			size_--;
		}
	}

	template <class T>
	inline T list<T>::pop_front ()
	{
		assert (size_);
		T buffer = T ();

		if (size_ == 1)
		{
			buffer = start_->data_;
			delete start_;
			start_ = end_ = nullptr;

			size_--;
		}
		else 
		{
			buffer = start_->data_;

			start_ = start_->next_;
			delete start_->prev_;
			start_->prev_ = nullptr;

			size_--;
		}

		return buffer;
	}


	template <class T>
	std::ostream& operator << (std::ostream& out, list<T>& data)
	{
		for (auto&& el : data)
		{
			out << el << " ";
		}

		return out;
	}
}
