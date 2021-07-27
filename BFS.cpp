
#include <iostream>
#include <string>

// Note: You must not change the definition of DisjointSets here.
class DisjointSets {
public:
  int s[256];
  int distance[256];


  DisjointSets() {
    for (int i = 0; i < 256; i++)
      s[i] = distance[i] = -1;
  }

  int find(int i) { return s[i] < 0 ? i : find(s[i]); }

  void dsunion(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
      s[root_i] = root_j;
    }
  }

  void bfs(int i, int n, int m, int edges[][2]);
};


/* Below are two conditions that need to be programmed
 * to allow this procedure to perform a breadth first
 * traversal and mark the edge distance of the graph's
 * vertices from vertex i.
 */

void DisjointSets::bfs(int i, int n, int m, int edges[][2]) {

  distance[i] = 0;

  for (int d = 1; d < m; d++) {
    int f = -1;
    int rooti = find(i);

    for (int j = 0; j < m; j++) {

      int root0 = find(edges[j][0]);
      int root1 = find(edges[j][1]);

      //std::cout << root0 << root1 << std::endl;

      if ( rooti == root0 && rooti != root1 ) {
        if (f == -1)
          f = edges[j][1];
        else
          dsunion(f, edges[j][1]);

        // set the distance of this frontier
        // vertex to d

        distance[edges[j][1]] = d;

      } else if ( rooti != root0 && rooti == root1) {
        if (f == -1)
          f = edges[j][0];
        else
          dsunion(f,edges[j][0]);
        distance[edges[j][0]] = d;
      }
    }

    // if no vertices added to the frontier
    // then we have run out of vertices and
    // are done, otherwise union the frontier
    // set with the set of vertices that have
    // already been processed.

    if (f == -1)
      break;
    else
      dsunion(f,i);
  }
}

int main() {

  int edges[8][2] = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,3}};

  DisjointSets d;

  d.bfs(3,8,8,edges);

  for (int i = 0; i < 8; i++)
    std::cout << "Distance to vertex " << i
              << " is " << d.distance[i] << std::endl;

// Should print
// Distance to vertex 0 is 3
// Distance to vertex 1 is 2
// Distance to vertex 2 is 1
// Distance to vertex 3 is 0
// Distance to vertex 4 is 1
// Distance to vertex 5 is 2
// Distance to vertex 6 is 2
// Distance to vertex 7 is 1


  return 0;
}
