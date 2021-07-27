
#pragma once

#include <iostream>
#include <string>

#include "LinkedList.h"

template <typename T>
void LinkedList<T>::insertOrdered(const T& newData) {

  Node * new_Node = new Node(newData);

  if (size_ == 0) {
    head_ = new_Node;
    tail_ = new_Node;
    size_++;
  }
  else if (size_ == 1) {
      if (newData <= head_->data) {
        pushFront(newData);
      }
      else if (newData > head_->data) {
        pushBack(newData);
      }
    }
  else if (size_ > 1) {
      if (newData <= head_->data) {
        pushFront(newData);
      }
      else if (newData > tail_->data) {
        pushBack(newData);
      }
      else {
         Node* thru = head_;
         while (thru->next) {
           if (newData > thru->data){
             thru = thru->next;}
           else {
             new_Node->prev = thru->prev;
             new_Node->next = thru;
             thru->prev->next = new_Node;
             break;}
           }
           size_++;
         }
       }
}




  // Hints:
  // Make your new node on the heap and then find where it needs to
  // go in the list. A good way to do this is by considering special
  // base cases first, then walk the list from front to back and find
  // the earliest position where you should insert the new node.

  // When you insert the node, make sure to update any and all pointers
  // between it and adjacent nodes accordingly (next and prev pointers).
  // You may also need to update the head_ and tail_ pointers in some
  // cases. Also update the size_ variable of the list.

  // There are explicit examples of how to do all those things in the
  // other provided code for this project!

  // More hints:

  // First, practice your technique for traversing the list from front
  // to back. You can see examples of several ways to do this throughout
  // the provided code for this project. We recommend that you try using
  // a temporary pointer that you update to track your position as you
  // traverse from node to node.

  // Consider all the cases that can happen when you're trying to insert
  // the new node. Is the list currently empty? Does the new node go
  // at the beginning? Does it go somewhere in the middle? Does it go
  // at the end? Remember that this is a doubly-linked list, so there
  // may be prev and next pointers to adjust on both sides of the node
  // that you insert.

  // Be careful to make your conditional cases mutually exclusive when
  // necessary. Are you using "else" where you should?

  // Don't dereference a null pointer, ever! Always make sure the logic
  // checks for that before dereferencing. This is the most common cause
  // of crashes. The test program will show you which test cases are
  // causing you issues with that. A common issue is that when students
  // traverse an empty list or when they traverse to the end of the list,
  // they don't handle the null pointer at the tail properly. Be careful
  // to update all next, prev, head_, and tail_ pointers as needed on your
  // new node or on those existing nodes that are adjacent to the new node.

/********************************************************************
  Exercise 2: Linear-time Merge

  This LinkedList member function is intended to perform the classic
  "merge" operation from the mergesort algorithm. It combines two sorted
  lists into a single sorted list. This algorithm is intended to run
  in linear time (that is, O(n) where n is the total number of elements
  in the input lists), so it is not appropriate to simply concatenate
  the two lists and then apply a sorting algorithm. Instead, the merge
  algorithm relies on the fact that the two input lists are already sorted
  separately in order to create the merged, sorted list in linear time.

  One of the implied input lists is the "*this" LinkedList instance that
  is calling the function, and the other input list is explicitly specified
  as the function argument "other". The function does NOT change either
  of the original lists directly, as the inputs are marked const.
  Instead, this function makes a new list containing the merged result,
  and it returns a copy of the new list. For example, one usage might
  look like this (OUTSIDE of this function, where we are making the call):

  LinkedList<int> leftList;
  // [... Add some sorted contents to leftList here. ...]
  LinkedList<int> rightList;
  // [... Add some sorted contents to rightList here. ...]
  LinkedList<int> mergedList = leftList.merge(rightList);

  You may assume that the two input lists have already been sorted.
  However, the lists may be empty, and they may contain repeated or
  overlapping elements. The lists may also have different lengths.
  For example, it's possible that these are the two input lists:

  Left list: [1, 2, 2, 3, 5, 5, 5, 6]
  Right list: [1, 3, 5, 7]

  And the result of merging those two sorted lists will contain all
  of the same elements, including the correct number of any duplicates,
  in sorted order:
  [1, 1, 2, 2, 3, 3, 5, 5, 5, 5, 6, 7]

  Because your implementation of this function operates on const inputs
  and returns a newly created list, you do not need to maintain the
  previous memory locations of any nodes as required in Exercise 1.
  You may need to make non-const "working copies" of the const input lists
  if you wish. You may approach this problem either iteratively or
  recursively, and you may use the member functions of the LinkedList class
  to make it simpler (such as push and pop), or you may edit the contents
  of lists explicitly by changing the pointers of a list or of its nodes
  (such as head_, tail_, next, and prev).

  Be sure that the size_ member of the resulting list is correct.

  A correct implementation of this function has O(n) time complexity
  for a list of length n. That is, in the worst case, you would
  traverse each element of the list some constant number of times.

  Important notes for getting the correct running time:

  1. Since both lists being merged are already sorted themselves, there
     is a way to merge them together into a single sorted list in a single
     traversal pass down the lists. This can run in O(n) time.
  2. You SHOULD NOT call any sorting function in your merge function.
  3. You SHOULD NOT call the insertOrdered function in merge. That would
     result in a very slow running time. (The insertOrdered function was
     part of the insertion sort exercise. It has nothing to do with merge
     or merge sort.)

  You can use "make test" followed by "./test" to check the correctness
  of your implementation, and then you can use "./test [bench]" to run
  some interesting benchmarks on the speed of your code.

 ********************************************************************/

template <typename T>
LinkedList<T> LinkedList<T>::merge(const LinkedList<T>& other) const {

  LinkedList<T> left = *this;
  LinkedList<T> right = other;

  LinkedList<T> merged;

  if (left.size() <= 1) {
    return left;
  }
  else{
    while (left.size() > 0 && right.size() > 0){
          if (left.front() <= right.front()){
            merged.pushBack(left.front());
            left.popFront();
          }
          else if (right.front() < left.front()){
            merged.pushBack(right.front());
            right.popFront();
          }
      }
      while (left.size() > 0){
              merged.pushBack(left.front());
              left.popFront();
            }
      while (right.size() > 0){
              merged.pushBack(right.front());
              right.popFront();
            }
  }
  return merged;
}
