cd doc2bow/src/
make
cd ../../
head -n 30 /home/tim/c2vdata/papers-2017-02-21.json | python get_abstracts.py | doc2bow/src/doc2bow --source=null --destination-bow=LDA_Proj/abstracts.bow --destination-dict=LDA_Proj/abstracts.dict
cd LDA_Proj
python lda_run.py 2 20 1 abstracts.dict abstracts.bow
