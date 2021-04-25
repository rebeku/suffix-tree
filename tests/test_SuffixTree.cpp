// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/SuffixTree.h"

#include <iostream>
#include <string>

using namespace std;

void expect_in(vector<int> vec, int a, string label) {
	for (int b: vec) {
		if (b == a) {
			return;
		}
	}
	EXPECT_TRUE(false) << "Expected to find " << a << " in test case " << label;
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
	mytree.print(tree);
}

TEST_F(test_SuffixTree, TestFindTopSubstring){
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	shared_ptr<Substring> match;

	for (int i = 0; i<genome.length(); i++) {
		for (int j=i; j<genome.length(); j++) {
			string seq = genome.substr(i, j-i);
			cout << "seq: " << seq << endl;
			match = mytree.FindTopSubstring(tree, seq);
			if (match == NULL) {
				continue;
			}
			EXPECT_EQ(match->length, j-i) << "Failed on i=" << i << " j=" << j << endl;

			cout << "i: " << i << " j: " << j << "\nFound tree matches [";
			for (int i: match->tree) {
				cout << i << ", ";
			}
			cout << "]\n";
			string label = to_string(i) + "|" + to_string(j);
			expect_in(match->tree, i, label);
			expect_in(match->seq, 0, label);
		}
	}
}