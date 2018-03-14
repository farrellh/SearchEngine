#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include "webpage.h"
#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


class SearchEngine {

 public:
  SearchEngine();
  SearchEngine(std::ifstream &input, std::ifstream &query, std::ofstream &output);
  ~SearchEngine();
  void Query(std::ifstream &query, std::ofstream &output);

 private:
 	void SingleWordSearch(std::string querycommand, std::ofstream &output);
 	void IncomingSetter();
 	void WordMap();
 	void Lowercase(std::string& querycommand);
 	std::map<std::string, std::set<WebPage*>> wordmap;
	std::map<std::string, WebPage*> webpages;
	std::string filename;
	std::string querytext;

};
#endif