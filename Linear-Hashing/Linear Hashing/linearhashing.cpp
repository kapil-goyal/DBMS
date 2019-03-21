#include <bits/stdc++.h>
using namespace std;

struct Bucket {
	list<int> bucket;
};	

class LinearHashing {
	int level;
	int bucketSize;
	int N;
	int splitPointer;
	vector<Bucket*> dir;
public:
	LinearHashing(int N, int bucketSize) {
		this->N = N;
		this->bucketSize = bucketSize;
		this->level = 0;
		this->splitPointer = 0;
		this->dir.resize(N);
		for (int i = 0; i < N; ++i)
		{
			this->dir[i] = new Bucket();
		}
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

	void insertElement(int val) {
		int index = getBucketIndex(val) ;
		if (this->dir[index]->bucket.size() % bucketSize == 0 && this->dir[index]->bucket.size() > 0) {
			increaseBucket();
			splitBucket(this->splitPointer);
		}
		index = getBucketIndex(val);
		this->dir[index]->bucket.push_back(val);		
	}

	bool searchElement(int val) {
		int index = getBucketIndex(val);
		if (find(this->dir[index]->bucket.begin(),
			this->dir[index]->bucket.end(), val) != this->dir[index]->bucket.end())
			return 1;
		return 0;
	}

	void increaseBucket() {
		int size = this->dir.size();
		this->dir.resize(size + 1);
		this->dir[size] = new Bucket();
	}

	void splitBucket(int bucketIndex) {
		list<int> &fb = this->dir[bucketIndex]->bucket;
		list<int> &sb = this->dir.back()->bucket;

		for (auto itr = fb.begin(); itr != fb.end();) {
			int dirSize = pow(2,this->level) * this->N;
			if ((*itr) % (2*dirSize) == this->dir.size()-1) {
				sb.push_back(*itr);
				itr = fb.erase(itr);
			}
			else {
				itr++;
			}
		}

		int dirSize = (pow(2,this->level) * this->N);
		this->splitPointer = (this->splitPointer + 1) % dirSize;
		if (this->splitPointer == 0) {
			this->level++;
		}

	}
	void printHashTable() {
		cout<<"\n ***** Hash Table ***** \n"<<endl;
		cout<<"noOfBuckets :: " << this->N <<endl;
		cout<<"pos :: " << this->splitPointer <<endl;
		cout<<"| Elements |"<<endl;
		for (auto temp : dir) {
			for (auto it : temp->bucket) {
				cout << it << " " ;
			}
			cout << "|" << endl;
		}
		cout << endl;
	} 	
};

int main() {
	int choice;
	LinearHashing *d=NULL;
	do{
		cout<<"1. Create new LinearHashTable"<<endl;
		cout<<"2. Add an element to the HashTable"<<endl;
		cout<<"3. Search an element to the HashTable"<<endl;
		cout<<"4. Print HashTable"<<endl;
		cout<<"Any other choice to exit"<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		cout<<"You entered "<<choice<<endl;
		switch(choice){
			case 1:
				if(d!=NULL)
					delete d;
				
				int noOfBuckets;
				cout<<"Enter Numner of Buckets: ";
				cin>>noOfBuckets;
				int bucketSize;
				cout<<"Enter number of entries in bucket: ";
				cin>>bucketSize;
				d= new LinearHashing(noOfBuckets, bucketSize);
				break;
			case 2:
				int add;
				cout<<"Enter element to add: ";
				cin>>add;
				d->insertElement(add);
				break;
			case 3:
				int search;
				cout<<"Enter element to search: ";
				cin>>search;
				if(d->searchElement(search))
					cout<<"Element is found"<<endl;
				else
					cout<<"Element is not found"<<endl;
				break;
			case 5:
				d->printHashTable();
				break;
			default:
				return 0;
		}
	}while(true);

}