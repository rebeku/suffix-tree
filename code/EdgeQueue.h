#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "edge.h"

using namespace std;

// This stores relevant information about the result
// of a substring match
struct substring{
  vector<int> tree; // starting pts in tree
  vector<int> seq; // starting pts in seq
  int length;
  substring(vector<int> tree_start, vector<int> seq_start, int len){
    tree = tree_start;
    seq = seq_start;
    length = len;
  }
};

class EdgeQueue{
public:
    EdgeQueue(int cap);
    ~EdgeQueue();
    // push adds a new match
    // if and only if it is not redundant
    // with other edges already in queue.
    // i.e. not a substring of an edge that already exists
    // if capacity is exceeded, the worst match
    // will be overwritten
    void push(edge_match em);
    // construct Substrings from elements currently in queue
    vector<shared_ptr<substring>> toSubstrings();
    int size(); // number of elements in queue
    int s_chars_cutoff(); // minimum number of s_chars in queue

    private:
        int capacity;
        // NOTE:  The assumption here is that we are tracking
        // a small constant number of edge matches.  Thus, 
        // optimizing performance of edge queue is not important
        // for performance.  So we just keep edges in a vector
        // and sort before converting to Substrings.
        vector<edge_match> edges;
        // TODO: Can I delete this?
        // tree_ends is used to evaluate whether a new match
        // is actually redundant with an existing match
        unordered_set<int> tree_ends;

        int min_index();
        int nonredundantTreeStart(edge_match em);
};