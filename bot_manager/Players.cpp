#include "Players.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace rapidxml;



Players::Players(string filename) {
	ifstream infile(filename);
	bool is_a_file = bool(infile);
	infile.close();
	if (!is_a_file) {
		string default_str = "<?xml version = \"1.0\" encoding = \"utf-8\"?>\n<players>\n\t<player name = \"joshua\">1337</player>\n</players>";
		ofstream file(filename);
		file << default_str;
		file.close();
	}
	this->filename = filename;
	rapidxml::file<> xmlFile(filename.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<> *pRoot = doc.first_node();
	for (xml_node<> *pNode = pRoot->first_node("player"); pNode; pNode = pNode->next_sibling())
	{
		string name = pNode->first_attribute("name")->value();
		int points = atoi(pNode->value());
		players[name] = points;
	}
}

void Players::printPlayers() {
	for (map<string, int>::iterator i = this->players.begin(); i != this->players.end(); i++) {
		cout << i->first << " " << i->second << endl;
	}
}

void Players::savePlayers() {
	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	xml_node<>* root = doc.allocate_node(node_element, "players");
	doc.append_node(root);


	for (map<string, int>::iterator i = this->players.begin(); i != this->players.end(); i++) {
		xml_node<>* child = doc.allocate_node(node_element, "player");
		child->append_attribute(doc.allocate_attribute("name", i->first.c_str()));
		char * node_value = doc.allocate_string(std::to_string(i->second).c_str());
		child->value(node_value);
		root->append_node(child);
	}
	std::ofstream myfile;
	myfile.open(filename);
	myfile << doc;
	myfile.close();
}

int Players::addPoint(std::string name) {
	players[name] = players[name]++;
	this->savePlayers();
	return players[name]; 
}