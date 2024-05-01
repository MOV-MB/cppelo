#include <iostream>
#include <vector>
#include <string>

#include "Player.hpp"


void read_server_stdout(string cmd, vector<Player> &playerList);

void read_tail_stdout(const string& cmd, vector<Player> &playerList);