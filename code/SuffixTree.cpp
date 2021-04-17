#include "SuffixTree.h"

SuffixTree::SuffixTree(){
    
}

SuffixTree::~SuffixTree(){
    
}

shared_ptr<s_tree> SuffixTree::init_node() {
    shared_ptr<s_tree> node (new s_tree);
    vector<shared_ptr<edge>> edges;
    // TODO: make this suck less
    node->edges = edges;
    return node;
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

edge_match SuffixTree::find(shared_ptr<s_tree> tree, string suffix, int s_start) {
    int match_chars = 0;
    bool full_edge_match = false;
    bool full_suffix_match = false;


    edge_match match;

    for (const auto& e: tree->edges) {
        if (e->text[0] == suffix[0]) {
            tree = e->dst;
            match.e_chars = count_match_chars(e->text, suffix);
            match.s_chars = match.e_chars + s_start;
            match.matched = e;

            if (match_chars == e->text.length()) {
                full_edge_match = true;
            }
            if (match_chars == suffix.length()) {
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
        return find(next, suffix, s_start);
    } else {
        // this is not a full match but it's 
        // the closest thing in the tree
        return match;
    }
}

void SuffixTree::insert(shared_ptr<s_tree> tree, string suffix, int start) {
    edge_match match = find(tree, suffix, 0);
    if (match.e_chars == 0) {
        int i = tree->edges.size();

        // TODO: is there a less verbose way of doing this?
        shared_ptr<edge> e(new edge);
        tree->edges.push_back(e);
        tree->edges[i]->text = suffix;

        shared_ptr<s_tree> node = init_node();
        node->start = start;
        tree->edges[i]->dst = node;
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