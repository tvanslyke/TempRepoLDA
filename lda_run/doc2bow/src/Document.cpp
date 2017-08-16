/*
 * Document.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#include "Document.h"
#include <numeric>
#include <functional>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "ioutils.h"

std::unordered_map<const Document*, Document::count_t> Document::word_totals_{};


Document::Document(const std::vector<id_t>& words, const std::vector<count_t> & counts):
		words_{words}, counts_{counts}
{

}

Document::Document(std::vector<id_t>&& words, const std::vector<count_t> & counts):
				words_{std::move(words)}, counts_{counts}
{

}

Document::Document(const std::vector<id_t>& words, std::vector<count_t> && counts):
				words_{words}, counts_{std::move(counts)}
{

}

Document::Document(std::vector<id_t>&& words, std::vector<count_t> && counts):
				words_{std::move(words)}, counts_{std::move(counts)}
{

}

Document::~Document()
{
	auto iter = word_totals_.find(this);
	if(iter != word_totals_.end())
	{
		word_totals_.erase(iter);
	}
}

bool Document::has_word(id_t word) const
{
	return std::binary_search(words_.cbegin(), words_.cend(), word);
}

bool Document::word_count(id_t word) const
{
	auto index = word_index(word);
	return (index == -1) ? 0 : counts_[index];
}

ptrdiff_t Document::word_index(id_t word) const
{
	auto lb = std::lower_bound(words_.cbegin(), words_.cend(), word);
	bool foundit = (lb < words_.end()) and (*lb == word);

	// Branchless.  Returns -1 if 'word' isn't in 'words_'.  otherwise does the obvious thing
	return -1 + foundit * (lb - words_.cbegin() + 1);
}

Document::count_t Document::total_unique_words() const
{
	return words_.size();
}

Document::count_t Document::total_words() const
{
	count_t total{0};
	auto iter = word_totals_.find(this);
	if(iter == word_totals_.end())
	{
		total = std::accumulate(counts_.cbegin(), counts_.cend(), count_t(0));
		word_totals_.emplace(this, total);
	}
	else
	{
		total = iter->second;
	}
	return total;
}

void Document::to_file(std::ostream & f) const
{
	const char * id_as_bytes;
	const char * count_as_bytes;
	if(is_big_endian())
	{
		for(size_t i = 0; i < words_.size(); ++i)
		{
			write_as_bytes_reverse(f, words_[i]);
			write_as_bytes_reverse(f, counts_[i]);
		}
	}
	else
	{
		for(size_t i = 0; i < words_.size(); ++i)
		{
			write_as_bytes(f, words_[i]);
			write_as_bytes(f, counts_[i]);
		}
	}
}

Document Document::from_file(std::istream & source, size_t word_count, size_t bytes_per_id, size_t bytes_per_count)
{
	/* TODO: make these exceptions */
	assert(bytes_per_id <= sizeof(id_t));
	assert(bytes_per_count <= sizeof(count_t));
	std::vector<id_t> words;
	std::vector<count_t> counts;
	words.resize(word_count);
	counts.resize(word_count);
	if(is_big_endian())
	{
		for(size_t i = 0; i < word_count; ++i)
		{
			read_as_bytes_reverse(source, words[i], bytes_per_id);
			read_as_bytes_reverse(source, counts[i], bytes_per_count);
		}
	}
	else
	{
		for(size_t i = 0; i < word_count; ++i)
		{
			read_as_bytes(source, words[i], bytes_per_id);
			read_as_bytes(source, counts[i], bytes_per_count);
		}
	}
	return Document(std::move(words), std::move(counts));
}

