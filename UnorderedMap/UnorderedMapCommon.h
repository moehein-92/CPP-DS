
#pragma once

#include <string> // for std::string
#include <vector> // for std::vector
#include <utility> // for std::pair
#include <unordered_map> // for std::unordered_map
#include <chrono> // for std::chrono::high_resolution_clock

// References for chrono library usage:
// https://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now
// https://en.cppreference.com/w/cpp/chrono/duration/duration_cast
using timeUnit = std::chrono::time_point<std::chrono::high_resolution_clock>;
static inline timeUnit getTimeNow() noexcept {
  return std::chrono::high_resolution_clock::now();
}
template <typename T>
static double getMilliDuration(T start_time, T stop_time) {
  std::chrono::duration<double, std::milli> dur_ms = stop_time - start_time;
  return dur_ms.count();
}

using StringVec = std::vector<std::string>;
using StringIntPair = std::pair<std::string, int>;
using StringIntMap = std::unordered_map<std::string, int>;
using StringIntPairVec = std::vector<StringIntPair>;

#include "IntPair.h" // for IntPair definition as well as custom hashing support

using LengthMemo = std::unordered_map<IntPair, int>;

StringVec loadBookStrings(unsigned int min_word_length=5);

bool wordCountComparator(const StringIntPair& x, const StringIntPair& y);

StringIntPairVec sortWordCounts(const StringIntMap& wordcount_map);

StringIntMap makeWordCounts(const StringVec& words);

int lookupWithFallback(const StringIntMap& wordcount_map, const std::string& key, int fallbackVal);

StringIntPairVec getBottomWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words=20);

StringIntPairVec getTopWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words=20);

int longestPalindromeLength(const std::string& str, int leftLimit, int rightLimit, timeUnit startTime, double maxDuration);

int memoizedLongestPalindromeLength(LengthMemo& memo, const std::string& str, int leftLimit, int rightLimit, timeUnit startTime, double maxDuration);

std::string reconstructPalindrome(const LengthMemo& memo, const std::string& str);

class TooSlowException : public std::runtime_error {
public:
  // import constructor from the base class
  using std::runtime_error::runtime_error;
};
