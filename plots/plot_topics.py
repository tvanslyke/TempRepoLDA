import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from sys import argv
import json
doc_count = 7210983

json_source_fname = argv[2]

def load_year_data():
	year_data =  np.ndarray(shape = (doc_count, ), dtype = np.int16)
	with open(json_source_fname, 'r') as f:
		for i, line in zip(xrange(doc_count), f):
			try:
				year_data[i] = int(json.loads(line.strip())["year"])
			except:
				year_data[i] = -1
	return year_data
if __name__ == '__main__':
	from sys import stdin, argv
	import matplotlib.pyplot as plt
	from tqdm import tqdm

	topic_count = int(argv[1])
	topic_dist = np.ndarray(shape = (doc_count, topic_count), dtype = np.float32)
	for i, line in tqdm(enumerate(stdin), total = doc_count):
		for j, value in enumerate( (float(item) for item in line.strip().split()) ):
			topic_dist[i, j] = value
	years = load_year_data()
	year_range = xrange(np.amin(years[years != -1]), np.amax(years) + 1)
	sums = np.ndarray(shape = (len(year_range), topic_count), dtype = np.float32)
	means = np.ndarray(shape = (len(year_range), topic_count), dtype = np.float32)
	norms = np.ndarray(shape = (len(year_range), topic_count), dtype = np.float32)
	for i, year in tqdm(enumerate(year_range)):
		this_year_docs = topic_dist[years == year]
		for j, col in enumerate(this_year_docs.transpose()):
			sums[i, j] = np.sum(col)
			means[i, j] = sums[i, j] / len(col)
			norms[i, j] = np.sqrt(np.sum(col ** 2))
	sum_max = np.amax(sums)
	mean_max = np.amax(means)
	norm_max = np.amax(norms)
	f, axarr = plt.subplots(3, sharex = True)
	def animate(i, *fargs):
		year = year_range[i]
		plt.suptitle("Topic Likelihood for " + str(year))
		axarr[0].cla()
		axarr[1].cla()
		axarr[2].cla()
		axarr[0].bar(xrange(topic_count), sums[i], label = "Sums")
		axarr[1].bar(xrange(topic_count), means[i], label = "Means")
		axarr[2].bar(xrange(topic_count), norms[i], label = "Norms")
		axarr[0].legend(loc = "upper right");
		axarr[1].legend(loc = "upper right");
		axarr[2].legend(loc = "upper right");
		axarr[0].set_ylim(0, sum_max)
		axarr[1].set_ylim(0, mean_max)
		axarr[2].set_ylim(0, norm_max)
		plt.xticks(xrange(topic_count))
		plt.xlabel("Topic ID")
		return axarr
	anim = animation.FuncAnimation(f, animate, frames = len(year_range) - 1, repeat = False)
	anim.save("topic-distribution-yearly-" + str(num_topics) + ".mp4", fps = 10, dpi = 200)

