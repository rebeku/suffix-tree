// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/SuffixTree.h"

#include <iostream>
#include <string>

using namespace std;

bool expect_in(vector<int> vec, int a, string label) {
	for (int b: vec) {
		if (b == a) {
			return true;
		}
	}
	return false;
}

class test_SuffixTree : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
	}
    
	// this function runs before every TEST_F function
	void SetUp() override {
    }

	// this function runs after every TEST_F function
	void TearDown() override {
	}
};

TEST_F(test_SuffixTree, TestEdgeComparison){
	shared_ptr<edge> a (new edge);
	a->text = "ACA";
	shared_ptr<edge> b (new edge);
	b->text = "AAAAAA";
	shared_ptr<edge> c (new edge);
	c->text = "C";
	EXPECT_TRUE(b < a);
	EXPECT_TRUE(a < c);
	EXPECT_FALSE(c < b);
	EXPECT_TRUE(c > b);
}

TEST_F(test_SuffixTree, TestBuildTree){
    SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    EXPECT_TRUE(tree);
	EXPECT_EQ(tree->edges.size(), 4);

	for (const auto& e: tree->edges) {
		cout << "Edge has text: " << e->text << endl;
		
	}
}

TEST_F(test_SuffixTree, TestFindTopSubstringPerfectMatch){
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	shared_ptr<Substring> match;

	for (int i = 0; i<genome.length(); i++) {
		for (int j=i; j<genome.length(); j++) {
			string seq = genome.substr(i, j-i);
			match = mytree.FindTopSubstring(tree, seq);
			if (match == NULL) {
				continue;
			}
			EXPECT_EQ(match->length, j-i) << "Failed on i=" << i << " j=" << j << endl;
			
			string label = to_string(i) + "|" + to_string(j);
			EXPECT_TRUE(expect_in(match->tree, i, label));
			EXPECT_TRUE(expect_in(match->seq, 0, label));
		}
	}
}

TEST_F(test_SuffixTree, TestFindTopSubstringIrrelevantPrefix){
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	shared_ptr<Substring> match;

	for (int i = 0; i<genome.length(); i++) {
		// every substring of 2 chars or more will be the best match
		for (int j=i+2; j<genome.length(); j++) {
			string seq = "GGG" + genome.substr(i, j-i);
			match = mytree.FindTopSubstring(tree, seq);
			cout << "seq: " << seq << endl;
			cout << "i: " << i << " j: " << j << " match->length: " << match->length;
			cout << "\nFound tree matches [";
			for (int i: match->tree) {
				cout << i << ", ";
			}
			cout << "]\n";
			
			cout << "\nFound seq matches [";
			for (int i: match->seq) {
				cout << i << ", ";
			}
			cout << "]\n";

			string label = to_string(i) + "|" + to_string(j);
			EXPECT_TRUE(expect_in(match->tree, i, label));
			EXPECT_TRUE(expect_in(match->seq, 3, label));

			// seq = "INVALID_CHARS" + seq;
			// match = mytree.FindTopSubstring(tree, seq);
			// EXPECT_EQ(match, NULL);
		}
	}
}

TEST_F(test_SuffixTree, TestFindTopSubstringInvalidChars){
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	shared_ptr<Substring> match;
	match = mytree.FindTopSubstring(tree, "FCAG");
	EXPECT_EQ(match->length, 3);
	EXPECT_EQ(match->seq[0], 1);
	EXPECT_EQ(match->tree[0], 3);


	match = mytree.FindTopSubstring(tree, "CAGBBBACACA");
	EXPECT_EQ(match->length, 5);
	EXPECT_EQ(match->seq[0], 6);
	EXPECT_EQ(match->tree[0], 0);


	match = mytree.FindTopSubstring(tree, "PFOOBR!");
	EXPECT_TRUE(match==NULL);
}