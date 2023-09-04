#include "string.hpp"

String::String() {
  str_ = new char[1];
  str_[0] = '\0';
  size_ = 0;
  capacity_ = 0;
}
String::String(size_t size, char character) {
  size_ = size;
  str_ = new char[size_ + 1];
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = character;
  }
  str_[size_] = '\0';
  capacity_ = size_;
}

String::String(const char* str) {
  size_ = strlen(str);
  capacity_ = size_;
  str_ = new char[capacity_ + 1];
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = str[i];
  }
  str_[size_] = '\0';
}

void String::Clear() { size_ = 0; }

void String::PushBack(char character) {
  if (size_ == capacity_) {
    Reserve(2 * capacity_ + 1);
  }
  str_[size_] = character;
  str_[size_ + 1] = '\0';
  ++size_;
}

void String::PopBack() {
  if (size_ != 0) {
    --size_;
  }
}

void String::Resize(size_t new_size) {
  size_ = new_size;
  if (new_size > capacity_) {
    Reserve(new_size);
  }
}

void String::Resize(size_t new_size, char character) {
  size_t size = size_;
  Resize(new_size);
  memset(str_ + size, character, new_size - size);
  str_[size_] = '\0';
}

void String::Reserve(size_t new_cap) {
  if (new_cap > capacity_) {
    char* dop_s = new char[new_cap + 1];
    strcpy(dop_s, str_);
    delete[] str_;
    str_ = dop_s;
    capacity_ = new_cap;
  }
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    capacity_ = size_;
    char* dop_s = new char[size_ + 1];
    strcpy(dop_s, str_);
    delete[] str_;
    str_ = dop_s;
  }
}

void String::Swap(String& other) { std::swap(*this, other); }

char& String::operator[](size_t index) { return str_[index]; }

const char& String::operator[](size_t index) const { return str_[index]; }

char& String::Front() { return str_[0]; }

const char& String::Front() const { return str_[0]; }

char& String::Back() { return str_[size_ - 1]; }

const char& String::Back() const { return str_[size_ - 1]; }

bool String::Empty() const { return size_ == 0; }

char* String::Data() { return str_; }

const char* String::Data() const { return str_; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

bool operator<(const String& str1, const String& str2) {
  return strcmp(str1.Data(), str2.Data()) < 0;
}

bool operator!=(const String& str1, const String& str2) {
  return strcmp(str1.Data(), str2.Data()) != 0;
}

bool operator>=(const String& str1, const String& str2) {
  return !(str1 < str2);
}

bool operator==(const String& str1, const String& str2) {
  return !(str1 != str2);
}

bool operator<=(const String& str1, const String& str2) {
  return (str1 < str2) || (str1 == str2);
}

bool operator>(const String& str1, const String& str2) {
  return !(str1 <= str2);
}

String& String::operator+=(const String& str2) {
  if (capacity_ < size_ + str2.Size()) {
    Reserve(std::max(Capacity() + str2.Size(), 2 * capacity_));
  }
  str_[size_] = '\0';
  strcat(str_, str2.Data());
  size_ += str2.Size();
  return *this;
}

String String::operator+(const String& str) const {
  String new_str;
  new_str += *this;
  new_str += str;
  return new_str;
}

String operator*(const String& str, size_t n) {
  if (n != 0) {
    String res = str;
    res.Reserve(n * res.Size());
    for (size_t i = 0; i < n - 1; ++i) {
      for (size_t j = 0; j < str.Size(); ++j) {
        res.PushBack(str.Data()[j]);
      }
    }
    return res;
  }
  return {};
}

String String::operator*=(size_t n) {
  *this = *this * n;
  return *this;
}

std::istream& operator>>(std::istream& instream, String& str) {
  char elem;
  while (instream >> elem) {
    str.PushBack(elem);
  }
  return instream;
}

std::ostream& operator<<(std::ostream& outstream, const String& str) {
  outstream << str.Data();
  return outstream;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> strings;
  char* str = str_;
  char* res = new char[Size()];
  char* dop_s = strstr(str, delim.Data());
  while (dop_s != nullptr) {
    for (int i = 0; i < dop_s - str; ++i) {
      res[i] = str[i];
    }
    res[dop_s - str] = '\0';
    strings.emplace_back(res);
    str = dop_s + delim.Size();
    dop_s = strstr(str, delim.Data());
  }
  strings.emplace_back(str);
  delete[] res;
  return strings;
}

String String::Join(const std::vector<String>& strings) const {
  if (strings.empty()) {
    return {};
  }
  String str1 = strings[0];
  for (size_t i = 1; i < strings.size(); ++i) {
    str1 += (*this + strings[i]);
  }
  return str1;
}

String::String(const String& str) {
  if (&str == this) {
    return;
  }
  size_ = str.Size();
  capacity_ = str.Capacity();
  str_ = new char[str.Capacity() + 1];
  strcpy(str_, str.Data());
}

String& String::operator=(const String& str) {
  if (&str == this) {
    return *this;
  }
  delete[] str_;
  size_ = str.Size();
  capacity_ = str.Capacity();
  str_ = new char[str.Capacity() + 1];
  strcpy(str_, str.Data());
  return *this;
}
String::~String() { delete[] str_; }
