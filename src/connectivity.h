#include <string>

namespace harmonize {
// Abstracts the entire connectivity layer of Harmonize.
// Harmonize implements it's protocol to speak between the nodes
// which the higher layer knows about
// Thus, the connectivity layer is going to contain code like
// TCPConnectivity::Send(harmonize::Sync data);
// Example:
//    Connectivity *conn = new TCPConnectivity(ip, port);
//    ServerProtocol server;
//    if (conn->Connect(server) != harmozine::SUCCESS) {
//    cerr << "Error connecting to server";
//    exit(1);
//    }
//    std::string sync_command = "sync -t 10:23";
//    if (conn->Send(sync_command)) {
//      cerr << "Error sending command";
//      exit(1);  
//    }
//    else {
//      cout << "Sync successful";
//      exit(0);
//    }
class Connectivity {
protected:
  Connectivity::NodeType node_type_;
public:
  /* Represents the types
   * harmonizeclient and harmonizeServer
   */
  enum class NodeType {
    Server, Client
  };
  virtual int Send(const std::string &cmd) = 0;
  virtual int Receive(std::string *line) = 0;
  virtual int Scan() = 0;
  virtual ServerProtocol Connect() = 0;
  virtual ClientProtocol Listen() = 0;
  virtual ~Connectivity() = default;
};


// TODO It's most probably better to separate server
// and client in TCP
class TCPConnectivity : public Connectivity {
private:
  // TODO should not be strings
  std::string server_ip_;
  std::string port_; // On server, this is the listen port
  struct csock_listener listener_; // The listener data
  struct csock_connection remote_conn_; // This stores the persistant connection data
public:
  TCPConnectivity(std::string ip, std::string port,
      Connectivity::NodeType type);
  TCPConnectivity(struct csock_connection conn);
  // Can't copy the object because it represents a 
  // persistant TCP connection
  TCPConnectivity(const TCPConnectivity &copy_from) = delete;

  virtual int Send(const std::string &cmd);
  virtual int ReceiveLine(std::string *line);
  virtual int Scan();
  virtual ServerProtocol Connect();
  virtual ClientProtocol Listen();
  ~TCPConnectivity();
};

bool validate_ip_addr(const string &ip);

}
