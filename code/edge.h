
#ifndef EDGE_H__
#define EDGE_H__

#include <memory>
#include <string>
#include <vector>

using namespace std;

struct edge;

// Node in the Suffix Tree 
struct s_tree{
  vector<shared_ptr<edge>> edges;
  vector<int> starts; // this contains the indices of all starting chars of the genome that will lead to this node
};

struct edge{
  string text;
  shared_ptr<s_tree> dst;

  friend bool operator< (const shared_ptr<edge> a, const shared_ptr<edge> b) {
    int j = min(a->text.size(), b->text.size());
    for (int i=0; i<j; i++) {
      if (a->text[i] < b->text[i]) {
        return true;
      } else if (a->text[i] > b->text[i]) {
        return false;
      }
    }
    return j == a->text.size();
  }
};

struct edge_match {
  int e_chars = 0; // numbers of characters into the edge that match
  int s_chars = 0; // number of characters into the original suffix that match
  int s_start = -1; // first char of seq that is matched
  int t_start = -1; // first char of tree that is matched
  shared_ptr<edge> matched = NULL; // matched edge
  shared_ptr<s_tree> src = NULL; // source node of matched edge

  edge_match(){}
  friend bool operator> (const edge_match a, const edge_match b) {
    return a.s_chars > b.s_chars;
  }
  friend bool operator< (const edge_match a, const edge_match b) {
    return a.s_chars < b.s_chars;
  }

  friend bool operator== (const edge_match a, const edge_match b) {
    // don't check for equality of matched edges
    // this won't be needed and leads to messy edge cases
    if (a.e_chars != b.e_chars) {
      return false;
    } else if (a.s_chars != b.s_chars) {
      return false;
    } else if (a.s_start != b.s_start) {
      return false;
    } else {
      return true;
    }
  }
};

#endif