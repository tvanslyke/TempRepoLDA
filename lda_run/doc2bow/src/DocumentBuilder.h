/*
 * DocumentBuilder.h
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#ifndef DOCUMENTBUILDER_H_
#define DOCUMENTBUILDER_H_
#include "WordMap.h"
#include "Document.h"
#include "Corpus.h"
#include <map>
class DocumentBuilder
{
public:
	using id_t = WordMap::id_t;
	using count_t = size_t;
	using word_t = WordMap::word_t;
	using word_view_t = WordMap::word_view_t;

	Document documentize() const;
	void add_word(const word_t &, count_t qty = 1);
	void add_word(word_t &&, count_t qty = 1);
	void add_word(word_view_t, count_t qty = 1);
	DocumentBuilder & operator+=(const word_t & word);
	DocumentBuilder & operator+=(word_t && word);
	DocumentBuilder & operator+=(word_view_t word);
private:
	DocumentBuilder(WordMap & wm);
	WordMap& idmap_;
	std::map<id_t, count_t> word_counts_;
	friend class Corpus;
};

#endif /* DOCUMENTBUILDER_H_ */
