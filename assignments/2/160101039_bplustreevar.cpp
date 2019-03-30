#include<iostream>
#include<vector>
#include<list>
#include<algorithm>
using namespace std;

int iid = 0;
int did = 0;

struct indexNode {
	int id = 0;
	vector<int> keys;
	vector<void *> children;
	int maxsize;
	indexNode *parent;

	indexNode(int n) {
		id = iid++;
		maxsize = n;
		parent = NULL;
	}

	void print(int level, int depth, vector<int> &ids) {
		cout << "Index node " << id << ":" << " Parent: ";
		if (!parent)
			cout << "NULL ";
		else
			cout << "Index Node " << parent->id << " ";

		cout << "Sequence of keys and children: ";
		auto it1 = keys.begin();
		auto it2 = ids.begin();
		while (1) {
			if (it2 != ids.end()) {
				if (level < depth-1) {
					cout << "Index Node " << (*it2) << ", ";
				}
				else {
					cout << "Data Node " << (*it2) << ", ";	
				}
				it2++;
			}
			else 
				break;
			if (it1 != keys.end()) {
				cout << (*it1) << ", ";
				it1++;
			}
		}cout << endl;
	}

	vector<int>::iterator insert(int key) {
		auto it = lower_bound(keys.begin(), keys.end(), key);
		if (it == keys.end()) {
			return keys.insert(keys.end(), key);
		}
		return keys.insert(it, key);
	}

};

struct dataNode {
	int id = 0;
	vector<int> keys;
	dataNode *left, *right;
	int maxsize;
	indexNode *parent;

	dataNode(int n) {
		id = did++;
		maxsize = n;
		left = NULL;
		right = NULL;
		parent = NULL;
	}

	void print() {
		cout << "Data Node " << id << ": Parent: ";
		if (!parent) {
			cout << "NULL ";
		}
		else {
			cout << "Index Node " << parent->id << " "; 
		}
		cout << " Contents: ";
		for (auto it: keys) {
			cout << it << " " ;
		}cout << "\n";
	}

	vector<int>::iterator insert(int key) {
		auto it = lower_bound(keys.begin(), keys.end(), key);
		if (it == keys.end()) {
			return keys.insert(keys.end(), key);
		}
		return keys.insert(it, key);
	}
};

class bptree {
	void *root;
	int numIndex;
	int numData;
	int depth;

public:
	bptree(int a, int b){
		numIndex = a;
		numData = b;
		depth = 0;
		root = new dataNode(b);
	}

	void print() {
		list<pair<void*, int>> q;
		q.push_back({root,0});
		while (!q.empty()) {
			pair<void *, int> F = q.front();
			q.pop_front();

			if (F.second == depth) {
				dataNode *u = (dataNode *)F.first;
				u->print();
			}
			else {
				indexNode *u = (indexNode *)F.first;
				vector<int> ids;
				for (auto child: u->children) {
					q.push_back({child, F.second+1});
					if (F.second == depth-1)
					ids.push_back(((dataNode *)child)->id);
					else 
					ids.push_back(((indexNode *)child)->id);
				}
				u->print(F.second, depth, ids);
				
			}

		}
		cout << endl;
	}


	void insert(int key) {
		insert_into_tree(root, key, 0);
	}

	void insert_into_tree(void *node, int key, int level) {
		if (level == depth) {
			dataNode *u = (dataNode *)node;
			if ((int)u->keys.size() == numData) {
				u->insert(key);
				root = splitLeaf(u);
			}
			else {
				u->insert(key);
			}
		}
		else {
			indexNode *u = (indexNode *)node;
			auto it = lower_bound(u->keys.begin(), u->keys.end(), key) - u->keys.begin();
			insert_into_tree(u->children[it], key, level+1);
		}
	}

	void *splitIndex(indexNode* node, int level) {
		if ((int)node->keys.size() <= numIndex)
			return root;

		int pos = (node->keys.size()-1) / 2;
		indexNode* par = node->parent;

		indexNode* left = new indexNode(numIndex);
		iid--; 
		indexNode* right = new indexNode(numIndex); 
		left->keys.insert(left->keys.begin(), node->keys.begin(), node->keys.begin() + pos+1);
		right->keys.insert(right->keys.begin(), node->keys.begin()+pos+2, node->keys.end());
		left->children.insert(left->children.begin(), node->children.begin(), node->children.begin() + pos+2);
		right->children.insert(right->children.begin(), node->children.begin()+pos+2, node->children.end());

		if (level < depth-1) {
			for (auto child: left->children) {
				((indexNode *)child)->parent = left;
			}
			for (auto child: right->children) {
				((indexNode *)child)->parent = right;
			}	
		}
		else {
			for (auto child: left->children) {
				((dataNode *)child)->parent = left;
			}
			for (auto child: right->children) {
				((dataNode *)child)->parent = right;
			}
		}

		if (par == NULL) {
			par = new indexNode(numIndex);
			par->insert(node->keys[pos+1]);
			par->children.push_back(left);
			par->children.push_back(right);
			left->id = node->id;
			delete node;
			left->parent = par;
			right->parent = par;
			depth++;
			return par;
		}
		else {
			par->insert(node->keys[pos+1]);
			auto it1 = find(par->children.begin(), par->children.end(), node);
			it1 = par->children.erase(it1);
			par->children.insert(it1, right);
			par->children.insert(it1,left);
			left->id = node->id;
			delete node;
			left->parent = par;
			right->parent = par;
			return splitIndex(par, level-1);
		}
	}

	void *splitLeaf(dataNode *node) {
		dataNode *R;
		root = splitleaf1(node,R);
		root = splitleaf2(R);
		return root;
	}

	void *splitleaf1(dataNode *node, dataNode * &R) {
		int pos = node->keys.size() / 3;
		if (node->keys.size() % 3 == 0)
			pos--;
		
		indexNode* par = node->parent;
		dataNode* left = new dataNode(numData); 
		did--;
		dataNode* right = new dataNode(numData); 
		left->keys.insert(left->keys.begin(), node->keys.begin(), node->keys.begin() + pos+1);
		right->keys.insert(right->keys.begin(), node->keys.begin()+pos+1, node->keys.end());
		left->left = node->left;
		left->right = right;
		right->right = node->right;
		right->left = left;

		if (par == NULL) {
			par = new indexNode(numIndex);
			par->insert(node->keys[pos] + 1);
			par->children.push_back(left);
			par->children.push_back(right);
			left->id = node->id;
			delete node;
			left->parent = par;
			right->parent = par;	
			depth++;
			R = right;
			return par;
		}
		else {
			par->insert(node->keys[pos] + 1);
			auto it1 = find(par->children.begin(), par->children.end(), node);
			it1 = par->children.erase(it1);
			par->children.insert(it1,right);
			par->children.insert(it1,left);
			left->id = node->id;
			delete node;
			left->parent = par;
			right->parent = par;
			R = right;
			return splitIndex(par, depth-1);
		}

	}

	void *splitleaf2(dataNode *node) {
		int pos = node->keys.size() / 2;
		if (node->keys.size() % 2 == 0)
			pos--;
		
		indexNode* par = node->parent;
		dataNode* left = new dataNode(numData);
		did--;
		dataNode* right = new dataNode(numData); 
		left->keys.insert(left->keys.begin(), node->keys.begin(), node->keys.begin() + pos+1);
		right->keys.insert(right->keys.begin(), node->keys.begin()+pos+1, node->keys.end());
		left->left = node->left;
		left->right = right;
		right->right = node->right;
		right->left = left;

		if (par == NULL) {
			par = new indexNode(numIndex);
			par->insert(node->keys[pos] + 1);
			par->children.push_back(left);
			par->children.push_back(right);
			left->id = node->id;
			delete node;
			left->parent = par;
			right->parent = par;	
			depth++;
			return par;
		}
		else {
			par->insert(node->keys[pos] + 1);
			auto it1 = find(par->children.begin(), par->children.end(), node);
			it1 = par->children.erase(it1);
			par->children.insert(it1,right);
			par->children.insert(it1,left);
			left->id = node->id;
			delete node;
			left->parent = par;
			right->parent = par;
			return splitIndex(par, depth-1);
		}

	}

	bool search(int key) {
		return search_tree(root, key, 0);
	}

	bool search_tree(void *node, int key, int level) {
		if (level == depth) {
			dataNode *u = (dataNode *)node;
			auto it = lower_bound(u->keys.begin(), u->keys.end(), key);
			if (it == u->keys.end() || *it != key) 
				return false;
			return true;
		}
		indexNode *u = (indexNode *)node;
		auto it = lower_bound(u->keys.begin(), u->keys.end(), key) - u->keys.begin();
		return search_tree(u->children[it], key, level+1);
	}
};

int main() {
	bptree* tree = NULL;
 	int key,choice;
 	int numIndexPointers, numDataPointers;
  do {
    cout << "1. Create new tree" << endl;
    cout << "2. Add new key to the tree" << endl;
    cout << "3. Search a key in the tree" << endl;
    cout << "4. Postorder traversal of the tree" << endl;
    cout << "Any other choice to exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cout << "You entered " << choice << endl;

    switch(choice){
      case 1:
        if(tree != NULL){
        	iid = 0;
        	did = 0;
          delete tree;
        }
        cout<<"Enter number of Index pointers: ";
        cin>>numIndexPointers;
        cout<<"Enter number of Data pointers: ";
        cin>>numDataPointers;
        if(numIndexPointers%2==0 || numDataPointers%2){
        	cout<<"Note:: Index pointers must be in odd number and Data pointers must be in even number"<<endl;
        	exit(0);
        }
        tree = new bptree(numIndexPointers, numDataPointers);
        break;
      
      case 2:
        if(tree == NULL){
          cout << "Create a new tree first" << endl;
          break;
        }
        cout << "Enter key to add ";
        cin >> key;
        tree->insert(key);
        break;
      case 3:
        if(tree == NULL){
          cout << "Create a new tree first" << endl;
          break;
        }
        cout << "Enter key to search: ";
        cin >> key;
        {
        	bool chk = tree->search(key);
	        if(!chk){
	           cout << "Key " << key << " not found." << endl;
	        }
	        else{
	          cout << "Key " << key << " found" << endl;
	        }
	    }
        break;
      case 4:
        if(tree == NULL){
          cout << "Create a new tree first" << endl;
          break;
        }
        tree->print();
        break;

      default:
        return 0;
    }
  }
  while(true);
	return 0;

}