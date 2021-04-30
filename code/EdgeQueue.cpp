
#include "EdgeQueue.h"

EdgeQueue::EdgeQueue(int cap){
    capacity = cap;
    edges = vector<edge_match>{};
}

EdgeQueue::~EdgeQueue(){
    
}

int EdgeQueue::min_index() {
    if (edges.size()==0) {
        return -1;
    }
    int min_index = 0;

    for (int i=0; i<edges.size(); i++) {
        if (edges[i]<edges[min_index]) {
            min_index = i;
        }
    }

    return min_index;
}

bool EdgeQueue::redundant(edge_match em) {
    for (auto em2: edges) {
        // if em is a substring of em2
        if (em2.s_chars - em.s_chars == em.s_start - em2.s_start) {
            return true;
        }
    }
    return false;
}

void EdgeQueue::push(edge_match em) {
    cout << endl << "Pushing edge match with s_chars " << em.s_chars << endl; 
    if (redundant(em)) {
        return;
    } else if (edges.size() < capacity) {
        edges.push_back(em);
        return;
    }

    int mindex = min_index();
    if (em > edges[mindex]) {
        edges[mindex] = em;
    }
}


bool greater_than(edge_match a, edge_match b) {
    return a > b;
}

vector<shared_ptr<substring>> EdgeQueue::toSubstrings() {
    sort (edges.begin(), edges.end(), greater_than);
    edge_match cur_match;
    vector<shared_ptr<substring>> ret(edges.size());
    for (int i=0; i<edges.size(); i++) {
        cur_match = edges[i];
        vector<int> tree_start = cur_match.matched->dst->starts;
        vector<int> seq_start {cur_match.s_start};
        ret[i] = shared_ptr<substring> (new substring(tree_start, seq_start, cur_match.s_chars));
      }
      return ret;
}