#include <gtest/gtest.h>
#include "priorityqueue.h"
#include <map>
#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


TEST(priorityqueue, defaultConstructor) {
	priorityqueue<int> pq;
	EXPECT_EQ(pq.Size(), 0);
}

TEST(priorityqueue, defaultConstructor2) {
	priorityqueue<int> pq;
	EXPECT_EQ(pq.toString(), "");
}

TEST(priorityqueue, enqueueSimple) {
	priorityqueue<int> pq;
	stringstream ss;
	int count = 0;
	
	for (int i = 0; i < 100; i++) {
		pq.enqueue(i * 5, i);
		ss << i << " value: " << i * 5 << "\n";
		count++;
	}
	EXPECT_EQ(pq.Size(), count);
	EXPECT_EQ(pq.toString(), ss.str());
}

TEST(priorityqueue, enqueueIntsDups) {
	priorityqueue<int> pq;
	stringstream ss;
	pq.enqueue(10, 1); ss << 1 << " value: " << 10 << endl;
	pq.enqueue(20, 2); ss << 2 << " value: " << 20 << endl;
	pq.enqueue(30, 2); ss << 2 << " value: " << 30 << endl;
	pq.enqueue(40, 2); ss << 2 << " value: " << 40 << endl;
	pq.enqueue(50, 3); ss << 3 << " value: " << 50 << endl;
	pq.enqueue(60, 4); ss << 4 << " value: " << 60 << endl;
	ASSERT_EQ(pq.toString(), ss.str());
}

TEST(priorityqueue, enqueueDuplicates) {
	priorityqueue<string> pq;
	stringstream ss;
	
	pq.enqueue("Dolores", 5);
	pq.enqueue("Bernard", 4);
	pq.enqueue("Arnold", 8);
	pq.enqueue("Ford", 2);
	pq.enqueue("Jasmine", 6);
	pq.enqueue("Carlos", 11);
	pq.enqueue("William", 8);
	pq.enqueue("Teddy", 8);
	
	priorityqueue<string> pq2;
	pq2 = pq;
	// EXPECT_EQ(pq.Size(), pq2.Size());
	// pq.dequeue();
	// EXPECT_NE(pq.Size(), pq2.Size());
	// pq2.dequeue();
	// EXPECT_EQ(pq.Size(), pq2.Size());
	
	string value;
	int priority;
	
	pq.begin();
	while (pq.next(value, priority)) {
		ss << priority << " value: " << value << endl;
	}
	ss << priority << " value: " << value << endl;
	
	EXPECT_EQ(pq.toString(), ss.str());
	
	pq.clear();
	EXPECT_EQ(0, pq.Size());
}

TEST(priorityqueue, emmptyTreeEquality) {
	priorityqueue<int> pq1;
	priorityqueue<int> pq2;
	pq1 = pq2;
	ASSERT_TRUE(pq1 == pq2);
}

TEST(priorityqueue, intEquality) {
	priorityqueue<int> pq1;
	priorityqueue<int> pq2;
	
	pq1.enqueue(1, 1);
	pq2.enqueue(1, 1);
	pq1.enqueue(2, 2);
	pq2.enqueue(2, 2);
	pq1.enqueue(3, 3);
	pq2.enqueue(3, 3);
	ASSERT_EQ(pq1.Size(), pq2.Size());
	ASSERT_TRUE(pq1 == pq2);
}

TEST(priorityqueue, assignmentEquals) {
	priorityqueue<int> pq1;
	priorityqueue<int> pq2;
	
	pq1.enqueue(1, 1);
	pq1.enqueue(2, 2);
	pq1.enqueue(3, 3);
	pq2 = pq1;
	ASSERT_TRUE(pq1 == pq2);
	pq2.dequeue();
	ASSERT_FALSE(pq1 == pq2);
}

TEST(priorityqueue, toString) {
	map<int, vector<int> > map;
	int n = 9;
	int vals[] = {15, 16, 17, 6, 7, 8, 9, 2, 1};
	int prs[] = {1, 2, 3, 2, 2, 2, 2, 3, 3};
	priorityqueue<int> pq;
	
	for (int i = 0; i < n; i++) {
	    pq.enqueue(vals[i], prs[i]);
	    map[prs[i]].push_back(vals[i]);
	}
	EXPECT_EQ(pq.Size(), 9);
	stringstream ss;
	
	for (auto e: map) {
	    int priority = e.first;
	    vector <int> values = e.second;
	    for (size_t j = 0; j < values.size(); j++){
	        ss << priority << " value: " << values[j] << endl;
	    }
	}
	
	EXPECT_EQ(pq.toString(), ss.str());
}

TEST (priorityqueue, equalsOperator) {
	map<int, vector<int> > map;
	int n = 9;
	int vals[] = {15, 16, 17, 6, 7, 8, 9, 2, 1};
	int prs[] = {1, 2, 3, 2, 2, 2, 2, 3, 3};
	priorityqueue<int> pq;
	priorityqueue<int> pq2;
	
	for (int i = 0; i < n; i++) {
		pq.enqueue(vals[i], prs[i]);
		map[prs[i]].push_back(vals[i]);
	}
	EXPECT_EQ(pq.Size(), 9);
	pq2.enqueue(1, 1);
	pq2 = pq;
	EXPECT_EQ(pq2.Size(), 9);
	pq2.clear();
	EXPECT_FALSE(pq == pq2);
	pq2 = pq;
	EXPECT_TRUE(pq == pq2);
	stringstream ss;
	
	for (auto e: map) {
	    int priority = e.first;
	    vector <int> values = e.second;
	    for (size_t j = 0; j < values.size(); j++){
	        ss << priority << " value: " << values[j] << endl;
	    }
	}
	EXPECT_EQ(pq.toString(), ss.str());
	EXPECT_EQ(pq2.toString(), ss.str());
}

TEST (priorityqueue, nextDups) {
	priorityqueue<int> pq;
	stringstream ss;
	pq.enqueue(0, -1);
	pq.enqueue(1, -1);
	pq.enqueue(2, 0);
	pq.enqueue(3, 0);
	pq.enqueue(4, 1);
	int priority, value;
	pq.begin();
	while (pq.next(value, priority)) {
		ss << priority << " value: " << value << endl;
	}
	ss << priority << " value: " << value << endl;
	
	ASSERT_EQ(pq.toString(), ss.str());
	// priorityqueue<int> pq2;
	// pq2 = pq;
	// cout << pq.toString() << endl << endl;
	// for (int i = 0; i < 5; i++) {
	// 	cout << pq.dequeue() << endl;
	// }
	// cout << endl;
	// cout << pq2.toString() << endl << endl;
	// for (int i = 0; i < 5; i++) {
	// 	cout << pq2.dequeue() << endl;
	// }
}

TEST (priorityqueue, dequeueSimple) {
	priorityqueue<int> pq;
	pq.enqueue(1, 0);
	ASSERT_EQ(1, pq.Size());
	ASSERT_TRUE(pq.dequeue());
	ASSERT_FALSE(pq.dequeue());
}

TEST (priorityqueue, dequeueLong) {
	priorityqueue<int> pq;
	priorityqueue<int> pq2;
	srand(time(NULL));
	int value, priority;
	for (int i = 0; i < 10000; i++) {
		value = rand() % 200;
		priority = rand() % 100;
		pq.enqueue(value, priority);
		pq2.enqueue(value, priority);
	}
	EXPECT_TRUE(pq == pq2);
	pq2.clear();
	EXPECT_FALSE(pq == pq2);
	pq2 = pq;
	EXPECT_EQ(pq.toString(), pq2.toString());
	EXPECT_TRUE(pq2 == pq);
	// for (int i = 0; i < 10; i++) {
	// 	cout << pq.peek() << endl;
	// }
	for (int i = 0; i < 10000; i++) {
		pq.dequeue();
	}
}

