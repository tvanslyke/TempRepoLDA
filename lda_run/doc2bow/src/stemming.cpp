/*
 * filtering.cpp
 *
 *  Created on: Jul 10, 2017
 *      Author: tim
 */


#include "PorterStemmer.h"
#include <string>
#include <iostream>



void stem_word(std::string & str)
{
	// have to cheat a little bit because std::string::data() is a const member function in g++5.4
	char & first = str.front();
	stemmer stems{&first, int(str.size()) - 1, int(str.size()) - 1};
	str.resize(stem(&stems) + 1);
}




