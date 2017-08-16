objects = ArgParser.o Corpus.o DocumentBuilder.o Document.o DocumentParser.o ioutils.o PorterStemmer.o stemming.o StopWords.o WordFilter.o WordMap.o Main.o

doc2bow: $(objects)
	g++ -O3 -std=c++17 $(objects) -lstdc++fs -o doc2bow


ArgParser.o: 
	g++ -O3 -std=c++17 -c ArgParser.cpp 

PorterStemmer.o:
	g++ -O3 -std=c++17 -c PorterStemmer.cpp 

stemming.o: 
	g++ -O3 -std=c++17 -c stemming.cpp

StopWords.o: stemming.o
	g++ -O3 -std=c++17 -c stemming.o StopWords.cpp 

WordMap.o: 
	g++ -O3 -std=c++17 -c WordMap.cpp

WordFilter.o: stemming.o StopWords.o
	g++ -O3 -std=c++17 -c stemming.o StopWords.o WordFilter.cpp

ioutils.o:
	g++ -O3 -std=c++17 -c ioutils.cpp

Document.o: ioutils.o
	g++ -O3 -std=c++17 -c ioutils.o Document.cpp

DocumentBuilder.o: 
	g++ -O3 -std=c++17 -c DocumentBuilder.cpp
DocumentParser.o:
	g++ -O3 -std=c++17 -c DocumentParser.cpp
Corpus.o: DocumentBuilder.o 
	g++ -O3 -std=c++17 -c DocumentBuilder.o Corpus.cpp
	
Main.o: Corpus.h filesystem_utils.h ArgParser.h DocumentParser.h
	g++ -O3 -std=c++17  -c Corpus.o ArgParser.o DocumentParser.o Main.cpp 

clean:
	rm $(objects)

