#ifndef __PLAYERS__
#define __PLAYERS__
#include <list>
#include <map>
#include <string>

class Players {
public:
	Players(std::string filename);
	Players() {};
	int addPoint(std::string name);
	void printPlayers();
	void savePlayers();
private:
	std::map<std::string, int> players;
	std::string filename;
};
#endif
