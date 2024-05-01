#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <vector>
#include <fstream>

class Config {
public:
    // Constructor declaration
    explicit Config(const std::string& filename = "mb2elo.cfg");

    // Getter methods for class members
    [[nodiscard]] std::string getPathToFile() const;
    [[nodiscard]] int getPort() const;
    [[nodiscard]] std::string getIP() const;
    [[nodiscard]] std::string getRconPass() const;

private:
    // Utility function for initialization from file
    static std::string initValue(const std::string& query, const std::vector<std::string>& lines);

    // Member variables
    std::string path_to_file;
    int port;
    std::string ip;
    std::string rconPass;

    // Utility function to trim whitespace
    static std::string trim(const std::string& str);
};
#endif // CONFIG_HPP

/*[SETTINGS]
#For the values you do not want to change leave as-is
#Lines that start with "#" are comments only


port = 29070
#Default port for MBII servers is 29070, change if you have a custom one

ip = 127.0.0.1
#IP address which Elo will communicate with, do not change unless you know what you're doing

#logpath = path/to/log/file
#uncomment this line to use tail -f of redirected stderr instead of reading stdout directly
#edit start.sh like so: ./openjkded.i386 +set fs_game "MBII" +set net_port 29070 +exec server.cfg 2>path/to/log/file


elogain = 10
#This refers to K in elo's original equation, if you want to tinker with it I suggest reading up on the math first.

rcon = rconpassword
#rcon password of your server, the script will not work without it.
#this must be edited to the rconpassword of your server
*/
