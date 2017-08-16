/*
 * WordMap.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#include "WordMap.h"
#include <cassert>
#include <iostream>

WordMap::WordMap():
	id_count_{0},
	bag_{},
	word2id_{}
{


}

bool WordMap::has_word(const word_t & word) const
{
	return word2id_.count(word);
}

WordMap::word_view_t WordMap::operator[](id_t id) const
{
	return bag_[id];
}

WordMap::id_t WordMap::operator[](word_view_t word)
{
	return add_word(std::string(word));
}

WordMap::id_t WordMap::operator[](const word_t & word)
{
	return add_word(word);
}

WordMap::id_t WordMap::operator[](word_t && word)
{
	return add_word(std::move(word));
}


size_t WordMap::size() const
{
	return bag_.size();
}

void WordMap::reserve(size_t qty)
{
	bag_.reserve(qty);
	word2id_.reserve(qty);
}


WordMap::id_t WordMap::add_word(word_t && word)
{
	auto iter = word2id_.find(word);
	if(iter == word2id_.end())
	{
		auto result = word2id_.emplace(word, id_count_);
		bag_.emplace_back(result.first->first.c_str(), result.first->first.size());
		++id_count_;
		return result.first->second;
	}
	return iter->second;
}

WordMap::id_t WordMap::add_word(const word_t & word)
{
	auto iter = word2id_.find(word);
	if(iter == word2id_.end())
	{
		auto result = word2id_.emplace(word, id_count_);
		bag_.emplace_back(result.first->first.c_str(), result.first->first.size());
		++id_count_;
		return result.first->second;
	}
	return iter->second;
}

void WordMap::to_file(std::ostream & strm) const
{
	for(const word_view_t & word:bag_)
	{
		strm.write(word.data(), word.size());
		strm.put('\n');
	}
	strm << std::flush;
}
void WordMap::from_file(std::istream & source)
{
	if(source)
	{
		decltype(word2id_)::const_iterator iter;
		bool inserted;
		std::string word;
		while(std::getline(source, word))
		{
			std::tie(iter, inserted) = word2id_.emplace(word, id_count_);
			bag_.emplace_back(iter->first.c_str(), iter->first.size());
			// TODO: remove assert
			assert(inserted);
			++id_count_;
		}
	}
}

void WordMap::from_file(std::istream & source, size_t reserve)
{
	word2id_.reserve(reserve);
	bag_.reserve(reserve);
	from_file(source);
}





