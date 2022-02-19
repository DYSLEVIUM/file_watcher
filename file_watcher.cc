#pragma GCC optimize("Ofast,fast-math,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include "dyslevium.h"

int main(int argc, char* argv[]) {
  File_Watcher fw{"./", std::chrono::milliseconds(500)};

  fw.start([](std::string path_to_watch, FILE_STATUS status) -> void {
    if (!std::filesystem::is_regular_file(
            std::filesystem::path(path_to_watch)) &&
        status != FILE_STATUS::erased)
      return;

    switch (status) {
      case FILE_STATUS::created:
        std::cout << "File Created!" << std::endl;
        break;
      case FILE_STATUS::modified:
        std::cout << "File Modified!" << std::endl;
        break;
      case FILE_STATUS::erased:
        std::cout << "File Erased!" << std::endl;
        break;
      default:
        std::cout << "Error! Unknown file status." << std::endl;
        break;
    }
  });
}