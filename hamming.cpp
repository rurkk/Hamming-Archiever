#include "hamming.h"
#include "hamarc.h"

std::vector<bool> VecFromHam(std::vector<bool> ham_bits) {
  std::vector<bool> bits;
  uint64_t degree_of_two;
  degree_of_two = 1;
  for (int i = 0; i < ham_bits.size() - 1; i++) {
    if (i == degree_of_two - 1) degree_of_two *= 2;
    else bits.push_back(ham_bits[i]);
  }
  bits = VecToHam(bits);
  degree_of_two = 1;
  uint64_t num_of_mist;
  num_of_mist = 0;
  while (degree_of_two - 1 < bits.size() - 1) {
    if (bits[degree_of_two - 1] != ham_bits[degree_of_two - 1]) {
      num_of_mist += degree_of_two;
    }
    degree_of_two *= 2;
  }
  if (num_of_mist != 0) {
    if (ham_bits[num_of_mist - 1] == 1) ham_bits[num_of_mist - 1] = false;
    else ham_bits[num_of_mist - 1] = true;
  }
  if (bits[bits.size() - 1] != ham_bits[ham_bits.size() - 1]) {
    std::cout << "hamarc can not be decode!";
  } else {
    bits.clear();
    degree_of_two = 1;
    for (int i = 0; i < ham_bits.size() - 1; i++) {
      if (i == degree_of_two - 1) {
        degree_of_two *= 2;
      } else {
        bits.push_back(ham_bits[i]);
      }
    }
  }

  return bits;
}

std::vector<bool> VecToHam(std::vector<bool> bits) {
  std::vector<bool> ham_bits;
  uint64_t degree_of_two;
  degree_of_two = 1;
  uint64_t ind;
  ind = 0;
  uint64_t new_ind;
  new_ind = 0;
  while (ind < bits.size()) {
    if (degree_of_two - 1 == new_ind) {
      ham_bits.push_back(false);
      degree_of_two *= 2;
      new_ind++;
    } else {
      ham_bits.push_back(bits[ind]);
      new_ind++;
      ind++;
    }
  }
  degree_of_two = 1;
  while (degree_of_two - 1 < ham_bits.size()) {
    int control_sum;
    control_sum = 0;
    for (uint64_t i = degree_of_two - 1; i < ham_bits.size(); i += degree_of_two * 2) {
      for (uint64_t j = 0; j < degree_of_two; j++) {
        if (i + j >= ham_bits.size()) {
          break;
        }
        control_sum = (control_sum + ham_bits[std::min(i + j, (uint64_t) ham_bits.size())]) % 2;
      }
    }
    ham_bits[degree_of_two - 1] = control_sum;
    degree_of_two *= 2;
  }
  int bit_of_chet;
  bit_of_chet = 0;
  for (bool bit : ham_bits) bit_of_chet = (bit_of_chet + bit) % 2;
  ham_bits.push_back(bit_of_chet);

  return ham_bits;
}

std::vector<unsigned char> FromBitsToChars(std::vector<bool> bits) {
  std::vector<unsigned char> chars;
  while (!bits.empty()) {
    uint64_t bs = bits.size();
    unsigned char new_c;
    new_c = 0;
    uint32_t degree_of_two;
    degree_of_two = pow(2, (uint32_t) std::min((uint64_t) 7, bs - 1));
    for (int j = 0; j < std::min((uint64_t) 8, bs); j++) {
      new_c += bits[0] * degree_of_two;
      degree_of_two /= 2;
      bits.erase(bits.begin());
    }
    chars.push_back(new_c);
  }

  return chars;
}