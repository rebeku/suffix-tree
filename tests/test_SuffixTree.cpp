// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/SuffixTree.h"

#include <iostream>
#include <string>

using namespace std;

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
	ASSERT_TRUE(b < a);
	ASSERT_TRUE(a < c);
	ASSERT_FALSE(c < b);
	ASSERT_TRUE(c > b);
}

TEST_F(test_SuffixTree, TestBuildTree){
    SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    ASSERT_TRUE(tree);

	for (const auto& e: tree->edges) {
		cout << e->text << endl;
	}
	ASSERT_EQ(tree->edges.size(), 4);
}

TEST_F(test_SuffixTree, TestFindTopSubstring){
    SuffixTree myobj;
    
    // ASSERT_EQ(NULL, myobj.FindTopSubstring());
}