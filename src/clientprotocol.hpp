#ifndef CLIENTPROTOCOL_HPP
#define CLIENTPROTOCOL_HPP

namespace harmonize {
class ClientProtocol {
  std::unique_ptr<Connectivity> conn_;
  static const std::string kCmdBounceBack;
public:
  ClientProtocol(std::unique_ptr<Connectivity> &conn);
  ClientProtocol(ClientProtocol &other) = delete;
  std::string getline();
  int CalculateRTT();
  int Play();
  int Pause();
  int Stop();
};
};

#endif
