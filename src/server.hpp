#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "player.h"
#include "connectivity.h"

namespace harmonize {
/*
 * The main server thread. Inits the various components
 * and starts listening for connections on the main
 * thread. Starts a new thread for each new connection
 */
class Server {
  std::string port_, file_name_; // Port to listen on and the file to play
  harmonize::Player player_;  // Player playing the file
  harmonize::Connectivity conn_;
  harmonize::CommandCentral cc_;
  unsigned int sync_time_;
  // List of all listener threads
public:
  Server(std::string &port, std::string &file_name);
  int Start();
private:
  void QueueNewClient(ClientProtocol client);
};

}

#endif
