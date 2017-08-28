from sys import argv, stdin, stdout, stderr
import sys
from gensim.models.ldamulticore import LdaMulticore
import numpy as np
import matplotlib.pyplot as plt
import json
from tqdm import tqdm

def tstr_to_nptype(s):
        if s[0] == 'u':
                return 'uint' + s[1:3]
        elif s[0] == 'i':
                return 'int' + s[1:3]
        else:
                assert(0)


def read_bow(bowf, skip_lines = 0):
	first_line = bowf.readline()
        type1 = tstr_to_nptype(first_line[:3])
        numdocs = int(first_line[4:].strip())
        sys_is_LE = sys.byteorder == 'little'
        docbowlens = np.fromfile(bowf, count = numdocs, dtype = type1, sep = "")
        tail_second_line = bowf.readline()
        type2, type3  = tstr_to_nptype(tail_second_line[:3]), tstr_to_nptype(tail_second_line[3:6])
        if sys_is_LE:
                for bowlen in tqdm(docbowlens[:skip_lines]):
			np.fromfile(bowf, count = long(2 * bowlen), dtype = type2, sep = "")
		for i, bowlen in enumerate(tqdm(docbowlens)):
                        yield np.fromfile(bowf,
                                              count = long(2 * bowlen), # BECAUSE NUMPY THIN$
                                              dtype = type2,
                                              sep = "")\
                                              .reshape((bowlen,  2))
        else:
                docbowlens.byteswap(inplace = True)
                for _ in tqdm(docbowlens[:skip_lines]):
			np.fromfile(bowf, count = long(2 * bowlen), dtype = type2, sep = "")
                for i, bowlen in enumerate(tqdm(docbowlens)):
                        yield np.fromfile(bowf,
                                              count = long(2 * bowlen), # SAME
                                              dtype = type2,
                                              sep = "").reshape((bowlen, 2)).byteswap()



stderr.write("Loading Lda Model.\n")
model = LdaMulticore.load(argv[1].strip())
stderr.write("Done.\n")
doc_id_map = {}
word_map = {}
doc_count = 7210983

with open("/home/tim/c2vdata/distilled/sorted_doc_ids.txt", "r") as f:
	for i, line in tqdm(enumerate(f), total = doc_count):
		doc_id_map[int(line.strip())] = i
topic_likelihoods = np.ndarray(shape = (model.num_topics, 1), dtype = np.float32)



with open(argv[2], 'r') as bow_file:
	for doc_idx, bow in enumerate(read_bow(bow_file)):
		for topic_id, prob in model.get_document_topics(bow, minimum_probability = 0):
			topic_likelihoods[topic_id] = prob
		for prob in topic_likelihoods:
			sys.stdout.write(str(prob[0]))
			sys.stdout.write(" ")
		sys.stdout.write("\n")




