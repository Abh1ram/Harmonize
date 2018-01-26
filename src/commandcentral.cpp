#include "commandcentral.hpp"

void AddClientHandler(std::shared_ptr<harmonize::ClientHandler> &handler) {
  handlers_.push_back(handler);
}

playerpos harmonize::CommandCentral::GetPlayerPos() {
  if (player_)
    return player->GetCurrentPos();
  else
    return -1;
}

// Pause the player and all clients
void harmonize::CommandCentral::PauseAll() {
  player_->Pause();
  for (auto &handler: handlers_) {
    int ret = handler->PauseClient();
  }
}

// Play the player and all clients
void harmonize::CommandCentral::PlayAll() {
  player_->Play();
  for (auto &handler: handlers_) {
    int ret = handler->PlayClient();
  }
}

// Stop the player and all clients
void harmonize::CommandCentral::PlayAll() {
  player_->Stop();
  for (auto &handler: handlers_) {
    int ret = handler->StopClient();
  }
}
