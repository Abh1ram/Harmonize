#include <harmonize.hpp>
#include <sys/socket.h>
#include <stdexcept>
#include <string>
#include <memory>

bool harmonize::validate_ip_addr(const std::string &ip_addr) {
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
  return result;
}

// Standard client constructor
harmonize::
TCPConnectivity::TCPConnectivity(std::string &ip,
                                 std::string &port,
                                 Connectivity::NodeType type)
    : server_ip_(ip), port_(port), node_type_(type) {
  if (!validate_ip_addr(server_ip_)) {
    throw std::invalid_argument("ip_addr");
  }
  else if (type != Connectivity::NodeType::Client) {
    // Wrong constructor called
    throw std::invalid_argument("ip");
  }
}

// This is the constructor for spawning a new connection to 
// client
harmonize::TCPConnectivity::TCPConnectivity(struct csock_connection conn)
    : remote_conn_(conn) { }

// Throws for invalid ip, port
ServerProtocol harmonize::TCPConnectivity::Connect() {

}


ClientProtocol harmonize::TCPConnectivity::Listen() {
  if (node_type_ != Connectivity::NodeType::Server) {
    throw std::domain_error("Only Server can 'listen'");
  }

  struct csock_connection new_conn;
  if (listen_and_accept(&listener_, &new_conn, port.c_str())) {
    throw std::runtime_exception(
      "Error accepting connections on port " + port_0);
  }

  unique_ptr<TCPConnectivity> client_conn(new TCPConnectivity(new_conn));
  // Now the new_client owns the connection
  ClientProtocol new_client(std::move(client_conn));
  return new_client;
}

int harmonize::TCPConnectivity::Send(const std::string &cmd) {
  if (!remote_conn_.is_open()) {
    throw std::runtime_exception("Need an open connection");
  }
  if (csock_send(remote_conn_, cmd.c_str(), cmd.size() + 1)) {
    return ENOSEND;
  }
  return 0;
}

int harmonize::TCPConnectivity::ReceiveLine(std::string *line) {
  if (!remote_conn_.is_open()) {
    throw std::runtime_exception("Need an open connection");
  }
  char buf[kMaxCmdLen];
  int ret = csock_readline(remote_conn_, buf, kMaxCmdLen);
  if (ret <= 0) {
    return ENORECEIVE;
  }
  else {
    line->append(buf, ret);
  }
  return 0;
}