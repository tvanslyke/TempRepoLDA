/*
 * Main.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */


#include "Corpus.h"
#include "filesystem_utils.h"
#include <iostream>
#include <fstream>
#include "ArgParser.h"
#include "DocumentParser.h"
#include <cassert>
#include <iterator>

void printhelp(bool verbose_help = false)
{
	std::clog <<  "Usage: doc2bow [OPTION=VALUE] ..." << '\n';
	std::clog <<  "Recursively navigate a directory and convert each plain-text document to a      " << '\n';
	std::clog <<  "'bag-of-words'.  When used in this manner, produces 2 files, a dictionary       " << '\n';
	std::clog <<  "listing each (not-ignored/filtered) word found on it's own line in plain-text   " << '\n';
	std::clog <<  "and a 'word-bag' file with a small header.                                      " << "\n\n";
	if(verbose_help)
	{
		std::clog <<  "The header has the following format (all 'binary' integer entries are written in" << '\n';
		std::clog <<  "little endian):                                                                 " << "\n\n";
		std::clog <<  "    <type1>[ex: u32 or i64]:<# of docs>[plaintext base-10]\\n                   " << '\n';
		std::clog <<  "    <#words 1st doc><#words 2nd doc> ...[each entry is binary type1]            " << '\n';
		std::clog <<  "    <type2><type3>:-1\\n                                                       " << "\n\n";
		std::clog <<  "Then the contents of the file are written as <word ID><word Count> pairs where  " << '\n';
		std::clog <<  "the word ID is its line # (starting from 0) in the dictionary file and the word " << '\n';
		std::clog <<  "Count is the the number of occurrences of the word with that ID in the original " << '\n';
		std::clog <<  "document.  The entries <#words Nth doc> indicate how many ID-Count pairs there  " << '\n';
		std::clog <<  "will be for the Nth document while <type2> and <type3> indicate the types of    " << '\n';
		std::clog <<  "the ID and Count entries respectively. There are no separators between pairs,   " << '\n';
		std::clog <<  "nor between documents, so the remainder of the contents look like:              " << "\n\n";
		std::clog <<  "    <1st word ID 1st doc><1st word Count 1st doc><2nd word ID 1st doc><2nd word " << '\n';
		std::clog <<  "    Count 1st doc>...                                                           " << '\n';
		std::clog <<  "    <1st word ID 2st doc><1st word Count 2st doc> ...                           " << '\n';
		std::clog <<  "    <last word ID last doc><last word Count last doc>                           " << "\n\n";
	}
	else
	{
		std::clog <<  "Call with --verbose-help for an explanation of the bag-of-words file.           " << "\n\n";
	}
	std::clog <<  "Within a particular directory, all names listed in a file called '.ignore' will " << '\n';
	std::clog <<  "not be searched.  This includes both subdirectories and files.                  " << '\n';
	std::clog <<  "and a 'word-bag' file with a small header.                                      " << "\n\n";
	std::clog <<  "    --source=           -s= Specify the directory to recursive search.  Default is  " << '\n';
	std::clog <<  "                            the current working directory.                          " << '\n';
	std::clog <<  "    --more-stopwords=   -m= Specify a file listing words to ignore in plain-text.   " << '\n';
	std::clog <<  "                            Acceptable Delimiters: ,. \\t\\n                        " << '\n';
	std::clog <<  "    --destination-dict= -d= Specify the name of the dictionary file to save.        " << '\n';
	std::clog <<  "                            Defaults to stdout.                                     " << '\n';
	std::clog <<  "    --destination-bow=  -b= Specify the name of the bag-of-words file to save.      " << '\n';
	std::clog <<  "                            Defaults to stdout.                                     " << '\n';
	std::clog <<  "    --existing-dict=    -D= Specify a dictionary file to load.                      " << '\n';
	std::clog <<  "    --existing-bow=     -B= Specify a bag-of-words file to load.                    " << '\n';
	std::clog <<  "    --stop-names        -s  Ignore common names.  (equivalent to -m=names.txt       " << '\n';
	std::clog << std::flush;
}

int main(int argc, const char ** argv)
{
	namespace fs = std::experimental::filesystem;
	ArgParser args;
	try
	{
		args.parse_args(--argc, ++argv);
	}
	catch(const std::runtime_error & e)
	{
		std::cerr << e.what() << std::endl;
		std::clog << "Call with -h, --help, or --verbose-help for information on usage." << std::endl;
		return -1;
	}
	// file/directory related cmd-line args
	std::string source_dir(std::string(fs::current_path().native()));
	std::string names = "../data/names.txt";
	std::string more_stopwords;
	std::string existing_dictionary;
	std::string existing_bow;
	std::string destination_dictionary;
	std::string destination_bow;
	std::string * arg_dest;


	bool use_stopnames{false};

	/* Get command-line args. */
	auto store_arg = [&arg_dest](const std::string & str) -> void { (*arg_dest) = str; };
	if(args['h'].first or args["help"].first)
	{
		printhelp(false);
		return 0;
	}
	else if(args["verbose-help"].first)
	{
		printhelp(true);
		return 0;
	}
	else
	{
		try
		{
			arg_dest  = &source_dir;
			args.get_arg("source", 's', store_arg);
			arg_dest = &more_stopwords;
			args.get_arg("more-stopwords", 'm', store_arg);
			arg_dest = &existing_dictionary;
			args.get_arg("existing-dict", 'D', store_arg);
			arg_dest = &existing_bow;
			args.get_arg("existing-bow", 'B', store_arg);
			arg_dest = &destination_dictionary;
			args.get_arg("destination-dict", 'd', store_arg);
			arg_dest = &destination_bow;
			args.get_arg("destination-bow", 'b', store_arg);
			use_stopnames = args['n'].first or args["stop-names"].first;
		}
		catch(const std::runtime_error & e)
		{
			std::cerr << e.what() << std::endl;
			std::clog << "Call with -h, --help, or --verbose-help for information on usage." << std::endl;
			return -1;
		}
	}

	Corpus corpus;


	std::clog << "\nRunning with these settings:\n";
	std::clog << "source_dir             = '" << source_dir << '\'' << '\n';
	// std::clog << "stop_words             = '" << stop_words << '\'' << '\n';
	std::clog << "more_stopwords         = '" << more_stopwords << '\'' << '\n';
	std::clog << "existing_dictionary    = '" << existing_dictionary << '\'' << '\n';
	std::clog << "existing_bow           = '" << existing_bow << '\'' << '\n';
	std::clog << "destination_dictionary = '" << destination_dictionary << '\'' << '\n';
	std::clog << "destination_bow        = '" << destination_bow << '\'' << '\n';
	std::clog << std::endl;



	if(existing_dictionary.size())
	{
		std::ifstream dict(existing_dictionary);
		corpus.load_dictionary(dict);
	}
	if(existing_bow.size())
	{
		std::ifstream bow(existing_bow);
		corpus.load_documents(bow);
	}

	size_t next_strlen{0};
	std::string line;
	// std::ifstream fstopwords(stop_words);
	DocumentParser<void> parser{};
	// std::stringstream strm;

	std::getline(std::cin, line);
	while(std::cin)
	{
		std::stringstream strm(line);
		parser.parse_doc(corpus, strm);
		std::getline(std::cin, line);
	}


	if((not destination_dictionary.size()) or destination_dictionary == "stdout")
	{
		corpus.save_dictionary(std::cout);
	}
	else if(destination_dictionary != "null")
	{
		std::ofstream dict(destination_dictionary);
		corpus.save_dictionary(dict);
	}
	if((not destination_bow.size()) or destination_bow == "stdout")
	{
		corpus.save_documents(std::cout);
	}
	else if(destination_bow != "null")
	{
		std::ofstream bow(destination_bow);
		corpus.save_documents(bow);
	}
	std::cerr << "DONE" << std::endl;

	return 0;
}
