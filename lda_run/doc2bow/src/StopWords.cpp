/*
 * StopWords.cpp
 *
 *  Created on: Jul 10, 2017
 *      Author: tim
 */

#include "StopWords.h"
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include "stemming.h"
#include <boost/tokenizer.hpp>

static const std::array<const char *, 758>& builtin_stopwords();
StopWords::StopWords():
				words_(builtin_stopwords().begin(), builtin_stopwords().end())
{

}
StopWords::StopWords(std::istream & source):
						words_(builtin_stopwords().begin(), builtin_stopwords().end())
{
	more_stopwords(source);

}
void StopWords::more_stopwords(std::istream & source)
{
	using tokenizer_t = boost::tokenizer<boost::char_separator<char>, std::istreambuf_iterator<char>, std::string>;
	boost::char_separator<char> seps(" .,\n\t");
	auto tokenizer = tokenizer_t(std::istreambuf_iterator<char>(source), std::istreambuf_iterator<char>(), seps);
	for(auto word:tokenizer)
	{
		words_.insert(word);
		stem_word(word);
		if(word.size())
			words_.insert(word);
	}
}
void StopWords::add_word(const std::string & word)
{
	words_.insert(std::move(word));
}
void StopWords::add_word(std::string && word)
{
	words_.emplace(std::move(word));
}
bool StopWords::is_stop_word(const std::string & word) const
{
	return words_.count(word);
}
void StopWords::to_file(std::ostream & sink, bool alphabetical) const
{
	if(alphabetical)
	{
		std::vector<std::string> wordvec(words_.begin(), words_.end());
		std::sort(wordvec.begin(), wordvec.end());
		for(const auto & word:wordvec)
		{
			sink << word << '\n';
		}
	}
	else
	{
		for(const auto & word:words_)
		{
			sink << word << '\n';
		}
	}
}


static const std::array<const char *, 758>& builtin_stopwords()
{
	static std::array<const char *, 758> words
	{
		"'ll","'ve","a","a's","able","about","above","abst","accordance",
		"according","accordingly","across","act","actually","added","adj","affected",
		"affecting","affects","after","afterwards","again","against","ah","ain't",
		"all","allow","allows","almost","alone","along","already","also",
		"although","always","am","among","amongst","an","and","announce",
		"another","any","anybody","anyhow","anymore","anyone","anything","anyway",
		"anyways","anywhere","apart","apparently","appear","appreciate","appropriate","approximately",
		"are","aren","aren't","arent","arise","around","as","aside",
		"ask","asking","associated","at","auth","available","away","awfully",
		"b","back","be","became","because","become","becomes","becoming",
		"been","before","beforehand","begin","beginning","beginnings","begins","behind",
		"being","believe","below","beside","besides","best","better","between",
		"beyond","biol","both","brief","briefly","but","by","c",
		"c'mon","c's","ca","came","can","can't","cannot","cant",
		"cause","causes","certain","certainly","changes","clearly","co","com",
		"come","comes","concerning","consequently","consider","considering","contain","containing",
		"contains","corresponding","could","couldn't","couldnt","course","currently","d",
		"date","definitely","described","despite","did","didn't","different","do",
		"does","doesn't","doing","don't","done","down","downwards","due",
		"during","e","each","ed","edu","effect","eg","eight",
		"eighty","either","else","elsewhere","end","ending","enough","entirely",
		"especially","et","et-al","etc","even","ever","every","everybody",
		"everyone","everything","everywhere","ex","exactly","example","except","f",
		"far","few","ff","fifth","first","five","fix","followed",
		"following","follows","for","former","formerly","forth","found","four",
		"from","further","furthermore","g","gave","get","gets","getting",
		"give","given","gives","giving","go","goes","going","gone",
		"got","gotten","greetings","h","had","hadn't","happens","hardly",
		"has","hasn't","have","haven't","having","he","he'd","he'll",
		"he's","hed","hello","help","hence","her","here","here's",
		"hereafter","hereby","herein","heres","hereupon","hers","herself","hes",
		"hi","hid","him","himself","his","hither","home","hopefully",
		"how","how's","howbeit","however","hundred","i","i'd","i'll",
		"i'm","i've","id","ie","if","ignored","im","immediate",
		"immediately","importance","important","in","inasmuch","inc","indeed","index",
		"indicate","indicated","indicates","information","inner","insofar","instead","into",
		"invention","inward","is","isn't","it","it'd","it'll","it's",
		"itd","its","itself","j","just","k","keep","keeps",
		"kept","kg","km","know","known","knows","l","largely",
		"last","lately","later","latter","latterly","least","less","lest",
		"let","let's","lets","like","liked","likely","line","little",
		"look","looking","looks","ltd","m","made","mainly","make",
		"makes","many","may","maybe","me","mean","means","meantime",
		"meanwhile","merely","mg","might","million","miss","ml","more",
		"moreover","most","mostly","mr","mrs","much","mug","must",
		"mustn't","my","myself","n","na","name","namely","nay",
		"nd","near","nearly","necessarily","necessary","need","needs","neither",
		"never","nevertheless","new","next","nine","ninety","no","nobody",
		"non","none","nonetheless","noone","nor","normally","nos","not",
		"noted","nothing","novel","now","nowhere","o","obtain","obtained",
		"obviously","of","off","often","oh","ok","okay","old",
		"omitted","on","once","one","ones","only","onto","or",
		"ord","other","others","otherwise","ought","our","ours","ourselves",
		"out","outside","over","overall","owing","own","p","page",
		"pages","part","particular","particularly","past","per","perhaps","placed",
		"please","plus","poorly","possible","possibly","potentially","pp","predominantly",
		"present","presumably","previously","primarily","probably","promptly","proud","provides",
		"put","q","que","quickly","quite","qv","r","ranbuiltin_stopwords().begin(), ",
		"rather","rd","re","readily","really","reasonably","recent","recently",
		"ref","refs","regarding","regardless","regards","related","relatively","research",
		"respectively","resulted","resulting","results","right","run","s","said",
		"same","saw","say","saying","says","sec","second","secondly",
		"section","see","seeing","seem","seemed","seeming","seems","seen",
		"self","selves","sensible","sent","serious","seriously","seven","several",
		"shall","shan't","she","she'd","she'll","she's","shed","shes",
		"should","shouldn't","show","showed","shown","showns","shows","significant",
		"significantly","similar","similarly","since","six","slightly","so","some",
		"somebody","somehow","someone","somethan","something","sometime","sometimes","somewhat",
		"somewhere","soon","sorry","specifically","specified","specify","specifying","still",
		"stop","strongly","sub","substantially","successfully","such","sufficiently","suggest",
		"sup","sure","t","t's","take","taken","taking","tell",
		"tends","th","than","thank","thanks","thanx","that","that'll",
		"that's","that've","thats","the","their","theirs","them","themselves",
		"then","thence","there","there'll","there's","there've","thereafter","thereby",
		"thered","therefore","therein","thereof","therere","theres","thereto","thereupon",
		"these","they","they'd","they'll","they're","they've","theyd","theyre",
		"think","third","this","thorough","thoroughly","those","thou","though",
		"thoughh","thousand","three","throug","through","throughout","thru","thus",
		"til","tip","to","together","too","took","toward","towards",
		"tried","tries","truly","try","trying","ts","twice","two",
		"u","un","under","unfortunately","unless","unlike","unlikely","until",
		"unto","up","upon","ups","us","use","used","useful",
		"usefully","usefulness","uses","using","usually","v","value","various",
		"very","via","viz","vol","vols","vs","w","want",
		"wants","was","wasn't","wasnt","way","we","we'd","we'll",
		"we're","we've","wed","welcome","well","went","were","weren't",
		"werent","what","what'll","what's","whatever","whats","when","when's",
		"whence","whenever","where","where's","whereafter","whereas","whereby","wherein",
		"wheres","whereupon","wherever","whether","which","while","whim","whither",
		"who","who'll","who's","whod","whoever","whole","whom","whomever",
		"whos","whose","why","why's","widely","will","willing","wish",
		"with","within","without","won't","wonder","wont","words","world",
		"would","wouldn't","wouldnt","www","x","y","yes","yet",
		"you","you'd","you'll","you're","you've","youd","your","youre",
		"yours","yourself","yourselves","z","zero"
	};
	return words;
}
