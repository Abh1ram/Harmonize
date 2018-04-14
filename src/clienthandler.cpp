#include "clienthandler.h"

harmonize::ClientHandler::ClientHandler(ClientProtocol &client, int t)
: client_(std::move(client)), t_(t) {}

void harmonize::ClearHandler::operator() () {
  // Start the listener
  listener = new std::thread(&harmonize::ClientHandler::Listen, this);
  StartSyncLoop();
}

int harmonize::ClientHandler::Listen() {
  std::string line;
  while (line = client_.getline()) {
    handleCommand(line);
  }
}

int harmonize::ClientHandler::StartSyncLoop() {
  while (client_.IsConnected()) {
    playerpos pos = server_.GetPlayerPos(), ret;
    if (pos > 0)
      ret = client_.SyncAt(pos);
    if (ret || pos < 0) {
      // kill the listener thread
      return -1;
    }
    // Sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(t_));
  }
  return 0;
}

int harmonize::ClientHandler::SyncNow(int stime) {
  if (client_.IsConnected()) {
    return client_.SyncAt(stime);
  }
  else {
    return -1;
  }
}
