import numpy as np
import matplotlib.pyplot as plt
from sys import argv
import json
doc_count = 7210983

json_source_fname = argv[2]
lda_file = argv[3]
def make_fname(year, topic_count):
	return "topics-" + str(topic_count) + "/topic-distribution-" + str(year)

def load_year_data():
	year_data =  np.ndarray(shape = (doc_count, ), dtype = np.int16)
	with open(json_source_fname, 'r') as f:
		for i, line in tqdm(enumerate(f), total = doc_count):
			try:
				year_data[i] = int(json.loads(line.strip())["year"])
			except:
				year_data[i] = -1
	return year_data

if __name__ == '__main__':
	from sys import stdin, argv
	import matplotlib.pyplot as plt
	from tqdm import tqdm
	from gensim.models.ldamulticore import LdaMulticore
	topic_count = int(argv[1])
	years = load_year_data()
	year_range = xrange(np.amin(years[years != -1]), np.amax(years) + 1)
	topic_year_mat = np.ndarray(shape = (topic_count, len(year_range)), dtype = np.uint32)
	lda_model = LdaMulticore.load(lda_file)
	for i, line in tqdm(enumerate(stdin), total = doc_count):
		maxm = 0
		idx = 0
		if years[i] < 0:
			continue
		for j, value in enumerate( (float(item) for item in line.strip().split()) ):
		 	maxm, idx  = (value, j) if value > maxm else (maxm, idx)
		topic_year_mat[idx][years[i] - year_range[0]] += 1
	for i, dist in tqdm(enumerate(topic_year_mat)):
		f = plt.figure(i)
		f, (ax1, ax2) = plt.subplots(1, 2)
		ax1.plot(year_range, dist)
		ax1.set_title("# Papers for Topic " + str(i) + " over time.")
		ax1.set_xlabel("Year")
		ax1.set_ylabel("# of Papers w/ Topic" + str(i))
		words, probs = zip(*lda_model.get_topic_terms(i, topn = 5))
		ax2.bar(range(5), probs)
		ax2.set_title("Top 5 Words for Topic " + str(i))
		ax2.set_xlabel("Key Words")
		ax2.set_ylabel("Key Word Probability")
		ax2.set_xticks(range(5))
		ax2.set_xticklabels([lda_model.id2word[word] for word in words])
		plt.savefig("topics-" + str(topic_count) + "/topic-" + str(i), dpi = 200)



