#ifndef SUFFIXTREE_H__
#define SUFFIXTREE_H__

#include "edge.h"
#include "EdgeQueue.h"

using namespace std;

class SuffixTree{
private:
  // this stores the root node of the actual suffix tree
  shared_ptr<s_tree> tree;

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
public:
  SuffixTree(string genome);
  ~SuffixTree();
  shared_ptr<substring> FindTopSubstring(string sequence);
  vector<shared_ptr<substring>> FindTopNSubstrings(string sequence, int n);
  vector<vector<shared_ptr<substring>>> FindBulkTopNSubstrings(vector<string> sequences, int n);
  void print(shared_ptr<s_tree> tree, int depth=0);
  shared_ptr<s_tree> root();
};

#endif // EMPTY_H__