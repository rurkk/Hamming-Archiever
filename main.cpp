#include "hamarc.h"
#include "hamming.h"

int main(int argc, const char* argv[]) {
  if (argc > 1) {
    int i = 1;
    while (i < argc) {
      std::string archname;
      std::vector<std::string> files;
      if (strcmp(argv[i], "-с") == 0 or strcmp(argv[i], "--create") == 0) {
        i++;
        if (strcmp(argv[i], "-f") == 0) {
          i++;
          archname = argv[i];
        } else archname = ((std::string) argv[i]).substr(7, strlen((argv[i])) - 7);
        i++;
        while (argv[i][0] != '-') {
          files.emplace_back(argv[i]);
          i++;
        }
        if (!files.empty()) {
          ArchiveFiles(files, archname);
        } else std::cout << "No files to archive them to " << archname;
      } else if (strcmp(argv[i], "-l") == 0 or strcmp(argv[i], "--list") == 0) {
        i++;
        if (strcmp(argv[i], "-f") == 0) {
          i++;
          archname = argv[i];
        } else archname = ((std::string) argv[i]).substr(7, strlen(argv[i]) - 7);
        i++;
        std::vector<std::string> filess;
        filess = PrintList(archname);
        for (std::string& file : filess) {
          std::cout << file << '\n';
        }
      } else if (strcmp(argv[i], "-x") == 0 or strcmp(argv[i], "--extract") == 0) {
        i++;
        if (strcmp(argv[i], "-f") == 0) {
          i++;
          archname = argv[i];
        } else archname = ((std::string) argv[i]).substr(7, strlen(argv[i]) - 7);
        i++;
        while (argv[i][0] != '-') {
          files.emplace_back(argv[i]);
          i++;
        }
        if (files.empty()) {
          std::vector<std::string> filess;
          filess = PrintList(archname);
          for (std::string& file : filess) {
            ExtractFile(file, archname);
          }
        } else {
          for (std::string& file : files) {
            ExtractFile(file, archname);
          }
        }
      } else if (strcmp(argv[i], "-a") == 0 or strcmp(argv[i], "--append") == 0) {
        i++;
        if (strcmp(argv[i], "-f") == 0) {
          i++;
          archname = argv[i];
        } else archname = ((std::string) argv[i]).substr(7, strlen(argv[i]) - 7);
        i++;
        AppendFile(argv[i], archname);
        i++;
      } else if (strcmp(argv[i], "-d") == 0 or strcmp(argv[i], "--delete") == 0) {
        i++;
        if (strcmp(argv[i], "-f") == 0) {
          i++;
          archname = argv[i];
        } else archname = ((std::string) argv[i]).substr(7, strlen(argv[i]) - 7);
        i++;
        DeleteFile(argv[i], archname);
        i++;
      } else if (strcmp(argv[i], "-A") == 0 or strcmp(argv[i], "--concatenate") == 0) {
        i++;
        std::string archname1;
        archname1 = argv[i];
        i++;
        std::string archname2;
        archname2 = argv[i];
        i++;
        if (strcmp(argv[i], "-f") == 0) {
          i++;
          archname = argv[i];
        } else archname = ((std::string) argv[i]).substr(7, strlen(argv[i]) - 7);
        i++;
        ConcatenateArchs(archname1, archname2, archname);
      } else if (strcmp(argv[i], "-C") == 0 or strcmp(argv[i], "--toham") == 0) {
        i++;
        FileToHam(argv[i], stoi(std::string(argv[i + 1])) * 8);
        i += 2;
      } else if (strcmp(argv[i], "-D") == 0 or strcmp(argv[i], "--fromham") == 0) {
        i++;
        FileFromHam(argv[i], stoi(std::string(argv[i + 1])) * 8);
        i += 2;
      }
    }
  }

  return 0;
}