#inlcude "clienthandler.h"

harmonize::ClientHandler::ClientHandler(ClientProtocol &client, int t)
: client_(std::move(client)), t_(t) {}

void harmonize::ClearHandler::operator() () {
  // Start the listener
  listener = new std::thread(&listen);
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
    int player_time = server_.GetPlayerTime(), ret;
    if (player_time > 0)
      ret = client_.SyncAt();
    if (ret || player_time < 0) {
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
