#include<bits/stdc++.h>
using namespace std;

class BTreeNode
{
	vector <int> keys;
	int t;
	vector <BTreeNode *> C;
	bool leaf;
public:
	BTreeNode(int t, bool leaf);
	void traverse(int tab);
	BTreeNode* search(int k);
	void insert(int key, int* val, BTreeNode* &newentry);
	void split(int* val, BTreeNode* &newentry);
	void enter(BTreeNode* newentry, int val, int pos);
	bool isFull();
	BTreeNode* makenewroot(int val, BTreeNode* newentry);
};

bool BTreeNode :: isFull(){
    return (this->keys.size() == 2*t - 1);
}

BTreeNode::BTreeNode(int t, bool leaf) {
	this->t = t;
	this->leaf = leaf;
}


void BTreeNode::traverse(int tab) {
	int i;
	string s;
	for(int j = 0; j < tab; j++)
        s += '\t';
	for (i = 0; i < keys.size(); i++)
	{
		if (leaf == false)
			C[i]->traverse(tab + 1);
		cout << s << keys[i] << endl;
	}

	if (leaf == false)
		C[i]->traverse(tab+1);
}

BTreeNode *BTreeNode::search(int k) {
	int i = 0;
	while (i < keys.size() && k > keys[i])
		i++;

	if (keys[i] == k)
		return this;

	if (leaf == true)
		return NULL;


	return C[i]->search(k);
}

void BTreeNode :: enter(BTreeNode* newentry, int val, int pos){
    keys.insert(keys.begin() + pos, val);
    C.insert(C.begin() + pos + 1, newentry);
	/*keys.push_back(val);
	C.push_back(newentry);
	int i = keys.size() - 2;
	while(i >= pos){
		swap(C[i+1], C[i+2]);
		swap(keys[i], keys[i+1]);
	}*/
}

void BTreeNode :: split(int* val, BTreeNode* &newentry){
	newentry = new BTreeNode(t, false);
	*val = this->keys[t];
	for(int i = t+1; i < 2*t; i++){
		newentry->keys.push_back(this->keys[i]);
	}
	this->keys.resize(t);
	for(int i = t+1; i <= 2*t; i++)
		newentry->C.push_back(this->C[i]);
	this->C.resize(t+1);
}

void BTreeNode:: insert(int key, int* val, BTreeNode* &newentry){
	if (leaf == false){
		int i = 0;
		while(i < keys.size() && key > keys[i])
			i++;
		C[i]->insert(key, val, newentry);
		if (newentry == NULL)
			return;
		if (keys.size() < 2*t - 1){
			enter(newentry, *val, i);
			newentry = NULL;
		}
		else{
			enter(newentry, *val, i);
			split(val, newentry);
		}
	}
	else{
        vector <int> :: iterator it = lower_bound(keys.begin(), keys.end(), key);
        keys.insert(it, key);
        if (keys.size() == 2*t){
            newentry = new BTreeNode(t, true);
            *val = this->keys[t];
            for(int i = t+1; i < 2*t; i++){
                newentry->keys.push_back(this->keys[i]);
            }
            this->keys.resize(t);
        }
	}
}

class BTree{
    BTreeNode* root;
    int t;
    public:
    BTree(int t);
    void insert(int key);
    void display();
};

BTree :: BTree(int t){
    root = new BTreeNode(t, true);
}

BTreeNode* BTreeNode :: makenewroot(int val, BTreeNode* newentry){
    BTreeNode* root = new BTreeNode(t, false);
    root->keys.push_back(val);
    root->C.push_back(this);
    root->C.push_back(newentry);
    return root;
}

void BTree :: insert(int key){
    BTreeNode* newentry = NULL;
    int val = 0;
    root->insert(key, &val, newentry);
    if (newentry != NULL){
        root = root->makenewroot(val, newentry);
    }
}

void BTree :: display(){
    root->traverse(0);
}

int main(){
    int t;
    cin >> t;
    BTree* tree = new BTree(t);
    int inp;
    cin >> inp;
    while(inp != -1){
        if (inp == 1){
            int x;
            cin >> x;
            tree->insert(x);
        }
        else if (inp == 2){
            tree->display();
        }
        cin >> inp;
    }
    return 0;
}
