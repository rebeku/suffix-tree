#include <iostream>
#include <sstream>
#include <fstream>
#include<chrono>

#include "../code/SuffixTree.h"

using namespace std;

SuffixTree MYTREE;


shared_ptr<s_tree> read_tree(string fname) {
    string genome;
    ifstream gFile(fname);
    // This file should just have one line so getline will read it into genome.
    getline (gFile, genome);
    gFile.close();
    std::cout << "Read genome from file.  Length: " << genome.length() << endl;

    auto start = chrono::system_clock::now();
    shared_ptr<s_tree> tree = MYTREE.BuildTree(genome);
    auto end = chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "Built suffix tree in " <<  elapsed_seconds.count() << " seconds" << endl;
    return tree;
}

int main(){
    // string fname = "../data/genome_5_10000_100_500_0.07_0.txt";

    // shared_ptr<s_tree> tree = read_tree(fname);
    // std::cout << "Now matching genes from this genome...";

    string fname = "../data/genes_5_10000_100_500_0.1.csv";
    shared_ptr<s_tree> tree;

    ifstream gFile(fname);

    // track accuracy
    int n_correct = 0;
    int n_total = 0;
    
    string row;
    string word;
    int row_i = 0;
    
    string source_g; // file name of source genome
    int start_i;
    int backtrack_start;
    string full_text;

    while( getline(gFile, row)) {
        stringstream s(row);
        row_i = 0;

        while( getline(s, word, ',')) {
            try{
                switch(row_i) {
                    case 0: source_g = word; break;
                    case 1: start_i = stoi(word); break;
                    case 2: full_text = word; break;
                    case 3: break;
                }
            } catch(...) {
                std::cout << "can't convert '" << word << "' to int.\n";
                continue; 
            }
            row_i++;
        }

        // time to load the next genome
        if (fname != source_g) {
            fname = source_g;
            tree = read_tree(fname);
        }

        shared_ptr<Substring> ss = MYTREE.FindTopSubstring(tree, full_text);

        // if longest substring is aligned between seq and genome,
        // where on genome would seq start?
        backtrack_start = start_i + ss->seq[0];
        
        if (ss == NULL) {
            cout << "Found no matches for '" << full_text << "' at all.\n";
            cout << "Full row: " << row << endl;
        } else if (ss->tree[0] == backtrack_start) {
            n_correct++;
        } else {
            cout << "Expected start_i: " << backtrack_start << endl;
            cout << "Found tree starts ";
            for (int j: ss->tree) {
                cout << j << " ";
            } 
            cout << endl;
            cout << "Found seq starts: ";
            for (int j: ss->seq) {
                cout << j << " ";
            }
            cout << endl;
            cout << "Chars matched: " << ss->length << endl << endl;
        }
        n_total++;
    }

    cout << "Correctly matched " << n_correct << " out of " << n_total << " genes.\n";
    cout << "Accuracy rate:  " << float(n_correct) / float(n_total) << endl;
    return 0;
}