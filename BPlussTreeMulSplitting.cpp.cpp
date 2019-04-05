#include<bits/stdc++.h>
using namespace std;

class BTreeNode
{
	vector <int> keys;
	int leaf_size;
	int non_leaf_size;
	vector <BTreeNode *> C;
	BTreeNode * prv;
	BTreeNode * next;
	bool leaf;
public:
	int getCsize();
	int getkeyssize();
	BTreeNode* getC(int i);
	bool isleaf();
	BTreeNode(int leaf_size, int non_leaf_size, bool leaf);
	void traverse();
	void insert(int key, int* val1, BTreeNode* &newentry1, int* val2, BTreeNode* &newentry2);
	void split(int* val, BTreeNode* &newentry);
	void enter(BTreeNode* newentry, int val, int pos);
	bool isFull();
	BTreeNode* makenewroot(int val, BTreeNode* newentry);
	bool search(int key);
	BTreeNode* makenewroot2(int val1, BTreeNode* newentry1, int val2, BTreeNode* newentry2);
	int parent;
	int id;
};

int BTreeNode :: getkeyssize(){
	return keys.size();
}

bool BTreeNode :: search(int key){
	if (leaf == false){
		int i = 0;
		while(i < keys.size() && key >= keys[i])
			i++;
		return C[i]->search(key);
	}
	else{
        vector <int> :: iterator it = lower_bound(keys.begin(), keys.end(), key);
        return (*it == key); 
	}

}

int  BTreeNode :: getCsize(){
	return C.size();
}

BTreeNode*  BTreeNode :: getC(int i){
	return this->C[i];
}


bool BTreeNode :: isleaf(){
    return leaf;
}


bool BTreeNode :: isFull(){
    if (leaf)
    	return (keys.size() == leaf_size);
    else
    	return (keys.size() == non_leaf_size);
}

BTreeNode::BTreeNode(int leaf_size, int non_leaf_size, bool leaf) {
	this->leaf_size = leaf_size;
	this->non_leaf_size = non_leaf_size;
	this->leaf = leaf;
	this->next = NULL;
	this->prv = NULL;
	this->parent = 0;
	this->id = 0;
}


void BTreeNode::traverse() {
	int i;
	if (C.size() == 0){
		cout << "Contents ";
		for(int i = 0; i < keys.size(); i++)
			cout << keys[i] << " ";
		cout << endl;
		return;
	}
	cout << "Sequence of keys and children ";
	for(i = 0; i < keys.size(); i++){
		if (this->getC(i)->getCsize() == 0)
			cout << "data node" << " " << this->C[i]->id << ", ";
		else
			cout << "index node" << " " << this->C[i]->id << ", ";
		cout << keys[i] << " ";
	}
	if (this->C[i]->getCsize() == 0)
			cout << "data node" << " " << this->C[i]->id << ", ";
		else
			cout << "index node" << " " << this->C[i]->id << ", ";
	cout << endl;
	//string s;
	//for(int j = 0; j < tab; j++)
        //s += '\t';
	//for (i = 0; i < keys.size(); i++)
	//{
		//if (leaf == false)
			//C[i]->traverse(tab + 1);
//		cout << keys[i] << " "; //<< endl;
//	}

	//if (leaf == false)
		//C[i]->traverse(tab+1);
}

void BTreeNode :: enter(BTreeNode* newentry, int val, int pos){
    keys.insert(keys.begin() + pos, val);
    C.insert(C.begin() + pos + 1, newentry);
}

void BTreeNode :: split(int* val, BTreeNode* &newentry){
	int t = (non_leaf_size+1)/2;
	newentry = new BTreeNode(leaf_size, non_leaf_size, false);
	*val = this->keys[t];
	for(int i = t+1; i < 2*t; i++){
		newentry->keys.push_back(this->keys[i]);
	}
	this->keys.resize(t);
	for(int i = t+1; i <= 2*t; i++)
		newentry->C.push_back(this->C[i]);
	this->C.resize(t+1);
}

void BTreeNode:: insert(int key, int* val1, BTreeNode* &newentry1, int* val2, BTreeNode* &newentry2){
	if (leaf == false){
		int i = 0;
		while(i < keys.size() && key >= keys[i])
			i++;
		C[i]->insert(key, val1, newentry1, val2, newentry2);
		if (newentry1 == NULL)
			return;
		enter(newentry1, *val1, i);
		newentry1 = NULL;
		if (keys.size() <= non_leaf_size){
			if (newentry2 == NULL)
				return;			
			enter(newentry2, *val2, i+1);
			if (keys.size() <= non_leaf_size)
				newentry1 = NULL;
			else{
				split(val1, newentry1);
				//cout << *val1 << endl;
			}
			newentry2 = NULL;
		}
		else{
			split(val1, newentry1);
			if (newentry2 == NULL)
				return;
			if (*val1 <= *val2){
				i = 0;
				while(i < newentry1->keys.size() && key >= newentry1->keys[i])
					i++;
				newentry1->enter(newentry2, *val2, i);
				if (newentry1->keys.size() > non_leaf_size){
					newentry1->split(val2, newentry2);;
				}
				else
					newentry2 = NULL;
			}
			else{
				i = 0;
				while(i < this->keys.size() && key >= keys[i])
					i++;
				enter(newentry2, *val2, i);
				if (keys.size() > non_leaf_size){
					split(val2, newentry2);
				}
				else
					newentry2 = NULL;
			}
			
		}
	}
	else{
		vector <int> :: iterator it = lower_bound(keys.begin(), keys.end(), key);
		keys.insert(it, key);
		if (keys.size() <= leaf_size)
			return;
		newentry1 = new BTreeNode(leaf_size, non_leaf_size, true);
		newentry2 = new BTreeNode(leaf_size, non_leaf_size, true);
		if (keys.size() > leaf_size){
			int k = keys.size()/3;
			if (keys.size()%3 == 0){
				for(int i = k; i < 2*k; i++)
					newentry1->keys.push_back(this->keys[i]);
				for(int i = 2*k; i < 3*k; i++)
					newentry2->keys.push_back(this->keys[i]);
				this->keys.resize(k);
				*val1 = this->keys[k-1] + 1;
				*val2 = newentry1->keys[k-1] + 1;
			}
			else if (keys.size()%3 == 1){
				for(int i = k+1; i <= 2*k; i++)
					newentry1->keys.push_back(this->keys[i]);
				for(int i = 2*k+1; i <= 3*k; i++)
					newentry2->keys.push_back(this->keys[i]);
				this->keys.resize(k+1);
				*val1 = this->keys[k] + 1;
				*val2 = newentry1->keys[k-1] + 1;
			}
			else if (keys.size()%3 == 2){
				for(int i = k+1; i <= 2*k+1; i++)
					newentry1->keys.push_back(this->keys[i]);
				for(int i = 2*k+2; i < 3*k+2; i++)
					newentry2->keys.push_back(this->keys[i]);
				this->keys.resize(k+1);
				*val1 = this->keys[k] + 1;
				*val2 = newentry1->keys[k] + 1;
			}
		/*            
			int t = leaf_size/2;
		    newentry = new BTreeNode(leaf_size, non_leaf_size, true);
		    *val = this->keys[t+1];
		    for(int i = t+1; i <= 2*t; i++){
		        newentry->keys.push_back(this->keys[i]);
		    }
		    this->keys.resize(t+1);
		    newentry->next = this->next;
		    if (newentry->next)
		    	newentry->next->prv = newentry;
		    this->next = newentry;
		    newentry->prv = this;
		*/
		}
	}
}

class BTree{
    BTreeNode* root;
    int t;
    int leaf_size;
    int non_leaf_size;
    public:
    BTree(int leaf_size, int non_leaf_size);
    void insert(int key);
    void display();
    void search(int key);
};

void BTree :: search(int key){
	int status =  root->search(key);
	if (status)
		cout << key << " found in tree" << endl;
	else
		cout << key << " not found in tree" << endl;
}

BTree :: BTree(int leaf_size, int non_leaf_size){
    root = new BTreeNode(leaf_size, non_leaf_size, true);
    this-> leaf_size = leaf_size;
    this-> non_leaf_size = non_leaf_size;
}

BTreeNode* BTreeNode :: makenewroot(int val, BTreeNode* newentry){
    BTreeNode* root = new BTreeNode(leaf_size, non_leaf_size, false);
    root->keys.push_back(val);
    root->C.push_back(this);
    root->C.push_back(newentry);
    return root;
}

BTreeNode* BTreeNode :: makenewroot2(int val1, BTreeNode* newentry1, int val2, BTreeNode* newentry2){
    BTreeNode* root = new BTreeNode(leaf_size, non_leaf_size, false);
    root->keys.push_back(val1); 
    root->keys.push_back(val2);
    root->C.push_back(this);
    root->C.push_back(newentry1);
    root->C.push_back(newentry2);
    return root;
}

void BTree :: insert(int key){
    BTreeNode* newentry1 = NULL;
    int val1 = 0;
    BTreeNode* newentry2 = NULL;
    int val2 = 0;
    root->insert(key, &val1, newentry1, &val2, newentry2);
    if (newentry1 != NULL && newentry2 == NULL){
        root = root->makenewroot(val1, newentry1);
    }
    if (newentry1 != NULL && newentry2 != NULL){
        root = root->makenewroot2(val1, newentry1, val2, newentry2);
	if (root->getkeyssize() > non_leaf_size){
		root->split(&val1, newentry1);
		root = root->makenewroot(val1, newentry1);
	}
    }
}

void BTree :: display(){
    queue<pair<int,BTreeNode *> > q;
	int nodeID =0;
	int leafID = 0;
	queue <int> level;
	pair<int, BTreeNode*> curNode;
	q.push(make_pair(nodeID, root));
	level.push(-1);
	cout<<"\n*** NodeID :: Values ***\n"<<endl;
	while(!q.empty()){
		curNode = q.front(); q.pop();
		/*if(prevLevel!=level.front()){
			prevLevel = curLevel++;
		}*/
		int parent = level.front();
		level.pop();
		BTreeNode *node = curNode.second;
		if (node->getCsize() != 0){
			cout << "Index node " << curNode.first << " ";
			if (parent != -1)
				cout << "parent: index node " << parent << " ";
			else
				cout << "parent NULL ";
			for(int i=0; i < node->getCsize(); i++){
				if (node->getC(i)->getCsize() != 0){
					q.push(make_pair(++nodeID, node->getC(i)));
					node->getC(i)->id = nodeID;
					level.push(curNode.first);	
				}
				else{
					q.push(make_pair(++leafID, node->getC(i)));
					node->getC(i)->id = leafID;					
					level.push(curNode.first);
				}
			}
			node->traverse();
			continue;
			
		}
		cout << "Data node " << curNode.first << " ";
		if (parent != -1)
			cout << "parent: index node " << parent << " ";
		else
			cout << "parent NULL ";
		node->traverse();
		
		/*if(node->isleaf()){
			node->traverse();
			cout<<endl;
		}
		else{
			for(int i=0; i < node->getCsize(); i++){
				q.push(make_pair(++nodeID, node->getC(i)));
				level.push(curNode.first);
			}
			node->traverse();
			cout<<endl;
		}*/
	}
}

int main(){
    BTree* tree = NULL;
    int inp;
    cin >> inp;
    cout << "Please note that id are assigned to nodes at printing only, hence the id are not same as that in sample test case\n";
    while(inp != -1){
    	if (inp == 1){
    		int non_leaf_size;
    		int leaf_size;
    		cout << "Enter number of keys in index node and data node" << endl;
    		cin >> non_leaf_size >> leaf_size;
    		if (tree)
    			delete(tree);
    		tree = new BTree(leaf_size, non_leaf_size);
    	}
        else if (inp == 2){
            int x;
            cin >> x;
            tree->insert(x);
        }
        else if (inp == 3){
        	int x;
        	cin >> x;
        	tree->search(x);
        }
        else if (inp == 4){
            tree->display();
        }
        cin >> inp;
    }
    return 0;
}
