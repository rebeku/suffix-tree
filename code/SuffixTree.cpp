#include "SuffixTree.h"


SuffixTree::~SuffixTree() {}

shared_ptr<s_tree> SuffixTree::init_node() {
  shared_ptr<s_tree> node(new s_tree);
  node->edges = vector<shared_ptr<edge>>{};
  node->starts = vector<int>{};
  return node;
}

void SuffixTree::print(shared_ptr<s_tree> tree, int depth) {
  for (int i = 0; i < depth; i++) {
    cout << "\t";
  }
  cout << "Node with depth " << depth << " has starts [";
  for (int i : tree->starts) {
    cout << i << ", ";
  }
  cout << "]" << endl;
  for (const auto &e : tree->edges) {
    for (int i = 0; i < depth; i++) {
      cout << "\t";
    }
    cout << "Edge with label " << e->text << " points to: \n";
    if (e->dst) {
      print(e->dst, depth + 1);
    } else {
      cout << "nothing.  Whoops." << endl;
    }
  }
  cout << "\n\n";
}

shared_ptr<edge> SuffixTree::init_edge(string text, shared_ptr<s_tree> dst) {
  shared_ptr<edge> e(new edge);
  e->text = text;
  e->dst = dst;
  return e;
}

int SuffixTree::count_match_chars(string a, string b) {
  int n = min(a.length(), b.length());
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) {
      return i;
    }
  }
  return n;
}

edge_match SuffixTree::find(shared_ptr<s_tree> tree, string suffix, int s_start,
                            int tree_start) {
  if (tree_start >= 0) {
    tree->starts.push_back(tree_start);
  }
  bool full_edge_match = false;
  bool full_suffix_match = false;

  edge_match match;

  // save some basic stuff about where you are
  // this may be a recursive call and there may be no further
  // matches but you still need to know what was matched before
  match.src = tree;
  match.s_chars = s_start;

  for (const auto &e : tree->edges) {
    if (e->text[0] == suffix[0]) {
      tree = e->dst;
      match.e_chars = count_match_chars(e->text, suffix);
      match.s_chars = match.e_chars + s_start;
      match.matched = e;
      // we can count on there being at least one match here
      // because otherwise edge wouldn't be in tree
      // if there are multiple matches we just keep the first
      // otherwise we get into messy edge cases
      // that are expensive to resolve
      // long substrings are unlikely to match in multiple places
      // so this should have minimal impact on utility
      match.t_start = e->dst->starts[0];

      if (match.e_chars == e->text.length()) {
        full_edge_match = true;
      }
      if (match.e_chars == suffix.length()) {
        full_suffix_match = true;
      }
      break;
    }
  }

  if (full_suffix_match) {
    return match;
  } else if (full_edge_match) {
    // traverse edge and find the remainder of suffix
    shared_ptr<s_tree> next = match.matched->dst;
    suffix = suffix.substr(match.e_chars, string::npos);
    s_start = s_start + match.e_chars;
    return find(next, suffix, s_start, tree_start);
  } else {
    // this is not a full match but it's
    // the closest thing in the tree
    // If there is no match at all this should return nothing
    return match;
  }
}

void SuffixTree::split(edge_match match, string suffix, int start) {
  // first create the new edge that will point to
  // the existing children of the matched edge
  shared_ptr<edge> e = match.matched;
  string text_forked = e->text.substr(match.e_chars, string::npos);
  shared_ptr<edge> edge_forked = init_edge(text_forked, match.matched->dst);
  shared_ptr<s_tree> new_node = init_node();
  new_node->edges.push_back(edge_forked);
  new_node->starts = match.matched->dst->starts;
  new_node->starts.push_back(start);

  // now add the new fork containing the required
  // chars of the new *suffix*
  string text_new = suffix.substr(match.s_chars, string::npos);
  shared_ptr<s_tree> new_leaf = init_node();
  new_leaf->starts.push_back(start);
  shared_ptr<edge> new_edge = init_edge(text_new, new_leaf);

  // now hook everything up into the existing tree
  new_node->edges.push_back(new_edge);
  e->text = e->text.substr(0, match.e_chars);
  e->dst = new_node;
}

void SuffixTree::insert(shared_ptr<s_tree> tree, string suffix, int start) {
  edge_match match = find(tree, suffix, 0, start);
  if (match.e_chars == 0) {
    tree = match.src;
    int i = tree->edges.size();

    // TODO: is there a less verbose way of doing this?
    shared_ptr<edge> e(new edge);
    tree->edges.push_back(e);
    // don't re-save chars that are already matched by higher layers of tree
    tree->edges[i]->text = suffix.substr(match.s_chars, string::npos);

    shared_ptr<s_tree> node = init_node();
    node->starts.push_back(start);
    tree->edges[i]->dst = node;
  } else {
    split(match, suffix, start);
  }
}

SuffixTree::SuffixTree(string genome) {
  genome = genome + "$";
  tree = init_node();

  // genome.length-1 because there is no need to add the final '$'
  for (int i = 0; i < genome.length() - 1; i++) {
    string suffix = genome.substr(i, string::npos);
    insert(tree, suffix, i);
  }
}

vector<shared_ptr<substring>> SuffixTree::FindTopNSubstrings(string seq, int n) {
  edge_match cur_match;
  EdgeQueue q(n);

  for (int i = 0; i < seq.length(); i++) {
    cur_match = find(tree, seq.substr(i, string::npos), 0);
    if (cur_match.matched == NULL) {
      continue;
    }
    cur_match.s_start = i;
    q.push(cur_match);

    // no need to continue searching if the best matches
    // have more characters than any remaining suffix of *seq*
    if (q.size() == n && q.s_chars_cutoff() > seq.length() - i - 1) {
      break;
    }
  }

  return q.toSubstrings();
}

shared_ptr<substring> SuffixTree::FindTopSubstring(string seq) {
  vector<shared_ptr<substring>> v = FindTopNSubstrings(seq, 1);
  if (v.size() == 0) {
    return NULL;
  }
  return v[0];
}

vector<vector<shared_ptr<substring>>> SuffixTree::FindBulkTopNSubstrings(vector<string> sequences, int n) {
  vector<vector<shared_ptr<substring>>> ret;

  for (auto &seq : sequences) {
    ret.push_back(FindTopNSubstrings(seq, n));
  }
  return ret;
}

shared_ptr<s_tree> SuffixTree::root() {
    return tree;
}