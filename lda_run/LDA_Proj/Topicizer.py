# -*- coding: utf-8 -*-
"""
Created on Thu Jul  6 16:08:41 2017

@author: tim
"""

from lda_test import extract_words, reduce_docs
if __name__ == '__main__':
    import matplotlib.pyplot as plt
    from gensim.models.ldamodel import LdaModel
    from sys import stdin
    from lda_test import print_topics
    SAVED_MODEL_PATH = "/home/tim/LDA_Proj/.lda_save"
    
    print "Loading saved LDA model from disk."
    lda = LdaModel.load(SAVED_MODEL_PATH)
    print "Done."
    print "Parsing document."
    Dic, bow = reduce_docs([list(extract_words(stdin.read()))])
    print "Done."
    print "Analyzing..."
    ids, probs = zip(*(list(lda.get_document_topics(bow, minimum_probability = .001))[0]))
    print_topics(lda, topn = 5)
    plt.bar(ids, probs)
    plt.show()
    
