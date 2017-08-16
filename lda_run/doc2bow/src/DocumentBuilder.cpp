/*
 * DocumentBuilder.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#include "DocumentBuilder.h"

DocumentBuilder::DocumentBuilder(WordMap & wm):
	idmap_{wm}
{

}


Document DocumentBuilder::documentize() const
{
	std::vector<id_t> words;
	std::vector<count_t> counts;
	words.reserve(word_counts_.size());
	counts.reserve(word_counts_.size());
	for(const auto & kvp:word_counts_)
	{
		words.push_back(kvp.first);
		counts.push_back(kvp.second);
	}

	return Document(words, counts);
}
void DocumentBuilder::add_word(const DocumentBuilder::word_t & word, DocumentBuilder::count_t qty)
{
	word_counts_[idmap_[word]] += qty;
}
void DocumentBuilder::add_word(DocumentBuilder::word_t && word, DocumentBuilder::count_t qty)
{
	word_counts_[idmap_[std::move(word)]] += qty;
}
void DocumentBuilder::add_word(DocumentBuilder::word_view_t word, DocumentBuilder::count_t qty)
{
	word_counts_[idmap_[word]] += qty;
}

DocumentBuilder & DocumentBuilder::operator+=(const word_t & word)
{
	++(word_counts_[idmap_[word]]);
	return *this;
}
DocumentBuilder & DocumentBuilder::operator+=(word_t && word)
{
	++(word_counts_[idmap_[std::move(word)]]);
	return *this;
}
DocumentBuilder & DocumentBuilder::operator+=(word_view_t word)
{
	++(word_counts_[idmap_[word]]);
	return *this;
}
