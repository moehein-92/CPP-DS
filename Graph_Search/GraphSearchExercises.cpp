
#include "GraphSearchCommon.h"

// =========================================================================
// EXERCISE 1: Adjacency List Utilities
//
// This exercise is based on the Week 3 lecture material.
//
// Our GridGraph class implements an "adjacency list" graph data structure,
// although it actually makes use of std::unordered_map and std::unordered_set
// to do this. You can read about GridGraph in the instructions PDF and by
// examining GridGraph.h. There are also various examples of GridGraph usage
// in the informal tests defined in main.cpp.
//
// Most of the GridGraph class is already implemented for you. For this
// exercise, you need to finish implementing two functions below:
//
// GridGraph::countEdges
// GridGraph::removePoint
//
// =========================================================================

// GridGraph::countEdges:
// This function should return the number of unique edges in the GridGraph.
// Remember that GridGraph doesn't explicitly store edge objects, but
// instead it stores point adjacency information. Since the graph edges are
// undirected, for any two vertices A and B, the edges (A,B) and (B,A) are the
// same edge. This means if we add up the sizes of the GridGraph::NeighborSet
// sets mapped in adjacencyMap, we'll be double-counting the number of edges.
// We can still use that to get the answer, or instead, the implementation of
// GridGraph::printDetails in GridGraph.h shows another method, by constructing
// a set of the unique edges only.
int GridGraph::countEdges() const {
  int numEdges = 0;

  for (auto i = adjacencyMap.begin(); i != adjacencyMap.end(); i++) {
      auto j = i->second;

      for (auto k = j.begin(); k != j.end(); k++){
        numEdges++;
      }
  }
  return numEdges/2;
}

void GridGraph::removePoint(const IntPair& p1) {

  if (!hasPoint(p1)) return;

  const GridGraph::NeighborSet originalNeighbors = adjacencyMap.at(p1);

  for (auto i = originalNeighbors.begin(); i != originalNeighbors.end(); i++){
      removeEdge(p1, *i);
  }
  adjacencyMap.erase(p1);
}

// =========================================================================
// EXERCISE 2: graphBFS
//
// This exercise is based on the Week 4 lecture material.
//
// The graphBFS function below largely implements the "breadth-first search"
// algorithm for the GridGraph class. You'll be asked to edit a few parts of
// the function to complete it. Those parts are marked with "TODO".
// (Please read the instructions PDF in case these hints are expanded upon
//  there.)
//
// Differences from the version of BFS discussed in lecture:
//
// - This implementation of BFS is geared toward finding the shortest path
//   from a start point to a goal point. So, it only explores within a single
//   connected component, and it may report that the goal is unreachable.
//   As soon as the goal point is found, our algorithm stops searching.
//
// - Our implementation uses sets of points to implicitly "label" points with
//   some status such as "visited", instead of assigning a label property to
//   a point itself. This lets us associate more than one kind of status with
//   any given point at the same time, by putting the point in several sets.
//   It's convenient to do this with STL containers like unordered set, since
//   we don't have to add some kind of status member variable to our own
//   classes like IntPair this way. It also means we don't have to initialize
//   a status for every vertex. In some graph search problems, such as later
//   in Exercise 3, the number of vertices is extremely large, so we'd rather
//   not have to initialize them all!
//
// - We use a map, "pred", to record the predecessor vertex of any newly-
//   discovered vertex during the search. This implicitly records what the
//   discovery edges are, as described in lecture. We can use that to rebuild
//   the path from start to goal at the end.
//
// - We do not assign status directly to edges. However, the vertex
//   predecessor information is basically recording the "discovery" edges.
//
// - We use a map, "dist", to record information about the shortest-path
//   distance to any given node that has been discovered so far. This is
//   similar to what Dijkstra's algorithm does, although with BFS graph search
//   when the edge lengths are all equal to 1, we know that as soon as we
//   discover a node, we have found the shortest path to it. We can still use
//   this to detect if we've taken more steps than expected and quit.
//
// - Redundantly, we have created a "dequeued set" that we use to help you
//   check for mistakes that could cause an infinite loop. This isn't normally
//   part of the BFS algorithm itself. In a way, this is mirroring what the
//   "visited set" is already supposed to accomplish.
//
// =========================================================================


std::list<IntPair> graphBFS(const IntPair& start, const IntPair& goal, const GridGraph& graph) {


  constexpr int maxDist = 100;


  std::queue<IntPair> exploreQ;

  std::unordered_map<IntPair, IntPair> pred;
  // Example usage: "pred[B] = A" records A as the predecessor of B, meaning that A discovered B.

  std::unordered_map<IntPair, int> dist;
  //Map that associates a given point with its shortest-path distance from the starting point.

  std::unordered_set<IntPair> visitedSet;

  // This is technically redundant, because we also have the visitedSet to use.
  std::unordered_set<IntPair> dequeuedSet;

  if (!graph.hasPoint(start)) throw std::runtime_error("Starting point doesn't exist in graph");
  if (!graph.hasPoint(goal)) throw std::runtime_error("Goal point doesn't exist in graph");


  pred[start] = start;

  dist[start] = 0;

  visitedSet.insert(start);

  // We will begin searching from the start point. Push a copy of it into the exploration queue.
  exploreQ.push(start);

  bool foundGoal = (start == goal);

  bool tooManySteps = false;

  // The main search loop --------------------------------------------------

  // While the exploration queue isn't empty yet, there are still discovered points to explore.
  // Also, we would want to stop looping if foundGoal or tooManySteps became true.
  while (!exploreQ.empty() && !foundGoal && !tooManySteps) {

    // Get a copy of the next point to explore from the front of the queue.
    // This becomes the "current" point.
    auto curPoint = exploreQ.front(); // This is the working point.
    // Pop to remove the point from the queue.
    exploreQ.pop();

    // Error prevention: Check whether this vertex has ever been dequeued in the past.
    bool curPointWasPreviouslyDequeued = dequeuedSet.count(curPoint);
    if (curPointWasPreviouslyDequeued) {
      std::cout << "graphBFS ERROR: Dequeued a vertex that had already been dequeued before." << std::endl
        << " If you're   using visitedSet correctly, then no vertex should ever be added" << std::endl
        << " to the explore qeueue more than once. [Returning an empty path now.]" << std::endl << std::endl;
      return std::list<IntPair>();
    }
    else {
      // We'll record that this vertex has been dequeued by adding a copy of it to the set.
      dequeuedSet.insert(curPoint);
    }


    GridGraph::NeighborSet neighbors = graph.adjacencyMap.at(curPoint);

    for (auto neighbor : neighbors) {

      bool neighborWasAlreadyVisited = false;

      if (visitedSet.find(neighbor) != visitedSet.end()) {
        neighborWasAlreadyVisited = true;
    }

      // If this adjacent vertex has NOT been visited before, we will visit it now.
      // If it HAS been visited before, we do nothing and continue to loop.
      // This way, we avoid enqueueing the same vertex more than once.
      if (!neighborWasAlreadyVisited) {

        // ================================================================
        // TODO: Your code here!
        pred[neighbor] = curPoint;
        // Record that the curPoint is the predecessor of the neighbor point,
        // since curPoint has just led to the discovery of this neighbor for
        // the first time.
        visitedSet.insert(neighbor);
        // Add neighbor to the visited set.
        exploreQ.push(neighbor);
        // Push neighbor into the exploration queue.

        dist[neighbor] = dist[curPoint]+1;
        if (dist[neighbor] > maxDist) {
          tooManySteps = true;
          break;
        }

        if (neighbor == goal) {
          foundGoal = true;
          break;
        }

      }
    }
  }

  if (tooManySteps) {
    std::cout << "graphBFS warning: Could not reach goal within the maximum allowed steps.\n (This may be expected if no path exists.)" << std::endl << std::endl;
    return std::list<IntPair>();
  }

  if (!foundGoal) {
    std::cout << "graphBFS warning: Could not reach goal. (This may be expected\n if no path exists.)" << std::endl << std::endl;
    return std::list<IntPair>();
  }

  // Make a new, empty list of IntPairs in the stack memory at function scope.
  // This will represent the path from start to goal. We'll return it by value at the end.
  std::list<IntPair> path;
  // We will walk back from the goal to the start using the predecessor records.
  auto cur = goal;
  // Push the currently-walked vertex onto the FRONT of the list, so that as we walk back
  // from goal to start, the created list will be in order from start to goal.
  path.push_front(cur);
  // Make sure that there is a predecessor recorded for cur, and then while the
  // predecessor of cur is not recorded as itself, keep looping. (The start vertex
  // recorded itself as its predecessor, so then we know to stop.)
  while (pred.count(cur) && pred[cur] != cur) {

    // Push a copy of the predecessor onto the front of the list, as we reconstruct
    // the path back to the start.
    path.push_front(pred[cur]);
    // Step backwards to the predecessor so we can continue looping.
    cur = pred[cur];
  }
  // Return the reconstructed path from start to goal.
  return path;
}


// =========================================================================
// EXERCISE 3: puzzleBFS
//
// This time, we will use BFS to solve a graph modeling problem. This is
// where we model a realistic problem in terms of an imaginary graph, and
// then we can use graph search concepts to solve the modeled problem.
//
// (Please see the instructions PDF for a detailed introduction to this
//  problem, with illustrations.)
//
// The PuzzleState class represents one current state of the "8 puzzle",
// which is a puzzle played on a 3x3 grid containing 8 tiles, where a tile
// can slide into the blank space to shuffle the puzzle. Each state of the
// puzzle can be modeled as a vertex in an imaginary graph, where two states
// are virtually connected by an edge (adjacent) if a single tile move can
// transform the first state into the second state. We do not need a map
// structure for adjacencies since we can use the helper functions from the
// PuzzleState class to determine which states are adjacent at any given
// time. Therefore we also don't need an explicit graph class at all. It's
// important that we can use such an implicit graph representation, because
// the total number of vertices (states) and edges (moves) in the graph model
// for "8 puzzle" would be extremely large, and that would greatly impact the
// running time and memory usage. We don't need to examine every vertex or
// every edge in the graph model; we can just search from the start and quit
// after either finding the goal or giving up (in relatively few steps).
//
// This function is VERY similar to graphBFS, but now we are using the
// PuzzleState class to represent imaginary vertices instead of using IntPair
// to represent literal 2D points, and we do not use GridGraph. You should
// finish graphBFS first before trying this problem. The starter code shown
// below for puzzleBFS is so similar to graphBFS that the comments have mostly
// been removed.
//
// =========================================================================

// puzzleBFS:
// Given start and goal sates as PuzzleState objects, we perform BFS in the
// imaginary graph model implied by the start state, where the rest of the
// reachable vertices (states) and the edges leading to them (puzzle moves)
// can be figure out based on the tile sliding rules of the puzzle.
// If there exists any path from start to goal, then the puzzle can be solved,
// and we return the shortest path (which represents the solution steps).
// If there is no path to the goal, or if we take too many steps without
// success, then the puzzle cannot be solved, and we return an empty list.
std::list<PuzzleState> puzzleBFS(const PuzzleState& start, const PuzzleState& goal) {

  // Intialization details -------------------------------------------------

  // maxDist failsafe:
  //
  // For the 8-tile puzzle, it actually never takes as many as 35 steps to
  // solve any given puzzle that has a solution. This has been proven in
  // the past by brute force. So we can implement a failsafe measure:
  // if we've explored all possible 35-move sequences and there's still no
  // solution found, then the puzzle cannot be solved. Then we can give up
  // and return early. (There do exist unreachable states for the 8 puzzle
  // because the tiles can only be rearranged by following the sliding rule.
  // So it's possible to specify some "goal" states that cannot be reached by
  // sliding the tiles.)
  constexpr int maxDist = 35;

  std::queue<PuzzleState> exploreQ;
  std::unordered_map<PuzzleState, PuzzleState> pred;
  std::unordered_map<PuzzleState, int> dist;
  std::unordered_set<PuzzleState> visitedSet;
  std::unordered_set<PuzzleState> dequeuedSet;

  pred[start] = start;
  dist[start] = 0;
  visitedSet.insert(start);
  exploreQ.push(start);

  bool foundGoal = (start == goal);
  bool tooManySteps = false;

  // The main search loop --------------------------------------------------

  while (!exploreQ.empty() && !foundGoal && !tooManySteps) {

    auto curState = exploreQ.front();
    exploreQ.pop();

    bool curPointWasPreviouslyDequeued = dequeuedSet.count(curState);
    if (curPointWasPreviouslyDequeued) {
      std::cout << "puzzleBFS ERROR: Dequeued a vertex that had already been dequeued before." << std::endl
        << " If you're using visitedSet correctly, then no vertex should ever be added" << std::endl
        << " to the explore qeueue more than once. [Returning an empty path now.]" << std::endl << std::endl;
      return std::list<PuzzleState>();
    }
    else {
      dequeuedSet.insert(curState);
    }

    // =====================================================================
    // TODO: Your code here!
    // We'll need to loop over the neighbors that are the points adjacent to curState.
    // We need a collection of neighbors we're going to loop over.

    auto neighbors = curState.getAdjacentStates(); // Change this! This line is totally wrong.

    // Hint: Look at PuzzleState.h
    // =====================================================================

    for (auto neighbor : neighbors) {

      // ==================================================================
      // TODO: Your code here!
      // Check whether the neighbor has already been visited.

      bool neighborWasAlreadyVisited = false; // Change this...

      if (visitedSet.count(neighbor) == 1){
        neighborWasAlreadyVisited = true;
      }

      // ==================================================================

      if (!neighborWasAlreadyVisited) {

        // ================================================================
        // TODO: Your code here!

        // Record that the curState is the predecessor of the neighbor point,
        // since curState has just led to the discovery of this neighbor for
        // the first time.
        pred[neighbor] = curState;

        // Add neighbor to the visited set.
        visitedSet.insert(neighbor);

        // Push neighbor into the exploration queue.
        exploreQ.push(neighbor);

        // ================================================================

        dist[neighbor] = dist[curState]+1;
        if (dist[neighbor] > maxDist) {
          tooManySteps = true;
          break;
        }

        if (neighbor == goal) {
          foundGoal = true;
          break;
        }

      } // end of handling the just-discovered neighbor

    } // end of for loop
  } // end of while loop

  if (tooManySteps) {
    std::cout << "puzzleBFS warning: Could not reach goal within the maximum allowed steps.\n (This may be expected if no path exists.)" << std::endl << std::endl;
    return std::list<PuzzleState>();
  }

  if (!foundGoal) {
    std::cout << "puzzleBFS warning: Could not reach goal. (This may be expected\n if no path exists.)" << std::endl << std::endl;
    return std::list<PuzzleState>();
  }

  std::list<PuzzleState> path;
  auto cur = goal;
  path.push_front(cur);
  while (pred.count(cur) && pred[cur] != cur) {
    path.push_front(pred[cur]);
    cur = pred[cur];
  }

  return path;
}
