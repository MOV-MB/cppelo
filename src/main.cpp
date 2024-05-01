#include <iostream>
#include <vector>
#include "Player.hpp"
#include "Database.hpp"
#include "DataInput.hpp"
#include "Config.hpp"

Config cfg;

int main() {
    vector<Player> playerList;
    string cmd = "tail -f ~/Elo/log.log";

    db_init();
    db_create_table();

    cout << "cfg values: " << std::endl;
    cout << cfg.getIP()<< '\n' << cfg.getPathToFile() << '\n' << cfg.getPort() << '\n'<< cfg.getRconPass() << std::endl;

    if (!cfg.getPathToFile().empty()) {
        read_tail_stdout(cfg.getPathToFile(), playerList);
    }
    else {
        read_server_stdout("sh ./start.sh", playerList);
    }
}
