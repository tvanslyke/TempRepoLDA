/*
 * Corpus.h
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#ifndef CORPUS_H_
#define CORPUS_H_
#include "WordMap.h"
#include "Document.h"
#include "WordFilter.h"
#include <experimental/filesystem>
class DocumentBuilder;

class Corpus
{
public:
	using id_t = Document::id_t;
	using count_t = Document::count_t;
	void reserve(size_t doc_count);
	void add_doc() const;
	DocumentBuilder getDocumentBuilder();
	void finalizeDocument(const DocumentBuilder & doc);
	void load_dictionary(std::istream & source);
	void save_dictionary(std::ostream & sink) const;
	void load_documents(std::istream & source);
	void save_documents(std::ostream & sink) const;

private:
	WordMap words_;
	std::vector<Document> docs_;
};

#endif /* CORPUS_H_ */
