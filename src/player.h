#ifndef PLAYER_H
#define PLAYER_H

namespace harmonize {

using playerpos = unsigned int;

class Player {
protected:
  std::string file_name_;
public:
  Player(std::string &file_name);
  Player(const Player &other) = delete;
  virtual int Play(int time = 0) = 0;
  virtual int Pause();
  virtual int GetTime();
  virtual void operator() ();
  virtual ~Player();
};

/*
 * Takes in a text file, and outputs a character at
 * each second onto a new console that it starts
 */
class TextPlayer: public Player {
  enum class PlayStatus {
    Playing, Paused, Stopped
  };
  fstream file;
  std::mutex play_mutex_;
  std::condition_variable play_cond_;
  PlayStatus status_;
  const unsigned int kWaitTime = 1000; // Time to wait between outputting characters
public:
  TextPlayer();
  TextPlayer(const TextPlayer &other) = delete;
  virtual int Play(int time = 0);
  virtual int Pause();
  virtual int GetTime();
  virtual void operator() ();
  ~TextPlayer();
};

};

#endif
