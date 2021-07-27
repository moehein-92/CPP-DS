

#include <iostream>

#include "UnorderedMapCommon.h"



// =========================================================================
// EXERCISE 1: makeWordCounts
//
// makeWordCounts takes a vector of strings, which are in no particular order
// and which may contain duplicates. You need to:
//
// 1. Create a StringIntMap (which is a type of std::unordered_map defined in
//    UnorderedMapCommon.h) in order to do work on it. You should create this
//    on the stack, not the heap. (That is, do not use the "new" operator.)
//    You will ultimately want to return this StringIntMap from the function.
// 2. Look through the "words" vector that has been passed into the function,
//    using whatever form of iteration you choose. For each unique string,
//    a key should be created in the StringIntMap.
// 3. The StringIntMap type uses strings as keys, and these are mapped to int
//    values. You should count the number of occurences of each unique string
//    found in the input, and that word count should be the value you assign
//    to its corresponding key in the map.
//
// Example: If the input is a vector containing {"dog", "cat", "dog"}, then
// the map should have these mappings:
//  Key: "cat" maps to value: 1
//  Key: "dog" maps to value: 2
//
// You do not need to perform any string operations on the strings.
// For example, you do NOT need to change the strings to lowercase or parse
// the strings in any further way. You can handle each string exactly as
// it appears in the input.
//==========================================================================

// makeWordCounts: Given a vector of (non-unique) strings, returns a
// StringIntMap where each string is mapped to its number of occurences.
StringIntMap makeWordCounts(const StringVec& words) {
  StringIntMap wordcount_map;

  for (std::string wor : words){
    std::string key = wor;

    if (wordcount_map.count(key)){
      wordcount_map[key] += 1;
    }
    else{
      wordcount_map[key] = 1;
    }
  }
  return wordcount_map;
}



// =========================================================================
// EXERCISE 2: lookupWithFallback
//
// The lookupWithFallback function is a wrapper function for safely
// performing lookups on a read-only std::unordered_map object. (In some
// languages, there is a standard library function that behaves this way.)
//
// People commonly use the [] operator with maps to conveniently do both
// assignments and lookups, but the [] operator will insert a new key with a
// default value when the key is not found. Sometimes that is not desirable!
// (Also, if the map is marked "const", you will not be able to use the []
// operator on it, because the map is read-only.)
//
// Instead, there is also the .at() function which can look up a key or throw
// an exception if not found; or there is the .find() function which can
// search for a key and return an iterator signifying the result. There is
// also the strangely-named .count() function, which does not actually count
// beyond 1; it can only tell you if the key is in the map or not. So, there
// are several ways to look for a key, and several ways to deal with the case
// when it is not found.
//
// 1. Given the input parameters shown, you need to figure out if the key
//    exists in the wordcount_map or not.
// 2. If the key exists, return the mapped value.
// 3. If the key does not exist, then return the provided fallback value,
//    which is the fallbackVal argument.
//
// You should not modify the original wordcount_map object. (However, it is
// marked const, so you probably can't edit it anyway! If you try to use []
// here, you will probably get a compiler error.) Also, the grader will check
// that you did not edit the original map.
// =========================================================================

int lookupWithFallback(const StringIntMap& wordcount_map, const std::string& key, int fallbackVal) {

  auto it = wordcount_map.find(key);
  return it != wordcount_map.end() ? it->second : fallbackVal;
}



// =========================================================================
// EXERCISE 3: Memoizing a Function
//
// This exercise is mostly conceptual. There is not that much you need to do,
// but there is an important concept for you to understand. So, the description
// here is a bit longer than usual. Please be sure to read the instructions
// PDF for a clearer presentation of this information.
//
// -- Background information --
// In essence, you will use an unordered_map type as a hash table to cache
// certain values that would otherwise be frequently recalculated. This way,
// by reducing the number of calculations, you can make certain kinds of
// functions run MUCH faster.
//
// A "palindrome" is a word that remains the same if its spelling is reversed.
// For example, "dad" and "mom" are palindromes. In the provided code file
// UnorderedMapCommon.cpp, there is a definition of a function called
// "longestPalindromeLength", which is a very slow function. The purpose
// of that function is this: Given a string "str" and two indices, "leftLimit"
// and "rightLimit", it find the length of the longest palindrome substring
// found anywhere in str between the leftLimit and rightLimit characters.
//
// So, for example, given this string: "xyzwDADxyzw"
// and these limits: 0 and 10 (which are the first and last character indices),
// we can calculate that the longest palindrome length is 3, because "DAD"
// is the longest palindrome substring to be found.
//
// This calculation can be very slow because a naive program would re-check
// the same substrings many times, and indeed, longestPalindromeLength will
// run very slow on large input strings.
//
// -- How the calculations are memoized here --
// Below, there is an edited version of that function definition, which is
// called "memoizedLongestPalindromeLength". This version of the function
// takes an extra parameter, LengthMemo& memo, which is the "memoization
// table", that is, a hash table used for caching calculation results. The
// LengthMemo type is defined in UnorderedMapCommon.h like this:
//   using LengthMemo = std::unordered_map<IntPair, int>;
// Therefore, LengthMemo is an unordered_map where each key is an IntPair,
// and each mapped value is an int. The IntPair key is a pair of left and
// right index limits, and the mapped int value is the recorded calculation
// result.
//
// So for our "xyzwDADxyzw" example above, one entry in the map would be this:
// Key: the pair (0,10)
// Mapped value: 3
//
// -- Your task --
// In order to make use of the "memo" object for caching purposes, you need
// to edit the function below in two different places, which we have clearly
// marked with comments as "PART A" and "PART B". There are also other hints
// in the comments below, marked "EXAMPLE".
// =========================================================================

// memoizedLongestPalindromeLength:
// As described above, this is the memoized version of a recursive function
// for finding the maximum palindrome substring length.
// The startTime and maxDuration parameters are used by the grader to make
// sure your function doesn't accidentally run very slow.
int memoizedLongestPalindromeLength(LengthMemo& memo, const std::string& str, int leftLimit, int rightLimit, timeUnit startTime, double maxDuration) {

  if (leftLimit < 0 && leftLimit <= rightLimit) {
    throw std::runtime_error("leftLimit negative, but it's not the base case");
  }
  if (rightLimit < 0 && leftLimit <= rightLimit) {
    throw std::runtime_error("rightLimit negative, but it's not the base case");
  }

  if (false) {
    // Debugging spam messages
    int range = rightLimit-leftLimit+1;
    if (range < 0) range = 0;
    std::cout << "Considering substring: " << str.substr(leftLimit, range) << std::endl;
    std::cout << " because l/r limits are: " << leftLimit << " " << rightLimit << std::endl;
  }

  const auto currentTime = getTimeNow();
  const auto timeElapsed = getMilliDuration(startTime, currentTime);
  if (timeElapsed > maxDuration) {
    throw TooSlowException("taking too long");
  }

  // Here's the memoization key for this pair of limit integers.
  // IntPair is our type alias for std::pair<int, int>
  const IntPair pairKey = std::make_pair(leftLimit, rightLimit);

  if (memo.count(pairKey)) {

    auto it = memo.find(pairKey);

    return it->second; //it->second returns palindrome word length count.
  }


  if (leftLimit > rightLimit) {

    memo[pairKey] = 0;
    return 0;
  }

  if (leftLimit == rightLimit && str.at(leftLimit) == str.at(rightLimit)) {
    // Another O(1) return case that we'll memoize anyway for completeness.

    // ====================================================================
    // EXAMPLE: This base case has already been memoized!
    // (You DON'T need to edit this code section at all!
    //  Just pay attention to what's happening here!)
    memo[pairKey] = 1;
    return 1;
  }

  // If the first and last character match, then...
  if (str.at(leftLimit) == str.at(rightLimit)) {
    // move left limit to the right
    int newLeft = leftLimit+1;
    // move right limit to the left
    int newRight = rightLimit-1;

    // Solve the middle subproblem.
    int middleSubproblemResult = memoizedLongestPalindromeLength(memo, str, newLeft, newRight, startTime, maxDuration);

    int middleMaxLength = newRight-newLeft+1;

    if (middleMaxLength < 0) middleMaxLength = 0;

    // If the middle subproblem result equals the entire length
    // of the middle substring, then the middle substring is a palindrome.
    // So, since the first and last outer characters match each other,
    // the entire string between leftLimit and rightLimit is a palindrome.
    if (middleSubproblemResult == middleMaxLength) {
      int result = 2+middleSubproblemResult;

      memo[pairKey] = result;
      return result;
      // ==================================================================
    }

  }

  // If we've reached this line in the function, we know the entire
  // string between leftLimit and rightLimit, inclusive, is NOT a palindrome.
  // That means we still need to try moving in each of the left limit
  // and right limit, separately, and compare the results.

  // Move the right limit to the left and recurse.
  int leftSubproblemResult = memoizedLongestPalindromeLength(memo, str, leftLimit, rightLimit-1, startTime, maxDuration);
  // Move the left limit to the right and recurse.
  int rightSubproblemResult = memoizedLongestPalindromeLength(memo, str, leftLimit+1, rightLimit, startTime, maxDuration);
  // Return whichever result was greater.
  // We can also store this result for memoization purposes.
  int greaterResult = std::max(leftSubproblemResult,rightSubproblemResult);

  memo[pairKey] = greaterResult;

  return greaterResult;
}
