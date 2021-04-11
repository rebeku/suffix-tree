#ifndef SUFFIXTREE_H__
#define SUFFIXTREE_H__

#include <string>
#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

struct edge;

// Node in the Suffix Tree 
struct s_tree{
  vector<edge> edges;
  int start;
  int root_start; // TODO: Do I want this?
};

struct edge{
  string text;

  bool operator() (shared_ptr<edge> a, shared_ptr<edge> b) {
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

// This stores relevant information about the result
// of a substring match
struct Substring{
  int start[2]; // start index in longer and shorter string
  int length;
};

class SuffixTree{
public:
  SuffixTree();
  ~SuffixTree();
  shared_ptr<s_tree> BuildTree(string genome);
  shared_ptr<Substring> FindTopSubstring(shared_ptr<s_tree> tree, string sequence);
  vector<shared_ptr<Substring>> FindTopNSubstrings(shared_ptr<s_tree> tree, string sequence, int n);
  vector<vector<shared_ptr<Substring>>> FindBulkTopNSubstrings(shared_ptr<s_tree> tree, string sequence);

    
private:
  int empty_count;
};

#endif // EMPTY_H__