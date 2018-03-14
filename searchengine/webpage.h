#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <map>

class WebPage {

 public:
  WebPage();
  WebPage(std::string filename);
  ~WebPage();
  std::set<std::string> getOutLinks();
  std::set<std::string> getInLinks();
  std::set<std::string> getWords();
  std::string getName();
  void setInLinks(std::string filename);

 private:
  void parse();
  std::set<std::string> word;
  std::set<std::string> outgoing;
  std::set<std::string> incoming;
  std::string name;

};
#endif