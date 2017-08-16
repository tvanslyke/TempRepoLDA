/*
 * DocumentParser.h
 *
 *  Created on: Jul 11, 2017
 *      Author: tim
 */

#ifndef DOCUMENTPARSER_H_
#define DOCUMENTPARSER_H_

#include "Corpus.h"
#include "WordFilter.h"
#include "DocumentBuilder.h"
#include <iostream>
#include <fstream>
template <class F>
class DocParseIter;
template <class F = void>
class DocumentParser {
public:
	DocumentParser(F & pred = [](const std::string & ){return true;}):
		filter_{}, pred_{pred}
	{

	}
	DocumentParser(std::istream & stopword_source, F & pred = [](const std::string & ){return true;}):
		filter_{stopword_source}, pred_{pred}
	{

	}
	void more_stopwords(std::istream& source)
	{
		filter_.more_stopwords(source);
	}
	void print_stopwords(bool alphabetical = true)
	{
		filter_.print_stopwords(alphabetical);
	}
	void parse_doc(Corpus & corpus, std::istream & docsrc)
	{
		auto db = corpus.getDocumentBuilder();
		std::string word;
		while(docsrc)
		{
			docsrc >> word;
			filter_.filter_word(word, pred_);
			if(word.size()) db += word;
		}
	}
	DocParseIter<F> as_iterator(Corpus & corp)
	{
		return DocParseIter<F>(*this, corp);
	}
private:
	WordFilter filter_;
	F & pred_;
};

template<>
class DocumentParser<void> {
public:
	DocumentParser() = default;
	DocumentParser(std::istream & stopword_source);
	void parse_doc(Corpus & corpus, std::istream & docsrc);
	void more_stopwords(std::istream& source);
	void print_stopwords(bool alphabetical = true);
	DocParseIter<void> as_iterator(Corpus & corp);
private:
	WordFilter filter_;
};

template <class F = void>
class DocParseIter
{
private:
	class RefType
	{
	public:
		RefType(DocumentParser<F> & dp, Corpus & c):
			parser_{dp}, corpus_{c}
		{

		}
		RefType & operator=(const std::string & str)
		{
			std::ifstream doc(str);
			/* TODO: Remove after debug. */
			std::cout << "Parsing: " << str << std::endl;
			parser_.parse_doc(corpus_, doc);
			return *this;
		}
	private:
		DocumentParser<F> & parser_;
		Corpus & corpus_;
	};
public:
	DocParseIter(DocumentParser<F> & dp, Corpus & c):
		ref_{dp, c}
	{

	}
	DocParseIter & operator++()
	{
		return *this;
	}
	DocParseIter & operator++(int)
	{
		return *this;
	}
	RefType operator*()
	{
		return ref_;
	}
	RefType ref_;
};

#endif /* DOCUMENTPARSER_H_ */
