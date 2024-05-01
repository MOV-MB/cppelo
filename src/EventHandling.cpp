#include "EventHandling.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "Player.hpp"
#include "StringProcessing.hpp"


void event_player(string& line, vector<Player>& playerList) {
    int id = get_player_id(line);
    bool existing = false;
    for (auto element: playerList) {
        if (id == element.get_id()) {
            std::cout << "Client with ID: \"" << id << "\" already initialized.";
            return;
        }
    }

    Player plr(get_player_name(line), get_player_ip(line), get_player_guid(line), id);

    if (!db_find_player(plr)) {
        db_create_player(plr);
    }

    playerList.push_back(plr);
    std::cout << "PTR 1: " << &plr << endl;
}



void event_say(string& line, vector<Player>& playerList) {
    //1: say: blake: "chat example"
    string::size_type pos = line.find("\"!", 0);
    if (pos != string::npos) {
        if (line.find("!chill", pos) == pos+1)
        {
            event_say_chill(line, playerList);
        }

        if (line.find("!elo", pos) == pos+1)
        {
            event_say_elo(line, playerList);
        }
    }

}
void event_say_chill(string& line, vector<Player>& playerList) {
    string::size_type pos1 = line.find(':', 0);
    int id = stoi(line.substr(0, pos1));
    if (line.find("\"!chill") != string::npos) {
        for (auto & i : playerList) {
            if (id == i.get_id()) {
                i.flip_chill();
                return;
            }
        }
    }
}
void event_say_elo(string& line, vector<Player>& playerList) {
    std::cout << "elo reached?" << endl;
    string::size_type pos1 = line.find(':', 0);
    int id = stoi(line.substr(0,pos1));
    if (line.find("\"!elo") != string::npos) {
        for (auto &x: playerList) {
            stringstream stream;
            stream << x.get_mmr();
            if (x.get_id() == id) {
                send_stuff("svtell " + std::to_string(id) + " Your ELO: ^2[^3" + stream.str() + "^2]");
            }
        }
    }
}

void event_broadcast(std::string& line, vector<Player>& playerList) { //event_broadcast
    std::string::size_type i = line.find("@@@PLD");
    if (i != string::npos) {
        event_broadcast_duel(line, playerList);
        return;
    }

    i = line.find("@@@PLR");
    if (i != std::string::npos) {
        event_broadcast_name(line, playerList);
        return;
    }

}
void event_broadcast_name(std::string& line, vector<Player>& playerList) {
    //broadcast: print "J^4|^7P ^4b^7lake_^4w^7on^7 @@@PLRENAME blake\n
    
    static std::string delimiter = "@@@PLRENAME";
    static std::string::size_type deli_size = delimiter.size();

    std::string::size_type pos1 = line.find('\"') + 1;
    std::string::size_type pos2 = line.find(delimiter) -1;

    std::string name1;
    std::string name2;

    pos_substr(line, name1, pos1, pos2 - 2);
    std::cout << "split name substr1 : " << name1 << endl;

    pos1 = pos2 + deli_size + 2;
    pos2 = line.find('\\', pos1);

    pos_substr(line, name2, pos1, pos2);
    std::cout << "split name substr2 : " << name2 << endl;

    try {
        Player* ptr = find_by_name(playerList, name1);
        if (ptr) ptr->set_name(name2);
        else {
            std::cout << "Player not found, returning" << endl;
            return;
        }
        std::cout << "\"" << name1 << "\" Has changed the name to: \"" << playerList[0].get_name() << "\"" << endl;

    }
    catch (int e) {
        std::cout << "Error in setting the name." << endl;
    }
}

void event_broadcast_duel(std::string& line, vector<Player>& playerList) {
    //broadcast: print "blake @@@PLDUELACCEPT segi!\n"

    static std::string delimiter = "@@@PLDUELACCEPT";
    static std::string::size_type deli_size = delimiter.size();
    std::string::size_type pos1 = line.find('\"') + 1;
    std::string::size_type pos2 = line.find(delimiter);

    std::string name1;
    std::string name2;

    pos_substr(line, name1, pos1, pos2 - 1);

    pos1 = pos2 + deli_size + 1;
    pos2 = line.find('\\', pos1) - 1;

    pos_substr(line, name2, pos1, pos2);

    Player* p1 = find_by_name(playerList, name1);
    Player* p2 = find_by_name(playerList, name2);
    
    if (p1) {
        p1->Player::duel_start(playerList, p2->get_id());
    }
    else
        std::cout << "Player \"" + name1 + "\" has not been found.";


    std::cout << "NAME1: " << "+" + name1 + "+" << endl << "NAME2: " << "+" + name2 + "+" << endl;


}

void event_kill(std::string& line, vector<Player>& playerList) {
    //Kill: 0 1 86: segi killed blake by MOD_SABER
    std::string::size_type pos1 = line.find(": ", 0) + 2;
    std::string::size_type pos2 = line.find(' ', pos1);

    std::string pid1;

    pos_substr(line, pid1, pos1, pos2);

    std::string pid2;

    pos1 = pos2 + 1;
    pos2 = line.find(' ', pos1 + 1);

    pos_substr(line, pid2, pos1, pos2);

    Player* player = find_by_id(playerList, stoi(pid1));
    if (player->in_duel()) {
        player->duel_win(playerList, stoi(pid2));
    }
    else
    {
        std::cout << "Player not in duel.";
        return;
    }
    }


void event_clientdisconnect(std::string& line, std::vector<Player>& playerList) {
    //ClientDisconnect: 0
    std::string::size_type pos1 = line.find(": ", 0) + 2;
    int pid = stoi(line.substr(pos1, line.size()-pos1));

    for (vector<Player>::size_type i = 0; i != playerList.size(); i++) {
        db_save_player(playerList[i]);
        playerList.erase(playerList.begin() + i);
        return;
    }
    std::cout << "Uninitialized player disconnected. ID: " << pid << endl;
}

void event_shutdowngame(vector<Player>& playerList) {
    std::cout << "Shutdowngame" << endl;
    for (auto& x: playerList) {
        db_save_player(x);
    }
    std::cout << "Hello1" << endl;
    playerList.clear();
    std::cout << "Hello2" << endl;
}
