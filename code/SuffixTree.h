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
  vector<shared_ptr<edge>> edges;
  int start;
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
  shared_ptr<edge> matched = NULL;

  edge_match(){}
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
  
  // edge_match stores pertinent information about
  // how far a suffix matches an edge.
  
  // init_node creates a new node
  shared_ptr<s_tree> init_node();

  // count_match_chars counts the number of matching chars
  // start from the beginning of boths strings.
  int count_match_chars(string a, string b);

  // find *suffix* place in *tree* starting from 
  // the *s_start*th char of *suffix*
  // use *s_start* = 0 to search the full suffix
  // this param is included for easy recursive calls
  edge_match find(shared_ptr<s_tree> tree, string suffix, int s_start);

  shared_ptr<s_tree> split(edge& e, string suffix);
  void insert(shared_ptr<s_tree> tree, string suffix, int index); 
};

#endif // EMPTY_H__