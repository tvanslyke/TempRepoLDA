/*
 * Document.h
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "WordMap.h"
#include "ioutils.h"
class Document {
public:
	using id_t = WordMap::id_t;
	using count_t = size_t;
	using count_map_t = std::unordered_map<const Document*, size_t>;
	Document(const std::vector<id_t>& words, const std::vector<count_t> & counts);
	Document(std::vector<id_t>&& words, const std::vector<count_t> & counts);
	Document(const std::vector<id_t>& words, std::vector<count_t> && counts);
	Document(std::vector<id_t>&& words, std::vector<count_t> && counts);
	~Document();
	bool has_word(id_t word) const;
	bool word_count(id_t word) const;
	count_t total_words() const;
	count_t total_unique_words() const;
	void to_file(std::ostream &) const;
	static Document from_file(std::istream & source, size_t word_count, size_t bytes_per_id = sizeof(id_t), size_t bytes_per_count = sizeof(count_t));
private:
	ptrdiff_t word_index(id_t word) const;
	const std::vector<id_t> words_;
	const std::vector<count_t> counts_;
	// non-member to keep cache footprint small.  Don't expect this to be used frequently.
	static std::unordered_map<const Document*, size_t> word_totals_;
};

#endif /* DOCUMENT_H_ */
