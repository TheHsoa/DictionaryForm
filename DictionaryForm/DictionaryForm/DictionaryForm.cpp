#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <map>
#include <algorithm>

using namespace std;

string readFile(char*); //запись файла в строку
void removeFile(char*); //удалить файл
bool fileExist(char*); // проверка на существование файла
void writeTextFileInDictFormFile(char* in_file_name, char* out_file_name); //запись стандартного текстового файла в файл в словарной форме
void addTextInDictFormFile(char* fileName, string text); //добавление очередного подтекста из файла в файл в словарной форме
int numOfWords(string text); // подсчет количества слов в файле(по количеству пробелов)
string delFirstAndLastSpaces(string text); // удаление из текста последнего и первого пробела
map<int, map<string, int>> wordsInText(string text); //представление слов в тексте в словарной форме в map-е
bool numComparison(const std::pair<string, int> &a, const std::pair<string, int> &b); //сравнение второму значению двух пар чисел(необходимо для сортировки вектора из пар по второму значению)
int numOfWordsInMap(map<int, map<string, int>> words); // подсчет количества слов в map-е

void main()
{
	char* inFileName = "input.txt";
	char* outFileName = "output.txt";
	writeTextFileInDictFormFile(inFileName, outFileName);
}
void writeTextFileInDictFormFile(char* inFile, char* outFile)
{
	removeFile(outFile);

	ifstream fin;	
	char ch;
	fin.open(inFile);
	
	fin.get(ch);	
	string mainHead = "";

	while(ch != '_')
	{
		mainHead += ch;
		fin.get(ch);
	}
	ofstream fos(outFile);
	fos << mainHead <<  endl;
	fos.close();
	string s = "";

	while (ch != '\n')
	{
		s += ch;
		fin.get(ch);
	}
	s += ch;

	while (fin.get(ch))
	{
		s += ch;
		if (ch == '\n' )
		{
			fin.get(ch);
			fin.get(ch);
			if (ch == '_') 
			{
				string head = "_";
				fin.get(ch);
				while (ch != '\n')
				{
					head += ch;
					fin.get(ch);
				}
				if (head != "_1_")
				{
					addTextInDictFormFile(outFile,s);
				}
				s = head + "\n";
			}
		}
	}
	addTextInDictFormFile(outFile, s.substr(0, s.length()));
}

void addTextInDictFormFile(char* fileName,string text)
{
	ofstream fos(fileName, ios_base::app);
	auto head = text.substr(0,text.find('\n')+1);
	fos << head;
	
	text = text.substr(text.find('\n') + 1);
	text = delFirstAndLastSpaces(text);
	fos << "N=" << numOfWords(text) << endl;

	auto words = wordsInText(text);
	
	fos << "N_words=" << numOfWordsInMap(words) << endl;

	map<int, map<string, int>>::reverse_iterator lenCur;
	map<string, int>::iterator wordCur;

	for(lenCur = words.rbegin(); lenCur != words.rend(); ++lenCur)
	{
		fos << (*lenCur).first << "(" << words[(*lenCur).first].size() << "):";

		vector<pair<string, int>> lenWordsVector(words[(*lenCur).first].begin(), words[(*lenCur).first].end());
		sort(lenWordsVector.begin(), lenWordsVector.end(), numComparison);

		for (auto i : lenWordsVector)
		{
			fos << i.first <<"(" << i.second << "); ";
		}
		fos << "\n";
	}
	fos << "\n";
	fos.close();
}

bool numComparison(const std::pair<string, int> &a, const std::pair<string, int> &b)
{
	return a.second > b.second;
}

int numOfWordsInMap(map<int, map<string, int>> words)
{
	auto num = 0;
	map<int, map<string, int>>::reverse_iterator lenCur;
	for (lenCur = words.rbegin(); lenCur != words.rend(); ++lenCur)
	{
		num += words[(*lenCur).first].size();
	}
	return  num;
}
map<int, map<string, int>> wordsInText(string text)
{
	map<int, map<string, int>> words;
	auto begin = 0;
	while (true)
	{
		int end = text.find(' ',begin);
		if (end != std::string::npos) {
			auto word = text.substr(begin, end - begin);
			begin = end + 1;
			if (words.find(word.length()) == words.end())
			{
				map <string, int> temp;
				temp.insert(pair<string, int >(word, 1));
				words.insert(pair<int, map<string, int>>(word.length(), temp));
			}
			else
			{
				if (words[word.length()].find(word) == words[word.length()].end())
				{
					words[word.length()].insert(pair<string, int>(word, 1));
				}
				else {
					words[word.length()][word] ++;
				}
			}
		}
		else
		{
			auto word = text.substr(begin, text.length() - begin - 1);
			if (words.find(word.length()) == words.end())
			{
				map <string, int> temp;
				temp.insert(pair<string, int >(word, 1));
				words.insert(pair<int, map<string, int>>(word.length(), temp));
			}
			else
			{
				if (words[word.length()].find(word) == words[word.length()].end())
				{
					words[word.length()].insert(pair<string, int>(word, 1));
				}
				else {
					words[word.length()][word] ++;
				}
			}
			break;
		}
	}
	return words;
}

int numOfWords(string text)
{
	return count(text.begin(), text.end(), ' ') + 1;
}

string delFirstAndLastSpaces(string text)
{
	if(text.find(' ') == 0)
	{
		text = text.substr(1, text.length()-1);
	}
	if (text.rfind(' ') == text.length() - 2) 
	{
		text = text.substr(0, text.length() - 2 ) + "\n";
	}
	return text;
}

string readFile(char* name) {

	ifstream fin;
	string s;

	fin.open(name);
	char ch;
	s = "";
	while (fin.get(ch)) {
		s += ch;
	}
	fin.close();
	return s;
}

void removeFile(char* fileName)
{	if (fileExist(fileName)) {
		
		if (remove(fileName) != 0)
			cout << "Can't delete file\n";
	}
}
bool fileExist(char* fileName)
{
	ifstream file(fileName);

	return file.good();
}