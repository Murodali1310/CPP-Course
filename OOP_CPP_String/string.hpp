#include <cstddef>
#include <cstring>
#include <iostream>
#include <vector>
#pragma once

class String {
 public:
  String();
  String(size_t size, char character);
  String(const char* str);
  String(const String& str);
  String& operator=(const String& str);
  ~String();
  void Clear();
  void PushBack(char character);

  void PopBack();
  void Resize(size_t new_size);

  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);

  void ShrinkToFit();

  void Swap(String& other);
  char& operator[](size_t index);
  const char& operator[](size_t index) const;
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;

  bool Empty() const;
  size_t Size() const;
  size_t Capacity() const;

  char* Data();
  const char* Data() const;
  friend bool operator<(const String& str1, const String& str2);
  friend bool operator!=(const String& str1, const String& str2);
  friend bool operator==(const String& str1, const String& str2);
  friend bool operator<=(const String& str1, const String& str2);
  friend bool operator>(const String& str1, const String& str2);
  friend bool operator>=(const String& str1, const String& str2);
  String& operator+=(const String& str2);
  String operator+(const String& str) const;
  friend String operator*(const String& str, size_t n);
  String operator*=(size_t n);
  friend std::istream& operator>>(std::istream& instream, String& str);
  friend std::ostream& operator<<(std::ostream& outstream, const String& str);

  std::vector<String> Split(const String& delim = " ");

  String Join(const std::vector<String>& strings) const;

 private:
  size_t size_;
  size_t capacity_;
  char* str_;
};