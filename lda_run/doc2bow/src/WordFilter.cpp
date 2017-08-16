/*
 * WordFilter.cpp
 *
 *  Created on: Jul 10, 2017
 *      Author: tim
 */

#include "WordFilter.h"
#include "stemming.h"
#include <cctype>
#include <algorithm>

WordFilter::WordFilter(std::istream & stop_word_source, bool filt_nums):
	stop_words_{stop_word_source}, filter_numbers_{filt_nums}
{

}
bool WordFilter::is_stop_word(const std::string & word) const
{
	return stop_words_.is_stop_word(word);
}

void WordFilter::more_stopwords(std::istream& source)
{
	stop_words_.more_stopwords(source);
}
void WordFilter::print_stopwords(bool alphabetical)
{
	stop_words_.to_file(std::cout, alphabetical);
}

void WordFilter::filter_word(std::string & str) const
{
	using str_iter_t = std::string::const_iterator;
	str_iter_t begin, end;
	std::tie(begin, end) = strip(str.cbegin(), str.cend());
	auto stripped_size = std::distance(begin, end);

	if(stripped_size < 3)
	{
		str.resize(0);
	}
	else
	{
		if(begin > str.cbegin())
		{
			std::copy(begin, end, str.begin());
		}
		str.resize(stripped_size);
		if(str.size())
		{
			std::transform(str.cbegin(), str.cend(), str.begin(), tolower);
			str.resize(str.size() * (not is_stop_word(str)));
			if(str.size())
			{
				// stem_word(str);
				str.resize(str.size() * (not is_stop_word(str)));
				str.resize(str.size() * (not (filter_numbers_ and std::any_of(str.cbegin(), str.cend(), isdigit))));
			}
		}

	}
}
