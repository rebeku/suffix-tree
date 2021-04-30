#ifndef SUFFIXTREE_H__
#define SUFFIXTREE_H__

#include "edge.h"
#include "EdgeQueue.h"

using namespace std;

class SuffixTree{
public:
  SuffixTree();
  ~SuffixTree();
  shared_ptr<s_tree> BuildTree(string genome);
  shared_ptr<substring> FindTopSubstring(shared_ptr<s_tree> tree, string sequence);
  vector<shared_ptr<substring>> FindTopNSubstrings(shared_ptr<s_tree> tree, string sequence, int n);
  vector<vector<shared_ptr<substring>>> FindBulkTopNSubstrings(shared_ptr<s_tree> tree, vector<string> sequences, int n);
  void print(shared_ptr<s_tree> tree, int depth=0);

private:
  
  // edge_match stores pertinent information about
  // how far a suffix matches an edge.
  
  // init_node creates a new node
  shared_ptr<s_tree> init_node();
  
  // init_edge creates a new node
  shared_ptr<edge> init_edge(string text, shared_ptr<s_tree> dst);

  // count_match_chars counts the number of matching chars
  // start from the beginning of both strings.
  int count_match_chars(string a, string b);

  // find *suffix* place in *tree* starting from 
  // the *s_start*th char of *suffix*
  // use *s_start* = 0 to search the full suffix
  // this param is included for easy recursive calls
  // *tree_start* is only used when building the tree.  It indicates
  // the starting index in the genome to be tracked by each intermediate
  // node in the tree.
  // Note: when *tree_start* is not its default value this function
  // will modify the tree.
  edge_match find(shared_ptr<s_tree> tree, string suffix, int s_start, int tree_start=-1);

  // helper function for inserting a new suffix
  // the edge will maintain the common chars shared
  // the edge and the new suffix, but will point to a new node
  // which will fork between the remaining chars of the old edge
  // and the chars for the suffix.
  void split(edge_match match, string suffix, int start);
  void insert(shared_ptr<s_tree> tree, string suffix, int index); 
};

#endif // EMPTY_H__