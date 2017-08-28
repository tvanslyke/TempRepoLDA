bowfile=/home/tim/lda_run/LDA_Proj/abstracts.bow
jsonfile=/home/tim/c2vdata/papers-2017-02-21.json
modelroot=/home/tim/lda_run/LDA_Proj/models/

python topics_over_time.py $modelroot"model2.lda" $bowfile | python plot_topics.py 2 $jsonfile & \
python topics_over_time.py $modelroot"model3.lda" $bowfile | python plot_topics.py 3 $jsonfile & \
python topics_over_time.py $modelroot"model4.lda" $bowfile | python plot_topics.py 4 $jsonfile & \
python topics_over_time.py $modelroot"model5.lda" $bowfile | python plot_topics.py 5 $jsonfile & \
python topics_over_time.py $modelroot"model6.lda" $bowfile | python plot_topics.py 6 $jsonfile & \
python topics_over_time.py $modelroot"model7.lda" $bowfile | python plot_topics.py 7 $jsonfile & \
python topics_over_time.py $modelroot"model8.lda" $bowfile | python plot_topics.py 8 $jsonfile & \
python topics_over_time.py $modelroot"model9.lda" $bowfile | python plot_topics.py 9 $jsonfile & \
python topics_over_time.py $modelroot"model10.lda" $bowfile | python plot_topics.py 10 $jsonfile & \
python topics_over_time.py $modelroot"model11.lda" $bowfile | python plot_topics.py 11 $jsonfile & \
python topics_over_time.py $modelroot"model12.lda" $bowfile | python plot_topics.py 12 $jsonfile & \
python topics_over_time.py $modelroot"model13.lda" $bowfile | python plot_topics.py 13 $jsonfile & \
python topics_over_time.py $modelroot"model14.lda" $bowfile | python plot_topics.py 14 $jsonfile & \
python topics_over_time.py $modelroot"model15.lda" $bowfile | python plot_topics.py 15 $jsonfile & \
python topics_over_time.py $modelroot"model16.lda" $bowfile | python plot_topics.py 16 $jsonfile & \
python topics_over_time.py $modelroot"model17.lda" $bowfile | python plot_topics.py 17 $jsonfile & \
python topics_over_time.py $modelroot"model18.lda" $bowfile | python plot_topics.py 18 $jsonfile & \
python topics_over_time.py $modelroot"model19.lda" $bowfile | python plot_topics.py 19 $jsonfile & \
python topics_over_time.py $modelroot"model20.lda" $bowfile | python plot_topics.py 20 $jsonfile
