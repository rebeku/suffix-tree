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
  shared_ptr<edge> matched = NULL;

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

// This stores relevant information about the result
// of a substring match
struct Substring{
  vector<int> tree; // starting pts in tree
  vector<int> seq; // starting pts in seq
  int length;
  Substring(vector<int> tree_start, vector<int> seq_start, int len){
    tree = tree_start;
    seq = seq_start;
    length = len;
  }
};


class SuffixTree{
public:
  SuffixTree();
  ~SuffixTree();
  shared_ptr<s_tree> BuildTree(string genome);
  shared_ptr<Substring> FindTopSubstring(shared_ptr<s_tree> tree, string sequence);
  vector<shared_ptr<Substring>> FindTopNSubstrings(shared_ptr<s_tree> tree, string sequence, int n);
  vector<vector<shared_ptr<Substring>>> FindBulkTopNSubstrings(shared_ptr<s_tree> tree, string sequence);
  void print(shared_ptr<s_tree> tree);

private:
  
  // edge_match stores pertinent information about
  // how far a suffix matches an edge.
  
  // init_node creates a new node
  shared_ptr<s_tree> init_node();
  
  // init_edge creates a new node
  shared_ptr<edge> init_edge(string text, shared_ptr<s_tree> dst);

  // count_match_chars counts the number of matching chars
  // start from the beginning of boths strings.
  int count_match_chars(string a, string b);

  // find *suffix* place in *tree* starting from 
  // the *s_start*th char of *suffix*
  // use *s_start* = 0 to search the full suffix
  // this param is included for easy recursive calls
  edge_match find(shared_ptr<s_tree> tree, string suffix, int s_start);

  // helper function for inserting a new suffix
  // the edge will maintain the common chars shared
  // the edge and the new suffix, but will point to a new node
  // which will fork between the remaining chars of the old edge
  // and the chars for the suffix.
  void split(edge_match match, string suffix, int start);
  void insert(shared_ptr<s_tree> tree, string suffix, int index); 
};

#endif // EMPTY_H__