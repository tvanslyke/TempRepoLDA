/*
 * WordFilter.h
 *
 *  Created on: Jul 10, 2017
 *      Author: tim
 */

#ifndef WORDFILTER_H_
#define WORDFILTER_H_


#include "StopWords.h"
#include <cctype>
class WordFilter {
public:
	WordFilter() = default;
	WordFilter(std::istream & stop_word_source, bool filtnums = true);
	bool is_stop_word(const std::string & word) const;
	void filter_word(std::string & str) const;
	template <class F>
	void filter_word(std::string & str, F & pred) const
	{
		filter_word(str);
		str.resize(pred((const std::string &)(pred)) * str.size());
	}
	template <class It>
	static std::pair<It, It> strip(It begin, It end)
	{
		while(begin < end and not std::isalpha(*begin))
		{
			++begin;
		}
		--end;
		while(begin < end and not std::isalpha(*end))
		{
			++begin;
		}
		++end;
		return {begin, end};
	}
	void more_stopwords(std::istream& source);
	void print_stopwords(bool alphabetical = true);
private:
	StopWords stop_words_;
	const bool filter_numbers_ = true;
};

#endif /* WORDFILTER_H_ */
