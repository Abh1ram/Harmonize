#include "clientprotocol.hpp"
#include "connectivity.hpp"
#include <string>

const std::string harmonize::ClientProtocol::kCmdBounceBack = "bounceback";
const std::string harmonize::ClientProtocol::kCmdSyncAt = "syncat -t ";
const std::string harmonize::ClientProtocol::kCmdPlay = "play";
const std::string harmonize::ClientProtocol::kCmdPause = "pause";
const std::string harmonize::ClientProtocol::kCmdStop = "stop";

harmonize::ClientProtocol::ClientProtocol(std::unique_ptr<Connectivity> &conn)
: conn_(std::move(conn)) {}

std::string harmonize::ClientProtocol::getline() {
  string line;
  conn_->ReceiveLine(&line);
  return line;
}

bool IsConnected() {
    return (*conn_);
}

int harmonize::ClientProtocol::SyncAt(playerpos pos) {
  std::string cmd = kCmdSyncAt + std::to_string(pos);
  return conn_->Send(cmd);
}

int harmonize::ClientProtocol::Play() {
  return conn_->Send(kCmdPlay);
}

int harmonize::ClientProtocol::Pause() {
  return conn_->Send(kCmdPause);
}

int harmonize::ClientProtocol::Stop() {
  return conn_->Send(kCmdStop);
}
