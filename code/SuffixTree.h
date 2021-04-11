#ifndef SUFFIXTREE_H__
#define SUFFIXTREE_H__

#include <string>
#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <iostream>

using namespace std;

// Node in the Suffix Tree representing 
struct STree{
  map<char, shared_ptr<STree>> children;
  int start;
  int root_start; // TODO: Do I want this?
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
  shared_ptr<STree> BuildTree(string genome);
  shared_ptr<Substring> FindTopSubstring(shared_ptr<STree> tree, string sequence);
  vector<shared_ptr<Substring>> FindTopNSubstrings(shared_ptr<STree> tree, string sequence, int n);
  vector<vector<shared_ptr<Substring>>> FindBulkTopNSubstrings(shared_ptr<STree> tree, string sequence);

    
private:
  int empty_count;
};

#endif // EMPTY_H__