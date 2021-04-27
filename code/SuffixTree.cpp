#include "SuffixTree.h"

SuffixTree::SuffixTree(){
    
}

SuffixTree::~SuffixTree(){
    
}

shared_ptr<s_tree> SuffixTree::init_node() {
    shared_ptr<s_tree> node (new s_tree);
    node->edges = vector<shared_ptr<edge>>{};
    node->starts = vector<int>{};
    return node;
}

void SuffixTree::print(shared_ptr<s_tree> tree, int depth) {
    for (int i=0; i<depth; i++) {
        cout <<"\t";
    }
    cout << "Node with depth " << depth << " has starts [";
    for (int i: tree->starts) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    for (const auto& e: tree->edges) {
        for (int i=0; i<depth; i++) {
            cout <<"\t";
        }
        cout << "Edge with label " << e->text << " points to: \n";
        if (e->dst) {
            print(e->dst, depth+1);
        } else {
            cout << "nothing.  Whoops." << endl;
        }
    }
    cout << "\n\n"; 
}

shared_ptr<edge> SuffixTree::init_edge(string text, shared_ptr<s_tree> dst) {
    shared_ptr<edge> e (new edge);
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

edge_match SuffixTree::find(shared_ptr<s_tree> tree, string suffix, int s_start, int tree_start) {
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

    for (const auto& e: tree->edges) {
        if (e->text[0] == suffix[0]) {
            tree = e->dst;
            match.e_chars = count_match_chars(e->text, suffix);
            match.s_chars = match.e_chars + s_start;
            match.matched = e;

            if (suffix.substr(0, 18) == "ATGAGATGAGAGGTGTCT") {
                cout << "found a match for suffix 6\n";
                cout << "e_chars: " << match.e_chars << endl;
                cout << "s_chars: " << match.s_chars << endl;
                cout << "null edge? " << (match.matched == NULL) << endl;
            }
            

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


void SuffixTree::split(edge_match match, string suffix, int start){
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
    if (suffix.substr(0, 18) == "ATGAGATGAGAGGTGTCT") {
        cout << "Inserting suffix 6" << endl;
    }
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
    } else{
        split(match, suffix, start);
    }
}    

shared_ptr<s_tree> SuffixTree::BuildTree(string genome){
    genome = genome + "$";
    shared_ptr<s_tree> tree = init_node();

    // genome.length-1 because there is no need to add the final '$'
    for (int i=0; i<genome.length()-1; i++) {
        string suffix = genome.substr(i, string::npos);
        insert(tree, suffix, i);
    }
    return tree;
}

vector<shared_ptr<Substring>> SuffixTree::FindTopNSubstrings(shared_ptr<s_tree> tree, string seq, int n) {
      edge_match cur_match;
	  priority_queue<edge_match, vector<edge_match>, std::greater<edge_match>> q;

      for (int i = 0; i<seq.length(); i++) {
          cur_match = find(tree, seq.substr(i, string::npos), 0);
          if (cur_match.matched == NULL) {
              continue;
          }
          cur_match.s_start = i;
          q.push(cur_match);

          if (q.size() > n) {
              q.pop();
          }

          // no need to continue searching if the best matches
          // have more characters than any remaining suffix of *seq*
          if (q.size() == n && q.top().s_chars > seq.length() - i - 1) {
            break;
          }
      }

      // there may not b *n* matches
      // so reduce the size of *n* if needed
      n = q.size();
      vector<shared_ptr<Substring>> ret(n);
      int i = 1;

      while (!q.empty()) {
        cur_match = q.top();
        q.pop();

        vector<int> tree_start = cur_match.matched->dst->starts;
        vector<int> seq_start {cur_match.s_start};
        ret[n - i] = shared_ptr<Substring> (new Substring(tree_start, seq_start, cur_match.s_chars));
        i++;
      }
      return ret;
  }

  shared_ptr<Substring> SuffixTree::FindTopSubstring(shared_ptr<s_tree> tree, string seq) {
    vector<shared_ptr<Substring>> v = FindTopNSubstrings(tree, seq, 1);
    if (v.size() == 0) {
        return NULL;
    }
    return v[0];
  }


  vector<vector<shared_ptr<Substring>>> SuffixTree::FindBulkTopNSubstrings(shared_ptr<s_tree> tree, vector<string> sequences, int n) {
      vector<vector<shared_ptr<Substring>>> ret;

        for (auto &seq : sequences) {
            cout << seq << endl;
            ret.push_back(FindTopNSubstrings(tree, seq, n));
        }
      return ret;
  }