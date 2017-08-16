/*
 * StopWords.h
 *
 *  Created on: Jul 10, 2017
 *      Author: tim
 */

#ifndef STOPWORDS_H_
#define STOPWORDS_H_


#include <string>
#include <unordered_set>
#include <iostream>

class StopWords
{
public:
	StopWords();
	StopWords(std::istream & source);
	void add_word(const std::string & word);
	void add_word(std::string && word);
	bool is_stop_word(const std::string & word) const;
	void to_file(std::ostream & sink, bool alphabetical = false) const;
	void more_stopwords(std::istream & source);

private:
	std::unordered_set<std::string> words_;
};

#endif /* STOPWORDS_H_ */
