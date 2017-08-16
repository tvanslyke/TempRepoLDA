/*
 * Corpus.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#include "Corpus.h"
#include "DocumentBuilder.h"
#include <climits>
#include <fstream>
#include <type_traits>
#include <cstdint>
#include <set>
#include <cassert>
#include <tuple>


DocumentBuilder Corpus::getDocumentBuilder()
{
	return DocumentBuilder(words_);
}

void Corpus::finalizeDocument(const DocumentBuilder & doc)
{
	docs_.push_back(doc.documentize());
}
void Corpus::load_dictionary(std::istream & source)
{
	words_.from_file(source);
}
void Corpus::save_dictionary(std::ostream & sink) const
{
	words_.to_file(sink);
}
void Corpus::load_documents(std::istream & source)
{
	// # of documents to read
	size_t doc_count{0};
	// placeholder for metadata as we parse the file
	std::string temp;
	std::getline(source, temp, ':');
	// information about metadata of the form 'i32', 'u64', 'f128', etc
	bool isint, issigned;
	size_t bits;
	std::tie(isint, issigned, bits) = string_to_type(temp);
	if(not isint)
	{
		throw std::runtime_error("[Corpus::load_documents] Failure due to unrecognized file format.");
	}
	else if(bits % CHAR_BIT != 0)
	{
		throw std::runtime_error("[Corpus::load_documents] Where did you get this file from? \n\t"
								 "Needs " + std::to_string(bits) + " bit integers, which are weird and gross."
								 " (this system uses " + std::to_string(CHAR_BIT) + " bit bytes)");
	}
	else
	{
		std::getline(source, temp);
		doc_count = std::stoull(temp);
		std::vector<size_t> counts(doc_count, 0);
		size_t bytes_to_read = bits / CHAR_BIT;

		if(bytes_to_read > sizeof(size_t))
		{
			if(bytes_to_read > sizeof(unsigned long long))
			{
				throw std::runtime_error("[Corpus::load_documents()] Fields in source file are too wide for this system!");
			}
			unsigned long long tempv;

			/* Endianness crap */
			if(is_big_endian())
			{
				for(auto & count:counts)
				{
					read_as_bytes_reverse(source, tempv, bytes_to_read);
					count = tempv;
					assert(count == tempv);
				}
			}
			else
			{
				for(auto & count:counts)
				{
					read_as_bytes(source, tempv, bytes_to_read);
					count = tempv;
					assert(count == tempv);
				}
			}
		}
		else
		{
			/* Endianness crap */
			if(is_big_endian())
			{
				for(auto & count:counts)
				{
					read_as_bytes_reverse(source, count, bytes_to_read);
				}
			}
			else
			{
				for(auto & count:counts)
				{
					read_as_bytes(source, count, bytes_to_read);
				}
			}
		}
		// read type of ID fields
		temp.assign(1, source.get());
		char c = source.get();
		while(std::isdigit(c))
		{
			temp.push_back(c);
			c = source.get();
		}
		std::tie(isint, issigned, bits) = string_to_type(temp);
		// ensure it makes sense
		assert(isint and issigned and (bits % CHAR_BIT) == 0);
		auto idtype_byte_count = bits / CHAR_BIT;
		// read type of count fields
		temp.assign(1, c);
		c = source.get();
		while(std::isdigit(c))
		{
			temp.push_back(c);
			c = source.get();
		}
		std::tie(isint, issigned, bits) = string_to_type(temp);
		// ensure it makes sense
		assert(isint and (not issigned) and (bits % CHAR_BIT) == 0);
		auto counttype_byte_count = bits / CHAR_BIT;
		getline(source, temp);
		// read the field that's supposed to tell us how many ID - Count pairs there are.
		// it should actually hold -1 (unknown/unbounded) since that information can be calculated from the info in the header.
		// namely, it would be the result of std::accumulate(counts.begin(), counts.end()).
		assert(temp == "-1");
		docs_.reserve(docs_.size() + doc_count);
		// finally read the documents
		for(auto count:counts)
		{
			docs_.push_back(Document::from_file(source, count, idtype_byte_count, counttype_byte_count));
		}
	}
}
void Corpus::save_documents(std::ostream & sink) const
{
	const std::string sizet_str = type_to_string<size_t>();
	const std::string idt_str = type_to_string<id_t>();
	const std::string countt_str = type_to_string<count_t>();
	sink << sizet_str << ':' << docs_.size() << '\n';
	if(is_big_endian())
	{
		for(const auto & doc:docs_)
		{
			write_as_bytes_reverse(sink, size_t(doc.total_unique_words()));
		}
	}
	else
	{
		for(const auto & doc:docs_)
		{
			write_as_bytes(sink, size_t(doc.total_unique_words()));
		}

	}
	sink << idt_str << countt_str << ':' << "-1\n";
	for(const auto & doc:docs_)
	{
		doc.to_file(sink);
	}

}
