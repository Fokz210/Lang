#pragma once
#include <functional>
#include "pair.h"
#include "Vector.h"
#include "list.h"

namespace icl
{
	template <class Key, class T, class Hash = std::hash <Key>>
	class unordered_map
	{
	public: 
		typedef icl::pair <const Key, T>																				        value_type;
		typedef typename icl::pair <typename icl::vector <icl::list <value_type>>:: iter, typename icl::list<value_type>::iter> address_t;

		unordered_map (size_t max_size);
		void insert (const value_type & value);
		
		class iterator
		{
		public:
			iterator (address_t address) :
				address_ (address)
			{
			}

			bool operator != (const iterator& data) const
			{
				return data.address_.first != address_.first || data.address_.second != address_.second;
			}

			value_type& operator * ()
			{
				return *address_.second;
			}

			iterator& operator ++ ()
			{
				if ((++address_.second).ptr_ == nullptr)
				{
					while (true)
					{
						if (++address_.first == address_.first.container_->end ())
						{
							return *this;
						}

						if ((*address_.first).size_ != 0)
							break;
					}
					address_.second = (*address_.first).begin ();

					return* this;
				}

				return *this;
			}

			address_t address_;
		};

		iterator find (Key key)
		{
			Hash hash;
			size_t hash_val = hash (key) % max_size_;

			for (auto && i = data_[hash_val].begin(); i != data_[hash_val].end(); ++i)
			{
				if ((*i).first == key)
				{
					return begin ();
				}
			}

			return end ();

		}

		T& operator [] (Key key)
		{
			Hash hash;
			size_t hash_val = hash (key) % max_size_;

			for (auto&& el : data_[hash_val])
			{
				if (el.first == key)
					return el.second;
			}

			assert (!"HASH_OUT_OF_RANGE");
		}

		iterator begin ()
		{
			return iterator ({data_.begin(), data_[0].begin()});
		}

		iterator end ()
		{
			return iterator ({ data_.end (), data_[0].end () });
		}

	protected:
		icl::vector <icl::list <value_type>> data_;
		size_t size_, max_size_;
	};

	template <class Key, class T, class Hash>
	unordered_map <Key, T, Hash>::unordered_map (size_t max_size) :
		data_ (max_size),
		size_ (0),
		max_size_ (max_size)
	{

	}

	template<class Key, class T, class Hash>
	inline void unordered_map<Key, T, Hash>::insert (const value_type & value)
	{
		Hash hash;

		size_t hash_val = hash (value.first) % max_size_;

		data_[hash_val].push_back (value);

		size_++;
	}
}


