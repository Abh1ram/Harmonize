#include "player.h"
#include <stdexcept>
#include <mutex>
#include <condition_variable>

harmonize::TextPlayer::TextPlayer(): file(file_name_, ios::in) {
  if (!file.good()) {
    throw std::invalid_argument(file_name_);
  }
  // TODO Fork a new process to play in the terminal
  // For now, just playing in the same terminal
}

int harmonize::TextPlayer::Start(int pos = 0) {
  char c;
  while (file >> c) {
    if (status_ != PlayStatus::Playing) {
      if (status_ == PlayStatus::Paused) {
        std::unique_lock<std::mutex> lock(play_mutex_);
        play_cond_.wait(lock);
      }
      else {
        break; // Stopped player, exit
      }
    }
    cout << c;
    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTime));
  }
  return 0;
}

// Play Control: Set the player to play
void harmonize::TextPlayer::Play() {
  if (status_ != PlayStatus::Playing) {
    std::unique_lock<std::mutex> lock(play_mutex_);
    status_ = PlayStatus::Playing;
    play_cond_.notify_one();
  }
}

// Pause Control: Set the player to pause
void harmonize::TextPlayer::Pause() {
  if (status_ != PlayStatus::Paused) {
    std::unique_lock<std::mutex> lock(play_mutex_);
    status_ = PlayStatus::Paused;
  }
}

// Set the player to stop
void harmonize::TextPlayer::Stop() {
  if (status_ != PlayStatus::Stopped) {
    std::unique_lock<std::mutex> lock(play_mutex_);
    status_ = PlayStatus::Stopped;
  }
}

void harmonize::TexPlayer::PlayAt(int pos) {
  // TODO This should happen in a thread-safe manner
  file.seekg(pos);
}

playerpos GetPlayerPos() {
  return file.tellg();
}

// The new player thread starts from here
void harmonize::TextPlayer::operator()() {
  Play();
}
