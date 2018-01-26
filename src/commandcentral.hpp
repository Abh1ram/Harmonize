#ifndef COMMANDCENTRAL_H
#define COMMANDCENTRAL_H

#include "clienthandler.h"
#include "player.h"
#include <memory>
#include <vector>

namespace harmonize {
/*
 * The server CommandCentral class, controls the global status of
 * all clients and the player on the server. Any message to be conveyed
 * to the server player and all the clients goes through a CommandCentral
 * object.
 */
class CommandCentral {
  std::vector<std::shared_ptr<harmonize::ClientHandler>> handlers_;
  std::shared_ptr<harmonize::Player> player_;
public:
  CommandCentral(std::shared_ptr<harmonize::Player> &player);
  void AddClientHandler(std::shared_ptr<harmonize::ClientHandler> &handler);
  void PauseAll();
  void PlayAll();
  void StopAll();
  playerpos GetPlayerPos();
};
};

#endif
