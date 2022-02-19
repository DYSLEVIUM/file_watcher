#pragma once

#include "dyslevium.h"

enum class FILE_STATUS { created, modified, erased };

class File_Watcher {
 public:
  std::string _path_to_watch;

  //  Time interval at which we check the base folder for changes
  std::chrono::duration<int, std::milli> _delay;

  File_Watcher(std::string path_to_watch,
               std::chrono::duration<int, std::milli> delay)
      : _path_to_watch(path_to_watch), _delay(delay) {
    for (auto& file :
         std::filesystem::recursive_directory_iterator(_path_to_watch)) {
      paths_[file.path().string()] = std::filesystem::last_write_time(file);
    }
  }

  void start(const std::function<void(std::string, FILE_STATUS)>& action) {
    while (running_) {
      std::this_thread::sleep_for(_delay);

      //  erased
      auto it = paths_.begin();
      while (it != paths_.end()) {
        if (!std::filesystem::exists(it->first)) {
          action(it->first, FILE_STATUS::erased);
          it = paths_.erase(it);
        } else
          ++it;
      }

      //  created
      for (auto& file :
           std::filesystem::recursive_directory_iterator(_path_to_watch)) {
        auto current_file_last_write_time =
            std::filesystem::last_write_time(file);
        auto file_path_string = file.path().string();
        if (!paths_.contains(file_path_string)) {
          paths_[file_path_string] = current_file_last_write_time;
          action(file_path_string, FILE_STATUS::created);
        }
      }

      //  modified
      for (auto& file :
           std::filesystem::recursive_directory_iterator(_path_to_watch)) {
        auto current_file_last_write_time =
            std::filesystem::last_write_time(file);
        auto file_path_string = file.path().string();
        if (paths_[file_path_string] != current_file_last_write_time) {
          paths_[file_path_string] = current_file_last_write_time;
          action(file_path_string, FILE_STATUS::modified);
        }
      }
    }
  }

 private:
  bool running_ = true;
  std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
};