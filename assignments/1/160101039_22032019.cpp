#include<iostream>
#include<vector>
#include<list>
#include<unordered_set>

using namespace std;

class Bucket{
public:
	int localDepth;
	list<int> bucket;
};
class ExtendibleHashing{
	int globalDepth,bucketSize, overflowBuckets;
	unordered_set<Bucket*> allbuckets, overBuckets;
	vector<Bucket*> dir;
public:
	void createHashTable(int globalDepth,int bucketSize);
	void insertElement(int val);
	bool searchElement(int val);
	bool deleteElement(int val);
	// bool checkOverflowbuckets();
	int getBucketIndex(int val);
	void doubleHashTable();
	void splitBucket(int bucketIndex);
	int twoPower(int p);
	void printHashTable();
};
// bool ExtendibleHashing::checkOverflowbuckets() {

// }
void ExtendibleHashing::createHashTable(int globalDepth,int bucketSize){
	int buckets = twoPower(globalDepth);
	this->globalDepth = globalDepth;
	this->dir.resize(buckets);
	this->bucketSize=bucketSize;
	this->overflowBuckets = 0;
	for(int i=0;i<buckets;i++){
		this->dir[i] = new Bucket();
		this->allbuckets.insert(this->dir[i]);
		this->dir[i]->localDepth = globalDepth;
	}
}

int ExtendibleHashing::getBucketIndex(int val){
	int lsbOfOnes = twoPower(this->globalDepth)-1;
	return val & lsbOfOnes;  
}

void ExtendibleHashing::insertElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	if(this->dir[bucketIndex]->bucket.size() >= this->bucketSize){
		if(this->globalDepth == this->dir[bucketIndex]->localDepth){
			if (this->dir[bucketIndex]->bucket.size() > bucketSize) {
				this->dir[bucketIndex]->bucket.push_back(val);		
				return;
			}
			else if (this->allbuckets.size()-1 == this->overBuckets.size()) {
				int size = twoPower(globalDepth);
				this->doubleHashTable();

				this->allbuckets.clear();
				this->overBuckets.clear();
				for (int i = 0; i < size; i++) {
					if (this->allbuckets.find(this->dir[i]) != this->allbuckets.end())
						continue;
					this->splitBucket(i);
					this->allbuckets.insert(this->dir[i]);
					// if (this->dir[i]->bucket.size() > bucketSize) {
					// 	this->overBuckets.insert(this->dir[i]);
					// }
				}
				for (int i = 0; i < 2*size; i++) {
					// if (this->allbuckets.find(this->dir[i]) != this->allbuckets.end())
					// 	continue;
					// this->allbuckets.insert(this->dir[i]);
					if (this->dir[i]->bucket.size() > bucketSize) {
						this->overBuckets.insert(this->dir[i]);
					}
				}

				// this->splitBucket(bucketIndex);
			}
			else {
				this->overBuckets.insert(this->dir[bucketIndex]);
				this->dir[bucketIndex]->bucket.push_back(val);
				return;
			}
			
		}
		else{
			this->splitBucket(bucketIndex);
		}
		this->insertElement(val);
	}
	else{
		this->dir[bucketIndex]->bucket.push_back(val);
	}
}

bool ExtendibleHashing::searchElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	for(auto itr = this->dir[bucketIndex]->bucket.begin(); itr!=this->dir[bucketIndex]->bucket.end(); itr++)
		if(*itr==val)
			return true;
	return false;
}

bool ExtendibleHashing::deleteElement(int val){
	int bucketIndex = this->getBucketIndex(val);
	for(auto itr = this->dir[bucketIndex]->bucket.begin(); itr!=this->dir[bucketIndex]->bucket.end(); itr++){
		if(*itr==val){
			this->dir[bucketIndex]->bucket.erase(itr);
			return true;
		}
	}
	return false;
}
void ExtendibleHashing::doubleHashTable(){
	int size = this->dir.size();
	this->globalDepth++;
	this->dir.resize(2*size);
	for(int i=size;i<2*size;i++)
		this->dir[i]=this->dir[i-size];
}

void ExtendibleHashing::splitBucket(int bucketIndex){
	int localDepthPower = twoPower(this->dir[bucketIndex]->localDepth);
	int firstBucketIndex = bucketIndex & (localDepthPower-1);
	int secondBucketIndex = firstBucketIndex + localDepthPower;
	/*creating new bucket and updating local depths*/
	this->dir[secondBucketIndex] = new  Bucket();
	this->dir[secondBucketIndex]->localDepth = ++this->dir[firstBucketIndex]->localDepth;
	this->allbuckets.insert(this->dir[secondBucketIndex]);
	
	list<int> &firstBucket = this->dir[firstBucketIndex]->bucket;
	list<int> &secondBucket = this->dir[secondBucketIndex]->bucket;
	/*distributing values to two buckets*/
	// bool chk = 0;
	// if (firstBucket.size() > this->bucketSize) {
	// 	chk = 1;
	// }
	for(auto itr=firstBucket.begin();itr!=firstBucket.end();){
		if(*itr & localDepthPower){
			secondBucket.push_back(*itr);
			itr = firstBucket.erase(itr);
		}
		else itr++;
	}
	// if (chk) {
	// 	if (firstBucket.size() <= this->bucketSize) {
	// 		this->overflowBuckets--;
	// 	}
	// 	if (secondBucket.size() > this->bucketSize) {
	// 		this->overflowBuckets++;
	// 	}
	// }
	/*pointing to proper buckets after spliting*/
	localDepthPower = 2*localDepthPower;
	for(int i=secondBucketIndex+localDepthPower; i < this->dir.size(); i=i+localDepthPower)
		this->dir[i] = this->dir[secondBucketIndex];
}

int ExtendibleHashing::twoPower(int p){
	return 1<<p;
}

void ExtendibleHashing::printHashTable(){
	cout<<"\n ***** Hash Table ***** \n"<<endl;
	cout<<"globalDepth :: " << this->globalDepth<<endl;
	unordered_set<Bucket*> uset;
	cout<<"index\tlocalDepth ::| Elements |"<<endl;
	// for(int i=0; i<this->dir.size(); i++){
	// 	if(uset.find(this->dir[i])!=uset.end())
	// 		continue;
	// 	cout<<this->dir[i]->localDepth<<" :: | ";
	// 	uset.insert(this->dir[i]);
	// 	for(auto l:this->dir[i]->bucket)
	// 		cout<<l<<" ";
	// 	cout<<"|" <<endl;
	// }
	// cout<<endl;
	for (int i = 0; i < this->dir.size(); ++i)
	{
		int localDepth = this->dir[i]->localDepth;
		int localDepthPower = twoPower(localDepth);
		int firstbucketIndex = i & (localDepthPower-1);
		if (firstbucketIndex == i) {
			cout << i << "\t";
			cout<<this->dir[i]->localDepth<<" :: | ";
			for(auto l:this->dir[i]->bucket)
				cout<<l<<" ";
			cout<<"|" <<endl;
		}
		else {
			cout << i << "\t";
			cout<<this->dir[i]->localDepth<<" :: | ";
			cout << "points to " << firstbucketIndex;
			cout<<"|" <<endl;
			// cout<<this->dir[i]->localDepth<<" :: | ";
			// for(auto l:this->dir[i]->bucket)
			// 	cout<<l<<" ";
			// cout<<"|" <<endl;
		}
	}
	cout<<endl;
}


int main(){
	
	int choice;
	ExtendibleHashing *d=NULL;
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
				d= new ExtendibleHashing();
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
				int del;
				cout<<"Enter element to delete: ";
				cin>>del;
				if(d->deleteElement(del))
					cout<<"Element is deleted"<<endl;
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