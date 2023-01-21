#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <clocale>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <valarray>

struct Info {
  std::vector<std::string> files;
  std::vector<std::string> size_of_files;
  uint32_t num_of_file_to_delete;
  uint32_t size_of_file_to_delete;
};

void ArchiveFiles(const std::vector<std::string>& files, const std::string& path);

void InfoAddFiles(const std::vector<std::string>& files);

uint32_t InfoMaker(const std::string& path);

Info InfoReader(const std::string& file_to_delete);

void DeleteFile(const std::string& file, const std::string& path);

void ExtractFile(const std::string& file_to_extract, const std::string& path);

std::vector<std::string> PrintList(const std::string& path);

void AppendFile(const std::string& file_to_append, const std::string& path);

void ConcatenateArchs(const std::string& arch1_to_merge, const std::string& arch2_to_merge, const std::string& path);

void FileToHam(const std::string& path, uint64_t size_of_word);

void FileFromHam(const std::string& path, uint64_t size_of_word);

std::vector<bool> VecToHam(std::vector<bool> bits);

std::vector<unsigned char> FromBitsToChars(std::vector<bool> bits);

std::vector<bool> VecFromHam(std::vector<bool> ham_bits);