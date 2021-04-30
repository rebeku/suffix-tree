// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/SuffixTree.h"

#include <iostream>
#include <string>
#include <queue>

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

TEST_F(test_SuffixTree, TestEdgeMatchComparison){
	edge_match a;
	a.s_chars = 5;
	edge_match b;
	b.s_chars = 3;
	edge_match c;
	c.s_chars = 10;

	priority_queue<edge_match, vector<edge_match>, std::greater<edge_match>> q;
	q.push(a);
	q.push(b);
	q.push(c);

	EXPECT_EQ(q.top().s_chars, b.s_chars);
	q.pop();

	EXPECT_EQ(q.top().s_chars, a.s_chars);
	q.pop();
	
	EXPECT_EQ(q.top().s_chars, c.s_chars);
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

	EXPECT_EQ(tree->edges[0]->text, "A");
	EXPECT_EQ(tree->edges[1]->text, "CA");
	EXPECT_EQ(tree->edges[2]->text, "GT$");
	EXPECT_EQ(tree->edges[3]->text, "T$");
	EXPECT_EQ(tree->edges[0]->dst->edges[0]->text, "CA");
	EXPECT_EQ(tree->edges[0]->dst->edges[0]->dst->edges[0]->text, "CAGT$");
	EXPECT_EQ(tree->edges[0]->dst->edges[0]->dst->edges[1]->text, "GT$");
	EXPECT_EQ(tree->edges[0]->dst->edges[1]->text, "GT$");
	EXPECT_EQ(tree->edges[1]->dst->edges[0]->text, "CAGT$");
	EXPECT_EQ(tree->edges[1]->dst->edges[1]->text, "GT$");
}

TEST_F(test_SuffixTree, TestFindTopSubstringPerfectMatch){
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	shared_ptr<substring> match;

	for (int i = 0; i<genome.length(); i++) {
		for (int j=i; j<genome.length(); j++) {
			string seq = genome.substr(i, j-i);
			match = mytree.FindTopSubstring(tree, seq);
			if (match == NULL) {
				continue;
			}
			EXPECT_EQ(match->length, j-i) << "Failed on i=" << i << " j=" << j << endl;
			
			string label = to_string(i) + "|" + to_string(j);

			// there are a bunch of duplicate substrings in this range
			// only the earliest occurrance in the genome will be returned.
			if (i < 2 || i > 4) {
				EXPECT_EQ(match->tree, i) << label;
			}
			EXPECT_EQ(match->seq, 0) << label;
		}
	}
}

TEST_F(test_SuffixTree, TestFindTopSubstringIrrelevantPrefix){
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	shared_ptr<substring> match;

	for (int i = 0; i<genome.length(); i++) {
		// every substring of 2 chars or more will be the best match
		for (int j=i+2; j<genome.length(); j++) {
			string seq = "GGG" + genome.substr(i, j-i);
			match = mytree.FindTopSubstring(tree, seq);
			string label = to_string(i) + "|" + to_string(j);
			// there are a bunch of duplicate substrings in this range
			// only the earliest occurrance in the genome will be returned.
			if (i < 2 || i > 4) {
				EXPECT_EQ(match->tree, i) << label;
			}
			EXPECT_EQ(match->seq, 3) << label;
		}
	}
}

TEST_F(test_SuffixTree, TestFindTopSubstringInvalidChars){
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	shared_ptr<substring> match;
	match = mytree.FindTopSubstring(tree, "FCAG");
	EXPECT_EQ(match->length, 3);
	EXPECT_EQ(match->seq, 1);
	EXPECT_EQ(match->tree, 3);


	match = mytree.FindTopSubstring(tree, "CAGBBBACACA");
	EXPECT_EQ(match->length, 5);
	EXPECT_EQ(match->seq, 6);
	EXPECT_EQ(match->tree, 0);


	match = mytree.FindTopSubstring(tree, "PFOOBR!");
	EXPECT_TRUE(match==NULL);
}

TEST_F(test_SuffixTree, TestFindTopNSubstrings) {
	SuffixTree mytree;

	string genome = "ACACAGT";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);
    
    EXPECT_TRUE(tree);

	string seq = "CACA";
	vector<shared_ptr<substring>> matches = mytree.FindTopNSubstrings(tree, seq, 3);
	ASSERT_EQ(matches.size(), 3);

	EXPECT_EQ(matches[0]->length, 4);
	EXPECT_EQ(matches[0]->seq, 0);
	EXPECT_EQ(matches[0]->tree, 1);

	EXPECT_EQ(matches[1]->length, 3);
	EXPECT_EQ(matches[1]->seq, 1);
	EXPECT_EQ(matches[1]->tree, 0);

	EXPECT_EQ(matches[2]->length, 2);
	EXPECT_EQ(matches[2]->seq, 2);
	EXPECT_EQ(matches[2]->tree, 3);
}

TEST_F(test_SuffixTree, TestFindBulkTopNSubstrings) {
	SuffixTree mytree;
	string genome = "CACGCAATGAGATGAGAGGTGTCTTCTCTGAAAGCACCGTTTAGAGTGGTGAGAATAAGTAGAGCTCACCGCGCAATGTTTGTCCACTAATTGGCTATAG";
	shared_ptr<s_tree> tree = mytree.BuildTree(genome);

	cout << "Built tree!\n";
    EXPECT_TRUE(tree);

	vector<string> sequences = {
		"GAGGAACGAGGTGTCTTCCCC",
		"AGAGCT",
		"ATGCAATGTTAGTCCACTAATTGGCTATAG",
		"CTTTAGAGTGGTGACGCGC",
		"TATTGGTGTCTTCTCTGT"
		};

	vector<shared_ptr<substring>> m;
	for (string seq: sequences) {
		m = mytree.FindTopNSubstrings(tree, seq, 3);
	}
	vector<vector<shared_ptr<substring>>> bulk_matches = mytree.FindBulkTopNSubstrings(tree, sequences, 3);
	ASSERT_EQ(bulk_matches.size(), 5);

	vector<shared_ptr<substring>> match;
	for (int i=0; i<5; i++) {
		match = bulk_matches[i];
		EXPECT_TRUE(match[0]->length >= match[1]->length);
		EXPECT_TRUE(match[1]->length >= match[2]->length);

		for (int j = 0; j< 3; j++) {
			cout << "i: " << i << " j: " << j << endl;
			cout << "length: " << match[j]->length;
			cout << " seq match: " << match[j]->seq << " tree match: " << match[j]->tree << endl << endl;
		}
	}

	shared_ptr<substring> expected(new substring(15,7,11));
	EXPECT_TRUE(bulk_matches[0][0]==expected);
	
	expected->length = 4;
	expected->tree = 15;
	expected->seq = 0;
	EXPECT_TRUE(bulk_matches[0][1] == expected);

	expected->length = 3;
	expected->seq = 5;
	expected->tree = 1;
	EXPECT_TRUE(bulk_matches[0][2] == expected);
	
	expected->length = 6;
	expected->seq = 0;
	expected->tree = 60;
	EXPECT_TRUE(bulk_matches[1][0] == expected);
	
	expected->length = 3;
	expected->seq = 3;
	expected->tree = 93;
	EXPECT_TRUE(bulk_matches[1][1] == expected);

	expected->length = 2;
	expected->seq = 4;
	expected->tree = 22;
	EXPECT_TRUE(bulk_matches[1][2] == expected);
	
	expected->length = 8;
	expected->seq = 2;
	expected->tree = 72;
	EXPECT_TRUE(bulk_matches[2][0] == expected);
	
	expected->length = 4;
	expected->seq = 8;
	expected->tree = 40;
	EXPECT_TRUE(bulk_matches[2][1] == expected);
	
	expected->length = 3;
	expected->seq = 7;
	expected->tree = 38;
	EXPECT_TRUE(bulk_matches[2][2] == expected);
	
	expected->length = 13;
	expected->seq = 1;
	expected->tree = 39;
	EXPECT_TRUE(bulk_matches[3][0] == expected);
	
	expected->length = 5;
	expected->seq = 14;
	expected->tree = 69;
	EXPECT_TRUE(bulk_matches[3][1] == expected);
	
	expected->length = 4;
	expected->seq = 13;
	expected->tree = 1;
	EXPECT_TRUE(bulk_matches[3][2] == expected);
	
	expected->length = 13;
	expected->seq = 4;
	expected->tree = 17;
	EXPECT_TRUE(bulk_matches[4][0] == expected);
	
	expected->length = 5;
	expected->seq = 1;
	expected->tree = 89;
	EXPECT_TRUE(bulk_matches[4][1] == expected);
	
	expected->length = 5;
	expected->seq = 3;
	expected->tree = 46;
	EXPECT_TRUE(bulk_matches[4][2] == expected);
}