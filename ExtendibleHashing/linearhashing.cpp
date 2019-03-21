#include <bits/stdc++.h>
using namespace std;

struct Bucket {
	list<int> bucket;
}

class LinearHashing {
	int level;
	int bucketSize;
	int N;
	int splitPointer;

	LinearHashing(int N, int bucketSize) {
		this->N = N;
		this->bucketSize = bucketSize;
		this->level = 0;
		this->splitPointer = 0;
	}

	int getBucketIndex(int val) {
		int dirSize = pow(2,this->level) * this->N;
		if (val % dirSize >= this->splitPointer) {
			return val % dirSize;
		}
		else {
			return val % (2 * dirSize);
		}
	}
}

int main() {

}