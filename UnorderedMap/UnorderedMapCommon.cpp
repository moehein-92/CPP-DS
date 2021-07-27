#include <stdexcept> // for std::runtime_error
#include <iostream> // for std::cerr, std::cout
#include <fstream> // for std::fstream
#include <string> // for std::string
#include <vector> // for std::vector
#include <cctype> // std::tolower
#include <algorithm> // for std::sorts
#include <regex> // for std::regex

#include "UnorderedMapCommon.h"

StringVec loadBookStrings(unsigned int min_word_length) {

  static const std::string filename = "through_the_looking_glass.txt";
  static const std::string start_text = "CHAPTER I";
  static const std::string end_text = "End of the Project Gutenberg EBook";
  constexpr bool DEBUGGING = false;
  constexpr int DEBUGGING_MAX_WORDS = 30;

  std::ifstream instream(filename);
  if (!instream) {
    instream.close();
    throw std::runtime_error("Could not load book file: " + filename);
  }

  // Prepare a vector of strings.
  StringVec bookstrings;

  // Skipping the introduction section of the book file. This fast-forwards
  // through the file until the start_text string is found.
  {
    // Read lines of the input one at a time, as a temporary copy string "line".
    std::string line;
    while (std::getline(instream, line)) {
      if (line.find(start_text) != std::string::npos) {
        break;
      }
    }
  }

  // We will read through the main body of the book one line at a time.
  std::string line;
  bool stillReading = true;
  while (stillReading && std::getline(instream, line)) {

    // If this line contains the ending of the book text,
    // then stop so we don't read the legal disclaimers.
    if (line.find(end_text) != std::string::npos) {
      stillReading = false;
      break;
    }

    while (line.find("’") != std::string::npos) {
      line = std::regex_replace(line, std::regex("’"), "'");
    }

    while (line.find("--") != std::string::npos) {
      line = std::regex_replace(line, std::regex("--"), " ");
    }

    while (stillReading && line.length() > 0) {
      // Keep only letters and convert to lower case.
      std::string trimmed_word = "";
      int consumedChars = 0;
      char prevChar = ' ';
      for (char c : line) {
        consumedChars++;
        if (std::isalpha(c)) {
          char thisChar = std::tolower(c);
          if (prevChar == '\'') {

            trimmed_word += "'";
          }
          else if (prevChar == '-') {

            trimmed_word += "-";
          }
          trimmed_word += thisChar;
          prevChar = thisChar;
        }
        else if ('\'' == c) {

          if (std::isalpha(prevChar)) {
            prevChar = '\'';
          }
          else {
            prevChar = ' ';
          }
        }
        else if ('-' == c) {

          if (std::isalpha(prevChar)) {
            prevChar = '-';
          }
          else {
            prevChar = ' ';
          }
        }
        else break;
      }

      line = line.substr(consumedChars);

      if (trimmed_word.length() >= min_word_length) {
        bookstrings.push_back(trimmed_word);
      }

      if (DEBUGGING && bookstrings.size() >= DEBUGGING_MAX_WORDS) {
        stillReading = false;
        break;
      }
    }
  }

  return bookstrings;
}

// This helper function can be used to sort records of word counts
// based on the count. This is used in sortWordCounts.
bool wordCountComparator(const StringIntPair& x, const StringIntPair& y) {
  return x.second < y.second;
}

// sortWordCounts produces a fresh vector containing sorted copies
// of the word count records from wordcount_map.
StringIntPairVec sortWordCounts(const StringIntMap& wordcount_map) {
  // Copy all the wordcount entries from the map into a vector,
  // then sort the vector with our custom comparator.
  StringIntPairVec wordcount_vec;
  for (const StringIntPair& wc : wordcount_map) {
    wordcount_vec.push_back(wc);
  }
  std::sort(wordcount_vec.begin(), wordcount_vec.end(), wordCountComparator);
  return wordcount_vec;
}

// Makes a list (actually, std::vector) of the least common words found in
// the book. As input, it takes the result of sortWordCounts.
StringIntPairVec getBottomWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words) {
  StringIntPairVec bottom_wordcounts;
  for (const auto& wordcount_item : sorted_wordcounts) {
    if (bottom_wordcounts.size() < max_words) {
      bottom_wordcounts.push_back(wordcount_item);
    }
    else break;
  }
  return bottom_wordcounts;
}

// Makes a list (actually, std::vector) of the most common words found in
// the book. As input, it takes the result of sortWordCounts.
StringIntPairVec getTopWordCounts(const StringIntPairVec& sorted_wordcounts, unsigned int max_words) {
  StringIntPairVec top_wordcounts;
  for (auto rit = sorted_wordcounts.rbegin(); rit != sorted_wordcounts.rend(); rit++) {
    if (top_wordcounts.size() < max_words) {
      const auto& wordcount_item = *rit;
      top_wordcounts.push_back(wordcount_item);
    }
    else break;
  }
  return top_wordcounts;
}

int longestPalindromeLength(const std::string& str, int leftLimit, int rightLimit, timeUnit startTime, double maxDuration) {

  if (leftLimit > rightLimit) {
    return 0;
  }
  if (leftLimit < 0) {
    throw std::runtime_error("leftLimit negative");
  }
  if (rightLimit < 0) {
    throw std::runtime_error("rightLimit negative, but it's not the base case");
  }
  if (false) {
    // Debugging messages
    int range = rightLimit-leftLimit+1;
    if (range < 0) range = 0;
    std::cout << "Considering substring: " << str.substr(leftLimit, range) << std::endl;
    std::cout << " because l/r limits are: " << leftLimit << " " << rightLimit << std::endl;
  }

  // Some examples may take an absurdly long time to calculate with brute force.
  const auto currentTime = getTimeNow();
  const auto timeElapsed = getMilliDuration(startTime, currentTime);
  if (timeElapsed > maxDuration) {
    throw TooSlowException("taking too long");
  }

  // A single-character substring is a palindrome of size 1.
  // We include the character check with .at() to make sure the string isn't
  // empty and the indices are valid.
  if (leftLimit == rightLimit && str.at(leftLimit) == str.at(rightLimit)) {
    return 1;
  }

  // If the first and last character match, then...
  if (str.at(leftLimit) == str.at(rightLimit)) {
    // move left limit to the right
    int newLeft = leftLimit+1;
    // move right limit to the left
    int newRight = rightLimit-1;

    // Solve the middle subproblem.
    int middleSubproblemResult = longestPalindromeLength(str, newLeft, newRight, startTime, maxDuration);

    // (Base case note: Suppose that str had length 2, so after moving the indices,
    //  now newLeft > newRight. Because we handled the crossing case already,
    //  in that situation, middleSubproblemResult correctly gets value 0.)

    // For reference, let's calculate the longest length possible in the
    // middle substring range.
    int middleMaxLength = newRight-newLeft+1;
    // In the base case situation when the indices cross,
    // we force this value to be 0 instead of negative.
    if (middleMaxLength < 0) middleMaxLength = 0;

    if (middleSubproblemResult == middleMaxLength) {
      return 2+middleSubproblemResult;
    }
  }
  // Move the right limit to the left.
  int leftSubproblemResult = longestPalindromeLength(str, leftLimit, rightLimit-1, startTime, maxDuration);
  // Move the left limit to the right.
  int rightSubproblemResult = longestPalindromeLength(str, leftLimit+1, rightLimit, startTime, maxDuration);
  // Return whichever result was greater.
  return std::max(leftSubproblemResult,rightSubproblemResult);
}

// -------------------------------------------------------------------------
// memoizedLongestPalindromeLength: Can be found in UnorderedMapExercises.cpp
// -------------------------------------------------------------------------

// reconstructPalindrome returns a copy of the longest palindrome within str,
// based on the information provided by the memoization table that has been
// previously calculated by memoizedLongestPalindromeLength.
std::string reconstructPalindrome(const LengthMemo& memo, const std::string& str) {

  // If the input string is empty, just return an empty string.
  if (!str.length()) return "";

  // Begin with the limits on the first and last characters.
  int left = 0;
  int right = str.length()-1;

  LengthMemo edit_memo = memo;

  // to end. So we begin with that best-ever value for comparison below.
  const int BEST_LEN = edit_memo[std::make_pair(left,right)];

  bool loop_again = true;
  while (loop_again) {
    // assume it's the final loop
    loop_again = false;

    int test_left, test_right;

    // try moving left limit
    test_left = left+1;
    test_right = right;
    if (test_left <= test_right) {
      int test_len = edit_memo[std::make_pair(test_left,test_right)];
      if (test_len == BEST_LEN) {
        left = test_left;
        loop_again = true;
      }
    }

    // try moving right limit
    test_left = left;
    test_right = right-1;
    if (test_left <= test_right) {
      int test_len = edit_memo[std::make_pair(test_left,test_right)];
      if (test_len == BEST_LEN) {
        right = test_right;
        loop_again = true;
      }
    }
  }

  if (left <= right) {
    return str.substr(left, right-left+1);
  }
  else {
    return "";
  }
}
