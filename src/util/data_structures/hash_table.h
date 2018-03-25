/****
DIAMOND protein aligner
Copyright (C) 2013-2018 Benjamin Buchfink <buchfink@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
****/

#ifndef HASH_TABLE2_H_
#define HASH_TABLE2_H_

#include <stdexcept>

template<typename _K, typename _V, typename _HashFunction>
struct HashTable : private _HashFunction
{

	struct Entry : public _V
	{
		_K key;
		bool blank() const
		{
			return _V::operator unsigned() == 0;
		}
	};

	HashTable(size_t size, const _HashFunction &hash) :
		_HashFunction(hash),
		table(new Entry[size]),
		size_(size),
		probe_l(0),
		probe_n(0)
	{
		memset(table, 0, size_ * sizeof(Entry));
	}

	~HashTable()
	{
		delete[] table;
	}

	_V& operator[](_K key)
	{
		Entry *e = get_entry(key);
		if (e->blank())
			e->key = key;
		return *e;
	}

	_V* find(_K key)
	{
		return get_present_entry(key);
	}

	Entry* find_entry(_K key)
	{
		return get_present_entry(key);
	}

	Entry* insert(_K key)
	{
		return get_or_insert_entry(key);
	}

	size_t size() const
	{
		return size_;
	}

	size_t count() const
	{
		size_t n = 0;
		for (size_t i = 0; i < size_; ++i)
			if (!table[i].blank())
				++n;
		return n;
	}

	Entry* data()
	{
		return table;
	}

	size_t probe_l, probe_n;

private:

	Entry* get_entry(_K key, bool stat=false)
	{
		//Entry *p = &table[_H()(key) % size_];
		Entry *p = &table[_HashFunction::operator()(key)];
		bool wrapped = false;
		//if(stat) ++probe_n;
		while (p->key != key && !p->blank()) {
			//if(stat) ++probe_l;
			++p;
			if (p == &table[size_]) {
				if (wrapped)
					throw std::runtime_error("Hash table overflow.");
				p = &table[0];
				wrapped = true;
			}
		}
		return p;
	}

	Entry* get_present_entry(_K key, bool stat = false)
	{
		Entry *p = &table[_HashFunction::operator()(key)];
		bool wrapped = false;
		//if(stat) ++probe_n;
		while(true) {
			//if(stat) ++probe_l;
			if (p->key == key)
				return p;
			if (p->blank())
				return NULL;
			++p;
			if (p == &table[size_]) {
				if (wrapped)
					throw std::runtime_error("Hash table overflow.");
				p = &table[0];
				wrapped = true;
			}
		}
		return p;
	}

	Entry* get_or_insert_entry(_K key, bool stat = false)
	{
		Entry *p = &table[_HashFunction::operator()(key)];
		bool wrapped = false;
		//if(stat) ++probe_n;
		while (true) {
			//if(stat) ++probe_l;
			if (p->key == key)
				return p;
			if (p->blank()) {
				p->key = key;
				return p;
			}
			++p;
			if (p == &table[size_]) {
				if (wrapped)
					throw std::runtime_error("Hash table overflow.");
				p = &table[0];
				wrapped = true;
			}
		}
		return p;
	}

	Entry *table;
	size_t size_;

};

#endif