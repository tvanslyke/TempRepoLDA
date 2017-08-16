/*
 * WordBag.h
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#ifndef WORDMAP_H_
#define WORDMAP_H_


#include <string>
#include <unordered_map>
#include <experimental/string_view>
#include <vector>
#include <cstddef>
class WordMap
{
public:
	using id_t = ptrdiff_t;
	using word_t = std::string;
	using word_view_t = std::experimental::string_view;
	WordMap();
	word_view_t operator[](id_t id) const;
	id_t operator[](word_view_t word);
	id_t operator[](const word_t & word);
	id_t operator[](word_t && word);
	bool has_word(const word_t & word) const;
	size_t size() const;
	size_t count() const;
	id_t add_word(const word_t & word);
	id_t add_word(word_t && word);
	void reserve(size_t count);
	void to_file(std::ostream &) const;
	void from_file(std::istream &);
	void from_file(std::istream &, size_t reserve);
private:
	id_t get_count(id_t) const;
	id_t clip_id(id_t) const;;
	id_t id_count_;
	std::vector<word_view_t> bag_;
	std::unordered_map<word_t, id_t> word2id_;
};




#endif /* WORDMAP_H_ */
