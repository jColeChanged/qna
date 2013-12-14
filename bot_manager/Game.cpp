#include "Game.h"
using namespace std;

Game::Game(string question_file, string player_file, string name, int time) {
	this->questions = Questions(question_file);
	this->players = Players(player_file);
	this->name = name;
	this->time = time * 10;
	this->timer = 0;
	this->active = false;
}

void Game::startGame(Responses & responses) {
	this->active = true;
	this->changeQuestion(responses);
}

void Game::stopGame() {
	this->active = false;
}

void Game::changeQuestion(Responses & responses) {
	responses.push_back("The answer was: " + this->currentQuestion.second);
	this->currentQuestion = this->questions.getQuestion();
	responses.push_back("The new question is: " + this->currentQuestion.first);
	this->timer = 0;
}

bool Game::isCorrectAnswer(const string & message) {
	unsigned int size = message.size();
	string answer = this->currentQuestion.second;
	if (size != answer.size()) {
		return false;
	}
	for (unsigned int i = 0; i < size; i++) {
		if (tolower(message[i]) != tolower(answer[i])) {
			return false;
		}
	}
	return true;
}

Responses Game::processMessage(string author, string message) {
	Responses responses;
	if (this->isCorrectAnswer(message) && this->active) {
		int points = this->players.addPoint(author);
		responses.push_back("Correct! " + author + " is now at " + std::to_string(points) + " point(s).");
		this->changeQuestion(responses);
	}
	else if (message == this->name + " start" && !this->active) {
		this->startGame(responses);
	}
	else if (message == this->name + " stop" && this->active) {
		this->stopGame();
	}
	else if (message == this->name + " help") {
		responses.push_back("commands are start, stop, help");
	}
	return responses;
}

Responses Game::tick() {
	Responses responses;
	if (this->active) {
		this->timer++;
		if (this->timer == this->time) {
			this->changeQuestion(responses);
		}
	}
	return responses;
}