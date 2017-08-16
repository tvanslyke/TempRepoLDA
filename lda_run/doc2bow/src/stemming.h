/*
 * filtering.h
 *
 *  Created on: Jul 10, 2017
 *      Author: tim
 */

#ifndef STEMMING_H_
#define STEMMING_H_

#include "StopWords.h"
#include <string>



void stem_word(std::string & str);

bool validate_word(const std::string & str);

#endif /* STEMMING_H_ */
