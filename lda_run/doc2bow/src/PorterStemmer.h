/*
 * PorterStemmer.h
 *
 *  Created on: Jul 10, 2017
 *      Author: tim
 */

#ifndef PORTERSTEMMER_H_
#define PORTERSTEMMER_H_




struct stemmer {
   char * b;       /* buffer for word to be stemmed */
   int k;          /* offset to the end of the string */
   int j;          /* a general offset into the string */
};



int stem(struct stemmer * z);



#endif /* PORTERSTEMMER_H_ */
