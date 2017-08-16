/*
 * DocumentParser.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: tim
 */


#include "DocumentParser.h"
#include <algorithm>
#include <iterator>
#include <boost/tokenizer.hpp>

DocumentParser<void>::DocumentParser(std::istream & stopword_source):
	filter_{stopword_source}
{

}
void DocumentParser<void>::parse_doc(Corpus & corpus, std::istream & docsrc)
{
	using tokenizer_t = boost::tokenizer<boost::char_separator<char>, std::istreambuf_iterator<char>, std::string>;
	boost::char_separator<char> seps(" .,\\/:;`\"[]{}()|~*+_&^#@!?-'\n\t");
	auto tokenizer = tokenizer_t(std::istreambuf_iterator<char>(docsrc), std::istreambuf_iterator<char>(), seps);
	auto db = corpus.getDocumentBuilder();

	for(auto word:tokenizer)
	{
		filter_.filter_word(word);
		if(word.size()) db += word;
	}
	corpus.finalizeDocument(db);
}

void DocumentParser<void>::more_stopwords(std::istream& source)
{
	filter_.more_stopwords(source);
}
void DocumentParser<void>::print_stopwords(bool alphabetical)
{
	filter_.print_stopwords(alphabetical);
}
DocParseIter<void> DocumentParser<void>::as_iterator(Corpus & corp)
{
	return DocParseIter<void>(*this, corp);
}

