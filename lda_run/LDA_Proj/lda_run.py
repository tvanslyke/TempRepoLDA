#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import numpy as np
from nltk.tokenize.stanford import StanfordTokenizer
from nltk.stem.porter import PorterStemmer
import stop_words 
from tqdm import tqdm
from gensim import corpora
from gensim.models import ldamulticore as LDA
import sys

STANPATH = "stanford-corenlp.jar"
STAN = StanfordTokenizer(STANPATH)
USELESS = set(stop_words.get_stop_words('en') + ["can", "us", "I", "n't", ".", ",", "!", "?"])
STEMMER = PorterStemmer()




def is_word(word):
    for char in word:    
        if not (ord('a') <= ord(char.lower()) <= ord('z')):
            return False
    return True
   
def extract_words(words):
    ''' Yields de-stemmed non-stop-words from the input iterable. '''
    for token in STAN.tokenize(words):
        token = token.lower()
        if token not in USELESS and is_word(token):
            yield token


def reduce_docs(docs):
    ''' Maps words in a list of documents to unique integer ID's and reduces 
        repeated words to a value and a count.

        returns the word-to-ID map and the (per-document) frequency of each 
        word as a list of tuples of the form (ID, count).
    '''
    print "Constructing dictionary."
    Dic = corpora.hashdictionary.HashDictionary(docs)
    print "Constructing Bags-of-Words."
    return Dic, [Dic.doc2bow(doc) for doc in tqdm(docs)]


def tstr_to_nptype(s):
	if s[0] == 'u':
		return 'uint' + s[1:3]
	elif s[0] == 'i':
		return 'int' + s[1:3]
	else:
		assert(0)

def read_bow(bowf):
	first_line = bowf.readline()
	type1 = tstr_to_nptype(first_line[:3])
	numdocs = int(first_line[4:].strip())
	sys_is_LE = sys.byteorder = 'little'
	docbowlens = np.fromfile(bowf, count = numdocs, dtype = type1, sep = "")
	tail_second_line = bowf.readline()
	type2, type3  = tstr_to_nptype(tail_second_line[:3]), tstr_to_nptype(tail_second_line[3:6])
	bows = np.ndarray(shape = (numdocs,), dtype = np.ndarray)
#	for i in tqdm(xrange(numdocs)):
	if sys_is_LE:
		for i, bowlen in enumerate(tqdm(docbowlens)):
                        bows[i] = np.fromfile(bowf,
					      count = long(2 * bowlen), # BECAUSE NUMPY THINKS (uint64() * int() -> float64) SOMETIMES FOR SOME REASON?!?!?!
					      dtype = type2,
					      sep = "")\
					      .reshape((bowlen,  2))
	else:
		docbowlens.byteswap(inplace = True)
                for i, bowlen in enumerate(tqdm(docbowlens)):
                        bows[i] = np.fromfile(bowf,
					      count = long(2 * bowlen), # SAME
					      dtype = type2,
					      sep = "").reshape((bowlen, 2)).byteswap()
	return bows
def read_dic(dicf):
	dic = {}
	for i, line in tqdm(enumerate(dicf)):
		dic[i] = line.strip()
	return dic

if __name__ == '__main__':
	from sys import stdin, stdout, argv

	ntopics1 = int(argv[1])
	ntopics2 = int(argv[2])
	npasses = int(argv[3])
	print "Fetching bag-of-words and dictionary"
	dicf = open(argv[4], 'r')
	bowf = open(argv[5], 'r')
	dic, bow = read_dic(dicf), read_bow(bowf)
	print "Processsing documents..."
	# Dic, corpus = reduce_docs((extract_words(line.strip()) for line in stdin))

	for i in xrange(ntopics1, ntopics2 + 1):
		print "Processing model " + str(i) + "..."
		lda_model = LDA.LdaMulticore(bow, num_topics = i, id2word = dic, passes = npasses, workers = 30, batch = True, eval_every = None)
		print "Saving model ..."
		lda_model.save('models/model' + str(i) + ".lda")


