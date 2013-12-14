#ifndef __GAME__
#define __GAME__
#include "Questions.h"
#include "Players.h"
#include <string>
#include <iostream>
#include <list>


typedef std::list<std::string> Responses;

class Game {
public:
	Game(std::string, std::string, std::string, int);
	Game() {};
	~Game() {};
	Responses tick();
	Responses processMessage(std::string, std::string);
private:
	void announceQuestion();
	void startGame(Responses &);
	void stopGame();
	bool isCorrectAnswer(const std::string&);
	void changeQuestion(Responses &);
	std::pair<std::string, std::string> currentQuestion;
	std::string name;
	int time;
	int timer;
	bool active;
	Questions questions;
	Players players;
};
#endif