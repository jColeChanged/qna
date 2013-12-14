#include <string>
#include <list>
#ifndef __QUESTIONS__
#define __QUESTIONS__
#include <vector>
#include <fstream>
#include <random>
#include <iostream>

class Questions {
public:
	Questions::Questions() {};
	Questions::Questions(std::string filename);
	void loadQuestions(std::string filename);
	std::pair<std::string, std::string> getQuestion();
	void printQuestion(std::pair<std::string, std::string>);
	void Questions::printRandomQuestions();
private:
	std::vector<std::pair<std::string, std::string>> questions;
};
#endif

