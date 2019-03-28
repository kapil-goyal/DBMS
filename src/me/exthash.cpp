#include <bits/stdc++.h>
using namespace std;
struct bucket {
	int localdepth;
	list<int> elements;
};
class extHash {
	int bucketsize;
	int globaldepth;
	vector<bucket*> dir;

public:
	void createHashTable(int globaldepth, int bucketsize) {
		this->bucketsize = bucketsize;
		this->globaldepth = globaldepth;
		this->dir.resize(pow(2,globaldepth));
		for (int i=0;i<this->dir.size();i++) {
			this->dir[i] = new bucket();
			this->dir[i]->localdepth = globaldepth;
		}
	}

	void insertElement(int val) {
		int globaldepthpower = pow(2,this->globaldepth);
		int bucketIndex = val % globaldepthpower;
		if (this->dir[bucketIndex]->elements.size() < this->bucketsize) {
			this->dir[bucketIndex]->elements.push_back(val);
			return;
		}
		if (this->dir[bucketIndex]->localdepth == this->globaldepth) {
			doubleHashTable();
			splitBucket(bucketIndex);
		}
		else {
			splitBucket(bucketIndex);
		}
		insertElement(val);
	}

	bool searchElement(int val) {
		int globaldepthpower = pow(2,this->globaldepth);
		int bucketIndex = val % globaldepthpower;
		if (find(this->dir[bucketIndex]->elements.begin(),
						 this->dir[bucketIndex]->elements.end(),
						 val) != this->dir[bucketIndex]->elements.end())
			return true;
		return false;
	}

	void doubleHashTable() {
		this->dir.resize(2*pow(2,this->globaldepth));
		int size = pow(2,this->globaldepth);
		for (int i = 0; i < size; ++i)
		{
			this->dir[i+size] = this->dir[i];
		}
		this->globaldepth++;
	}

	void splitBucket(int bucketIndex) {
		int localdepth = this->dir[bucketIndex]->localdepth;
		int localdepthPower = pow(2,localdepth);
		int firstbucketIndex = bucketIndex & (localdepthPower-1);
		int secondbucketIndex = firstbucketIndex + localdepthPower;

		this->dir[secondbucketIndex] = new bucket();
		this->dir[secondbucketIndex]->localdepth = ++this->dir[firstbucketIndex]->localdepth;
		
		for (auto itr = this->dir[firstbucketIndex]->elements.begin();
				itr != this->dir[firstbucketIndex]->elements.end();) {
			if (*itr & localdepthPower) {
				this->dir[secondbucketIndex]->elements.push_back(*itr);
				itr = this->dir[firstbucketIndex]->elements.erase(itr);
			}
			else {
				itr++;
			}
		}

		localdepthPower = 2*localdepthPower;
		for (auto i = secondbucketIndex+localdepthPower;
				i < this->dir.size(); i += localdepthPower) {
			this->dir[i] = this->dir[secondbucketIndex];
		}		
	}

	void printHashTable() {
		cout<<"\n ***** Hash Table ***** \n"<<endl;
		cout<<"globalDepth :: " << this->globaldepth<<endl;
		for (int i = 0; i < this->dir.size(); ++i)
		{
			int localdepth = this->dir[i]->localdepth;
			int localdepthPower = pow(2,localdepth);
			int firstbucketIndex = i & (localdepthPower-1);
			if (firstbucketIndex == i) {
				cout << i << "\t";
				cout<<this->dir[i]->localdepth<<" :: | ";
				for(auto l:this->dir[i]->elements)
					cout<<l<<" ";
				cout<<"|" <<endl;
			}
			else {
				cout << i << "\t";
				cout<<this->dir[i]->localdepth<<" :: | ";
				cout << "points to " << firstbucketIndex;
				cout<<"|" <<endl;
				// cout<<this->dir[i]->localdepth<<" :: | ";
				// for(auto l:this->dir[i]->elements)
				// 	cout<<l<<" ";
				// cout<<"|" <<endl;
			}
		}
		cout<<endl;
	}
};

int main() {
	int choice;
	extHash *d=NULL;
	do{
		cout<<"1. Create new ExtendibleHashTable"<<endl;
		cout<<"2. Add an element to the HashTable"<<endl;
		cout<<"3. Search an element to the HashTable"<<endl;
		cout<<"4. Delete an element from the HashTable"<<endl;
		cout<<"5. Print HashTable"<<endl;

		cout<<"Any other choice to exit"<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		cout<<"You entered "<<choice<<endl;
		switch(choice){
			case 1:
				if(d!=NULL)
					delete d;
				d= new extHash();
				int globalDepth;
				cout<<"Enter global depth: ";
				cin>>globalDepth;
				int bucketSize;
				cout<<"Enter number of entries in bucket: ";
				cin>>bucketSize;
				d->createHashTable(globalDepth,bucketSize);
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
			case 4:
				// int del;
				// cout<<"Enter element to delete: ";
				// cin>>del;
				// if(d->deleteElement(del))
				// 	cout<<"Element is deleted"<<endl;
				// else
				// 	cout<<"Element is not found"<<endl;
				break;
			case 5:
				d->printHashTable();
				break;
			default:
				return 0;
		}
	}while(true);
}