#include "EdgeQueue.h"

EdgeQueue::EdgeQueue(int cap) {
  capacity = cap;
  edges = vector<edge_match>{};
}

EdgeQueue::~EdgeQueue() {}

int EdgeQueue::min_index() {
  if (edges.size() == 0) {
    return -1;
  }
  int min_index = 0;

  for (int i = 0; i < edges.size(); i++) {
    if (edges[i] < edges[min_index]) {
      min_index = i;
    }
  }

  return min_index;
}

int EdgeQueue::nonredundantTreeStart(edge_match em) {
  // cout << "testing nonredunt tree start for edge with t_start " << em.t_start
  // << endl;
  for (auto em2 : edges) {
    // if em is a substring of em2
    if (em.t_start - em2.t_start == em.s_start - em2.s_start) {
      // is there another tree start that would not be redundant?
      for (int ts : em.matched->dst->starts) {
        if (ts != em.t_start) {
          return ts;
        }
      }

      return -1;
    }
  }
  return em.t_start;
}

void EdgeQueue::push(edge_match em) {
  // make sure there is a tree start that is not redundant
  em.t_start = nonredundantTreeStart(em);
  if (em.t_start == -1) {
    return;
  }
  if (edges.size() < capacity) {
    edges.push_back(em);
    return;
  }

  int mindex = min_index();
  if (em > edges[mindex]) {
    edges[mindex] = em;
  }
}

bool greater_than(edge_match a, edge_match b) { return a > b; }

vector<shared_ptr<substring>> EdgeQueue::toSubstrings() {
  sort(edges.begin(), edges.end(), greater_than);
  edge_match cur_match;
  vector<shared_ptr<substring>> ret(edges.size());
  for (int i = 0; i < edges.size(); i++) {
    cur_match = edges[i];
    ret[i] = shared_ptr<substring>(
        new substring(cur_match.t_start, cur_match.s_start, cur_match.s_chars));
  }
  return ret;
}

int EdgeQueue::size() { return edges.size(); }

int EdgeQueue::s_chars_cutoff() {
  int mindex = min_index();
  return edges[mindex].s_chars;
}