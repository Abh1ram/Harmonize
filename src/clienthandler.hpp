#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <thread>
#include <memory>

namespace harmonize {

/*
 * The ClientHandler, handles all interaction with the
 * client in a separate thread. Spawns it's own listening
 * thread to recieve commands.
 *
 * The ClientHandler owns the `ClientProtocol' object
 * inside it. During destruction, the connection must
 * be closed.
 */
class ClientHandler {
  ClientProtocol client_;
  unsigned int t_;
  std::unique_ptr<std::thread> listener;
public:
  ClientHandler(ClientProtocol &client, int t);
  ClientHandler(ClientHandler &other) = delete;
  void StartSyncLoop();
  int SyncNow(int stime);
  bool operator()();
private:
  int Listen();
  void handleCommand(std::string &cmd);
};

};

#endif
