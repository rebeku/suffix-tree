#include <algorithm>
#include "SuffixTree.h"

using namespace std;

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
    vector<shared_ptr<Substring>> toSubstrings();

    private:
        int capacity;
        // NOTE:  The assumption here is that we are tracking
        // a small constant number of edge matches.  Thus, 
        // optimizing performance of edge queue is not important
        // for performance.  So we just keep edges in a vector
        // and sort before converting to Substrings.
        vector<edge_match> edges;

        int min_index();
        bool redundant(edge_match em);
};