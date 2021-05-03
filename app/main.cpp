#include <iostream>
#include <sstream>
#include <fstream>
#include<chrono>

#include "../code/SuffixTree.h"

using namespace std;

shared_ptr<SuffixTree> read_tree(string fname) {
    string genome;
    ifstream gFile(fname);
    // This file should just have one line so getline will read it into genome.
    getline (gFile, genome);
    gFile.close();
    std::cout << "Read genome from file '" << fname << "'.  Length: " << genome.length() << endl;

    auto start = chrono::system_clock::now();
    shared_ptr<SuffixTree> tree(new SuffixTree(genome));
    auto end = chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    return tree;
}

int main(int argc, char** argv){
    string fname = argv[1];
    cout << "Reading test data from " << fname << endl;

    shared_ptr<SuffixTree> tree;

    ifstream gFile(fname);

    // track accuracy
    int n_correct = 0;
    int n_total = 0;
    int batch_size = 0;
    
    string row;
    string word;
    int row_i = 0;
    
    string source_g; // file name of source genome
    int start_i;
    int backtrack_start;
    string full_text;

    chrono::system_clock::time_point batch_start;
    chrono::system_clock::time_point batch_end;
    std::chrono::duration<double> elapsed_seconds;

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
            if (n_total > 0) {
                batch_end = chrono::system_clock::now();
                elapsed_seconds = batch_end - batch_start;
                cout << "Matched " << batch_size << " genes from sample genome in " << elapsed_seconds.count() << " seconds\n";
                cout << "Accuracy rate so far:  " << float(n_correct) / float(n_total) << endl;
                cout << "Loading next genome..." << endl << endl;
            }
            fname = source_g;
            tree = read_tree(fname);
            batch_size = 0;
            batch_start = chrono::system_clock::now();
        }

        shared_ptr<substring> ss = tree->FindTopSubstring(full_text);

        // if longest substring is aligned between seq and genome,
        // where on genome would seq start?
        backtrack_start = start_i + ss->seq;
        
        if (ss == NULL) {
            cout << "Found no matches for '" << full_text << "' at all.\n";
            cout << "Full row: " << row << endl;
        } else if (ss->tree == backtrack_start) {
            n_correct++;
        }

        n_total++;
        batch_size++;
    }

    batch_end = chrono::system_clock::now();
    elapsed_seconds = batch_end - batch_start;
    cout << "Matched " << batch_size << " genes from sample genome in " << elapsed_seconds.count() << " seconds\n";
    cout << "\n\nFinal Summary:\n";
    cout << "Correctly matched " << n_correct << " out of " << n_total << " genes.\n";
    cout << "Accuracy rate:  " << float(n_correct) / float(n_total) << endl;
    return 0;
}