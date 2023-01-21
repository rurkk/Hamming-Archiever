#include "hamarc.h"
#include "hamming.h"

uint64_t FileSize(const std::string& path) {
  std::ifstream file(path, std::ios::binary);
  file.seekg(0, std::ios::end);
  uint64_t size;
  size = file.tellg();
  file.seekg(0, std::ios::beg);
  file.close();
  return size;
}

void InfoAddFiles(const std::vector<std::string>& files) {
  std::ofstream info_in("info", std::ios::binary);
  for (const std::basic_string<char>& file : files) {
    for (char c : file) {
      info_in.write((const char*) &c, 1);
    }
    info_in.write("|", 1);
    std::ifstream new_file(file, std::ios::binary);
    new_file.seekg(0, std::ios::end);
    std::string num_of_chars;
    num_of_chars = std::to_string(new_file.tellg());
    for (char c : num_of_chars) {
      info_in.write((const char*) &c, 1);
    }
    info_in.write("|", 1);
  }
  info_in.close();
}

void ArchiveFiles(const std::vector<std::string>& files, const std::string& path) {
  std::ofstream arch(path, std::ios::binary);
  InfoAddFiles(files);
  std::string info_size_char;
  info_size_char = std::to_string(FileSize("info"));
  std::ifstream info_out("info", std::ios::binary);
  info_size_char.insert(0, 18 - info_size_char.length(), '0');
  for (char c : info_size_char) {
    arch.write((const char*) &c, 1);
  }
  char c;
  while (info_out.get(c)) {
    arch.write((const char*) &c, 1);
  }
  info_out.close();
  remove("info");
  for (const std::string& file : files) {
    std::ifstream fin(file, std::ios::binary);
    while (fin.get(c)) {
      arch.write((const char*) &c, 1);
    }
    fin.close();
    remove(file.c_str());
  }
}

uint32_t InfoMaker(const std::string& path) {
  std::ofstream info_in("info", std::ios::binary);
  std::ifstream arch(path, std::ios::binary);
  char c;
  uint32_t num_of_chars;
  num_of_chars = 0;
  for (int i = 0; i < 18; i++) {
    arch.get(c);
    num_of_chars *= 10;
    num_of_chars += c - '0';
  }
  for (int i = 0; i < num_of_chars; i++) {
    arch.get(c);
    info_in.write((const char*) &c, 1);
  }
  arch.close();
  info_in.close();

  return num_of_chars;
}

Info InfoReader(const std::string& file_to_delete) {
  Info inf;
  std::ifstream info_out("info", std::ios::binary);
  char c;
  bool flag_num = false;
  bool flag_path = true;
  std::string another_file;
  std::string another_size;
  while (info_out.get(c)) {
    if (c == '|') {
      if (flag_path and another_file != file_to_delete) {
        inf.files.push_back(another_file);
      }
      if (flag_num) {
        inf.size_of_files.push_back(another_size);
      }
      std::swap(flag_num, flag_path);
      if (another_file == file_to_delete) {
        inf.num_of_file_to_delete = inf.files.size();
        std::swap(flag_num, flag_path);
        info_out.get(c);
        while (c != '|') {
          another_size.push_back(c);
          info_out.get(c);
        }
        inf.size_of_file_to_delete = std::stoi(another_size);
      }
      another_file = "";
      another_size = "";
    } else if (flag_path) {
      another_file.push_back(c);
    } else if (flag_num) {
      another_size.push_back(c);
    }
  }
  info_out.close();
  remove("info");

  return inf;
}

void DeleteFile(const std::string& file_to_delete, const std::string& path) {
  uint32_t num_of_chars = InfoMaker(path);
  Info inf;
  inf = InfoReader(file_to_delete);
  std::ofstream info_in("info", std::ios::binary);
  for (int i = 0; i < inf.files.size(); i++) {
    std::string file;
    file = inf.files[i];
    std::string num_of_chars_new;
    num_of_chars_new = inf.size_of_files[i];
    for (char j : file) {
      info_in.write((const char*) &j, 1);
    }
    info_in.write("|", 1);
    for (char j : num_of_chars_new) {
      info_in.write((const char*) &j, 1);
    }
    info_in.write("|", 1);
  }
  info_in.close();
  std::ofstream arch_new("arch_new", std::ios::binary);
  std::ifstream info_out_new("info", std::ios::binary);
  std::string info_size_char;
  info_size_char = std::to_string(FileSize("info"));
  info_size_char.insert(0, 18 - info_size_char.length(), '0');
  for (char i : info_size_char) {
    arch_new.write((const char*) &i, 1);
  }
  char c;
  while (info_out_new.get(c)) {
    arch_new.write((const char*) &c, 1);
  }
  info_out_new.close();
  remove("info");
  std::ifstream arch(path, std::ios::binary);
  arch.seekg(18 + num_of_chars, std::ios::cur);
  for (int i = 0; i < inf.files.size(); i++) {
    if (i == inf.num_of_file_to_delete) arch.seekg(inf.size_of_file_to_delete, std::ios::cur);
    for (uint64_t j = 0; j < std::stoi(inf.size_of_files[i]); j++) {
      arch.get(c);
      arch_new.write((const char*) &c, 1);
    }
  }
  arch.close();
  remove(path.c_str());
  arch_new.close();
  rename("arch_new", path.c_str());
}

void ExtractFile(const std::string& file_to_extract, const std::string& path) {
  uint32_t num_of_chars = InfoMaker(path);
  Info inf;
  inf = InfoReader(file_to_extract);
  std::ofstream file(file_to_extract, std::ios::binary);
  std::ifstream arch(path, std::ios::binary);
  char c;
  arch.seekg(18 + num_of_chars, std::ios::cur);
  for (int i = 0; i < inf.files.size(); i++) {
    if (i == inf.num_of_file_to_delete) {
      for (int j = 0; j < inf.size_of_file_to_delete; j++) {
        arch.get(c);
        file.write((const char*) &c, 1);
      }
    }
    arch.seekg(std::stoi(inf.size_of_files[i]), std::ios::cur);
  }
  arch.close();
  file.close();
}

std::vector<std::string> PrintList(const std::string& path) {
  InfoMaker(path);
  Info inf;
  inf = InfoReader("no file");

  return inf.files;
}

void AppendFile(const std::string& file_to_append, const std::string& path) {
  uint32_t num_of_chars = InfoMaker(path);
  std::ifstream arch(path, std::ios::binary);
  std::ifstream file(file_to_append, std::ios::binary);
  std::ofstream arch_new("arch_new", std::ios::binary);
  uint32_t file_size;
  file_size = FileSize(file_to_append);

  std::string info_size_char;
  info_size_char = std::to_string(FileSize("info") + file_size + 2 + file_to_append.length());
  std::ifstream info_out("info", std::ios::binary);
  info_size_char.insert(0, 18 - info_size_char.length(), '0');
  for (char c : info_size_char) {
    arch_new.write((const char*) &c, 1);
  }
  arch.seekg(18, std::ios::cur);
  char c;
  for (int i = 0; i < num_of_chars; i++) {
    arch.get(c);
    arch_new.write((const char*) &c, 1);
  }
  for (char i : file_to_append) {
    arch_new.write((const char*) &i, 1);
  }
  arch_new.write((const char*) &"|", 1);
  for (char i : std::to_string(file_size)) {
    arch_new.write((const char*) &i, 1);
  }
  arch_new.write((const char*) &"|", 1);
  while (arch.get(c)) {
    arch_new.write((const char*) &c, 1);
  }
  while (file.get(c)) {
    arch_new.write((const char*) &c, 1);
  }
  remove("info");
  file.close();
  remove(file_to_append.c_str());
  arch.close();
  remove(path.c_str());
  arch_new.close();
  rename("arch_new", path.c_str());
}

void ConcatenateArchs(const std::string& arch1_to_merge, const std::string& arch2_to_merge, const std::string& path) {
  uint32_t num_of_chars1 = InfoMaker(arch1_to_merge);
  rename("info", "info1");
  uint32_t num_of_chars2 = InfoMaker(arch2_to_merge);
  rename("info", "info2");
  std::ifstream arch1(arch1_to_merge, std::ios::binary);
  std::ifstream arch2(arch2_to_merge, std::ios::binary);
  std::ofstream arch_new(path, std::ios::binary);
  std::string info_size_char;
  info_size_char = std::to_string(num_of_chars1 + num_of_chars2);
  std::ifstream info_out("info", std::ios::binary);
  info_size_char.insert(0, 18 - info_size_char.length(), '0');
  for (char c : info_size_char) {
    arch_new.write((const char*) &c, 1);
  }
  char c;
  arch1.seekg(18);
  for (int i = 0; i < num_of_chars1; i++) {
    arch1.get(c);
    arch_new.write((const char*) &c, 1);
  }
  arch2.seekg(18);
  for (int i = 0; i < num_of_chars2; i++) {
    arch2.get(c);
    arch_new.write((const char*) &c, 1);
  }
  while (arch1.get(c)) {
    arch_new.write((const char*) &c, 1);
  }
  while (arch2.get(c)) {
    arch_new.write((const char*) &c, 1);
  }
  arch1.close();
  arch2.close();
  remove("info1");
  remove("info2");
  remove(arch1_to_merge.c_str());
  remove(arch2_to_merge.c_str());
}

void FileToHam(const std::string& path, uint64_t size_of_word) {
  std::ifstream file(path, std::ios::binary);
  std::ofstream file_new("file_new_dfghbvcftyhnmkhb", std::ios::binary);
  char c;
  std::vector<bool> bits;
  uint64_t cur_size_of_word;
  cur_size_of_word = 0;
  while (file.get(c)) {
    for (int i = 7; i >= 0; --i) {
      bits.push_back((c >> i) & 1);
      cur_size_of_word++;
    }
    if (cur_size_of_word >= size_of_word) {
      std::vector<bool> ham_bits;
      for (int i = 0; i < size_of_word; i++) {
        ham_bits.push_back(bits[0]);
        bits.erase(bits.begin());
      }
      ham_bits = VecToHam(ham_bits);
      std::vector<unsigned char> chars;
      chars = FromBitsToChars(ham_bits);
      for (unsigned char i : chars) {
        file_new.write((const char*) &i, 1);
      }
      cur_size_of_word -= size_of_word;
    }
  }
  if (cur_size_of_word > 0) {
    while (bits.size() < size_of_word) {
      bits.insert(bits.begin(), false);
    }
    bits = VecToHam(bits);
    std::vector<unsigned char> chars;
    chars = FromBitsToChars(bits);
    for (unsigned char i : chars) {
      file_new.write((const char*) &i, 1);
    }
  }
  file.close();
  remove(path.c_str());
  file_new.close();
  rename("file_new_dfghbvcftyhnmkhb", path.c_str());
}

void FileFromHam(const std::string& path, uint64_t size_of_word) {
  std::ifstream file(path, std::ios::binary);
  std::ofstream file_new("file_new_dfsdfgghmnmkhb", std::ios::binary);

  uint64_t degree_of_two;
  degree_of_two = 1;
  uint64_t ind;
  ind = 0;
  uint64_t new_ind;
  new_ind = 0;
  while (ind < size_of_word) {
    if (degree_of_two - 1 == new_ind) {
      degree_of_two *= 2;
      new_ind++;
    } else {
      new_ind++;
      ind++;
    }
  }
  size_of_word = new_ind + 1;
  std::vector<bool> ham_bits;
  uint64_t cur_size_of_word;
  cur_size_of_word = 0;
  char c;
  std::vector<bool> all_bits;
  while (file.get(c)) {
    if (cur_size_of_word + 8 < size_of_word) {
      for (int i = 7; i >= 0; --i) {
        ham_bits.push_back((c >> i) & 1);
        cur_size_of_word++;
      }
    } else {
      for (int i = size_of_word - cur_size_of_word - 1; i >= 0; --i) {
        ham_bits.push_back((c >> i) & 1);
        cur_size_of_word++;
      }
    }
    if (cur_size_of_word == size_of_word) {
      std::vector<bool> bits;
      bits = VecFromHam(ham_bits);
      for (bool bit : bits) all_bits.push_back(bit);
      cur_size_of_word -= size_of_word;
      ham_bits.clear();
    }
    if (all_bits.size() >= 8) {
      std::vector<unsigned char> chars;
      uint64_t ssize;
      ssize = all_bits.size() - all_bits.size() % 8;
      std::vector<bool> bits_to_chars;
      for (int i = 0; i < ssize; i++) {
        bits_to_chars.push_back(all_bits[0]);
        all_bits.erase(all_bits.begin());
      }
      chars = FromBitsToChars(bits_to_chars);
      for (unsigned char i : chars) file_new.write((const char*) &i, 1);
    }
  }
  if (!all_bits.empty()) {
    std::vector<unsigned char> chars;
    chars = FromBitsToChars(all_bits);
    for (unsigned char i : chars) file_new.write((const char*) &i, 1);
  }
  file.close();
  remove(path.c_str());
  file_new.close();
  rename("file_new_dfsdfgghmnmkhb", path.c_str());
}