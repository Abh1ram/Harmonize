#include "server.h"

harmonize::Server::Server(std::string &port, std::string &file_name,
    unsigned int sync_time = 100)
: port_(port), file_name_(file_name), conn_(port), sync_time_(sync_time) {
    player_ = Player(file_name_);
    cc_ = CommandCentral(player_);
}

/*
 * Start a new thread to handle client. Send synchronize messages
 * every t seconds and listen for any messages.
 */
void harmonize::Server::QueueNewClient(ClientProtocol client) {

}

/*
 * Start player, the command listener and listen for new
 * clients. QueueNewClient synchronizer in a new thread.
 */
int harmonize::Server::Start() {
  CommandLineListener cmd(cc_);
  // Start the player in a new thread
  std::thread player_thread(player_);
  std::thread cmd_thread(cmd);


  ClientProtocol new_client;
  // listen for new connections
  while (new_client = conn_.listen()) {
    shared_ptr<harmonize::ClientHandler> client_handler = std::make_shared(
      new harmonize::ClientHandler(new_client, sync_time_);
    );
    cc_.AddClientHandler(client_handler);
    client_threads_.push_back(std::thread(*client_handler));
  }
}
