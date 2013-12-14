#include "Questions.h"
using namespace std;

bool is_file(string filename) {
	ifstream infile(filename);
	return bool(infile);
}
Questions::Questions(string filename) {
	this->loadQuestions(filename);
}


void Questions::loadQuestions(string filename) {
	if (!is_file(filename)) {
		ofstream file(filename);
		file << "Set up a questions file at: " + filename + "\nSet up a questions file at:  " + filename;
		file.close();
	}
	ifstream infile(filename);
	while (!infile.eof()) {
		string question = "";
		string answer = "";
		getline(infile, question);
		if (!infile.eof()) {
			getline(infile, answer);
		}
		if (question != "" && answer != "") {
			this->questions.push_back(pair<string, string>(question, answer));
		}
	}
}

pair<string, string> Questions::getQuestion() {
	uniform_int_distribution<int> distribution(0, this->questions.size() - 1);
	std::random_device r;
	std::mt19937 gen(r());
	int random_number = distribution(gen);
	return this->questions[random_number];
}

void Questions::printQuestion(pair<string, string> question) {
	cout << question.first << " " << question.second << endl;
}

void Questions::printRandomQuestions() {
	int i = 0;
	while (i < 10) {
		this->printQuestion(this->getQuestion());
		i++;
	}
}
