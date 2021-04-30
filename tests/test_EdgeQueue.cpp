// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/EdgeQueue.h"

#include <iostream>
#include <string>
#include <queue>

using namespace std;

edge_match make_edge_match() {
    shared_ptr<s_tree> node (new s_tree);
    node->edges = vector<shared_ptr<edge>>{};
    node->starts = vector<int>{};

	shared_ptr<edge> e(new edge);
	e->text = "abc123";
	e->dst = node;

	edge_match em;
	em.matched = e;

	em.s_chars = 7;
	em.s_start = 0;
	return em;
}

class test_EdgeQueue : public ::testing::Test {
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

TEST_F(test_EdgeQueue, TestEdgePush){
	edge_match a = make_edge_match();
	a.s_chars = 5;
	edge_match b = make_edge_match();
	b.s_chars = 3;
	edge_match c = make_edge_match();
	c.s_chars = 10;
	edge_match d = make_edge_match();
	d.s_chars = 2;
	edge_match e = make_edge_match();
	e.s_chars = 8;

	EdgeQueue q(3);
	vector<shared_ptr<Substring>> ss;

	q.push(a);
	ss = q.toSubstrings();
	ASSERT_EQ(ss.size(), 1);
	EXPECT_EQ(a.s_chars, ss[0]->length );

	q.push(b);
	ss = q.toSubstrings();
	ASSERT_EQ(ss.size(), 2);
	EXPECT_EQ(a.s_chars, ss[0]->length);
	EXPECT_EQ(b.s_chars, ss[1]->length);
	
	q.push(c);
	ss = q.toSubstrings();
	ASSERT_EQ(ss.size(), 3);
	EXPECT_EQ(c.s_chars, ss[0]->length);
	EXPECT_EQ(a.s_chars, ss[1]->length);
	EXPECT_EQ(b.s_chars, ss[2]->length);

	q.push(d);
	ss = q.toSubstrings();
	ASSERT_EQ(ss.size(), 3);
	EXPECT_EQ(c.s_chars, ss[0]->length);
	EXPECT_EQ(a.s_chars, ss[1]->length);
	EXPECT_EQ(b.s_chars, ss[2]->length);


	q.push(e);
	ss = q.toSubstrings();
	ASSERT_EQ(ss.size(), 3);
	EXPECT_EQ(c.s_chars, ss[0]->length);
	EXPECT_EQ(e.s_chars, ss[1]->length);
	EXPECT_EQ(a.s_chars, ss[2]->length);
}

TEST_F(test_EdgeQueue, TestRedundantEdges){
	edge_match a = make_edge_match();
	a.s_chars = 10;
	a.s_start = 4;
	edge_match b = make_edge_match();
	b.s_chars = 9;
	b.s_start = 5;
	edge_match c = make_edge_match();
	c.s_chars = 8;
	c.s_start = 6;
	edge_match d = make_edge_match();
	d.s_chars = 7;
	d.s_start = 7;
	edge_match e = make_edge_match();
	e.s_chars = 6;
	e.s_start = 8;

	EdgeQueue q(3);
	vector<shared_ptr<Substring>> ss;

	q.push(a);
	EXPECT_EQ(q.toSubstrings().size(), 1);
	EXPECT_EQ(q.toSubstrings()[0]->length, 10);
	
	q.push(b);
	EXPECT_EQ(q.toSubstrings().size(), 1);
	EXPECT_EQ(q.toSubstrings()[0]->length, 10);

	q.push(c);
	EXPECT_EQ(q.toSubstrings().size(), 1);
	EXPECT_EQ(q.toSubstrings()[0]->length, 10);

	q.push(d);
	EXPECT_EQ(q.toSubstrings().size(), 1);
	EXPECT_EQ(q.toSubstrings()[0]->length, 10);

	q.push(e);
	EXPECT_EQ(q.toSubstrings().size(), 1);
	EXPECT_EQ(q.toSubstrings()[0]->length, 10);
}