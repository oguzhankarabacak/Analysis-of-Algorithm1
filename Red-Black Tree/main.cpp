#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream> 
using namespace std; 

//red->0 , black->1
void swap(int &a,int &b){ //swap function for color
	int temp=a;
	a=b;
	b=temp;
}

struct Node {   //node
	string name;  //variables
	string season;
	string team;
	int rebound;
	int assist;
	int point;
	int color;   //red:0 , black : 1
	Node *left;  //left child
	Node *right;  //right child 
	Node *parent;  //parent 
}; 
void bst_preorder(Node *root,int depth) {   //preorder traversal for Binary Search Tree
	if (root == NULL) 	return;   //if root==NULL finish func
	
	if (root->color == 1) {
		for (int i=0;i<depth;i++) cout << "-";  //print depth using '-'
		cout << "(BLACK) " << root->name << endl;  //print name and black color
	}
	else{
		for (int i=0;i<depth;i++) cout << "-";  //print depth using '-'
		cout << "(RED) " << root->name << endl; //print red color and name
	}
	depth+=1; //increase depty by 1
	bst_preorder(root->left,depth);   //preorder is node->left->right so prince left tree 
	bst_preorder(root->right,depth); 	//preorder is node->left->right so prince right tree 
}

Node* bst_search(Node* root,const string &name){  //search for update
	if (root == NULL || root->name==name) return root; //if exists return root or return NULL
	if (root->name < name) return bst_search(root->right,name); //if node->name < name , search in right tree of root and return
	return bst_search(root->left,name); // if node->name > name , search in left treen of root and return
}



Node* bst_insert(Node* root, Node *new_node) { //bst_insert is to add new node to tree and return Node*
	if (root == NULL)  return new_node; //if tree is empty return new_node
	if (new_node->name > root->name)  //if new_node->name > root->name
	{ 
		root->right = bst_insert(root->right,new_node);  //call recursive func for right tree of root 
		root->right->parent = root; //and right child parent is root
	} 
	else if (new_node->name < root->name)  //if new_node->name < root->name
	{ 
		root->left = bst_insert(root->left, new_node); //call recursive func for left tree of root 
		root->left->parent = root; //and left child parent is root
	} 
	return root; 
} 
 
void binary_find_max(Node* root,Node* max_root,const string &type){  //find max in tree according to type and in same season
	if (root == NULL) return;  //if tree is empty
	
	if (type=="point"){ //if max type is point
		if (max_root->point< root->point){ //if max point is smaller than node point 
			max_root->point = root->point; //max_point=node point
			max_root->name = root->name; //owner name of max_point = name of node
		}
	}
	else if (type=="rebound"){  //if max type is rebound
		if (max_root->rebound < root->rebound){ //if max rebound is smaller than node rebound
			max_root->rebound = root->rebound; //max_rebound = node rebound
			max_root->name=root->name; //owner name of max_rebound = name of node
		}
	}
	else if (type=="assist"){  //if max type is assist
		if (max_root->assist < root->assist){ //if max assist is smaller than node assist 
			max_root->assist = root->assist; //max_assist = node assist
			max_root->name=root->name; //owner name of max_assist = name of node
		}
	}
		
	binary_find_max(root->left,max_root,type);  //recursive for left tree 
	binary_find_max(root->right,max_root,type);  //recursive for right tree
}

void left_rotate(Node *&root, Node *&nodep) { //left_rotate function
	Node *nodep_right = nodep->right; 
	nodep->right = nodep_right->left; //node right child is node->right->left   
	
	if (nodep->right != NULL) nodep->right->parent = nodep;  // if node->right is not NULL,node->right->parent is node 
	
	nodep_right->parent = nodep->parent;  //node->right->parent=node->parent
	
	if (nodep->parent == NULL) root = nodep_right; //if node->parent is NULL, root is node->right 
	else if (nodep == nodep->parent->left) nodep->parent->left = nodep_right;  //if node is node->parent->left , node->parent->left is node->right
	else nodep->parent->right = nodep_right; //else node->parent->right is node->right
	
	nodep_right->left = nodep;  //node->right->left is node
	nodep->parent = nodep_right;  //node->parent = node->right
} 

void right_rotate(Node *&root, Node *&nodep) //right rotate function
{ 
	Node *nodep_left = nodep->left; 
	nodep->left = nodep_left->right; //node left child is node->left->right   

	if (nodep->left != NULL) nodep->left->parent = nodep;  // if node->left is not NULL,node->left->parent is node 

	nodep_left->parent = nodep->parent;  //node->left->parent=node->parent

	if (nodep->parent == NULL) root = nodep_left;  //if node->parent is NULL, root is node->left 
	else if (nodep == nodep->parent->left) 	nodep->parent->left = nodep_left;  //if node is node->parent->left , node->parent->left is node->left
	else nodep->parent->right = nodep_left;   //else node->parent->right is node->left

	nodep_left->right = nodep;  //node->left->right is node
	nodep->parent = nodep_left; //node->parent = node->left
} 

class RedBlackTree { //red black tree class
	private: 
		Node *root;  //root of tree
		
	public: 
		void again_balance(Node *&, Node *&); //again balance after insert
		RedBlackTree() { root = NULL;} //constructor
		void insert(const string &,const string &,const string &,const int &,const int &,const int &); //insert new player
		void preorder_traversal(){
			int depth=0; //first depth is 0
			bst_preorder(this->root,depth); //call bst_preorder with root of tree
		}; //preorder_traversal func
		Node* search(const string& name){	return bst_search(this->root,name);	};  //search_name
		Node* find_max(const string &type){ //find_max according to type
			Node* max_node=new Node;  //create max node
			max_node->point=0;  //first  point value is 0
			max_node->rebound=0; //first rebound value is 0
			max_node->assist=0; //first assist value is 0
			max_node->name=""; //first name value is empty
			max_node->left=NULL;  //left,right and parent is null
			max_node->right=NULL;
			max_node->parent=NULL;
			binary_find_max(this->root,max_node,type);  //call binary_find_max function parameters are root of three,season,max_node and type
			return max_node;  //return max_node
		};
		void update(Node* node,const string &season,const string &team,const int &rebound,const int &assist,const int &point){ //update function
			node->season=season; //assign new season
			node->team=team; //assign new team
			node->rebound += rebound; //add rebound
			node->assist += assist; //add assist
			node->point += point; //add point
		}
};


void RedBlackTree::again_balance(Node *&root, Node *&nodep) { //again balanca function 
	Node *parent_nodep = NULL; 
	Node *grand_parent = NULL; 

	while ((nodep != root)) { //until node == root
		if (nodep->parent->color != 0) break; //if nodep parent is red break
		if (nodep->color == 1) break; //if node is black break

		parent_nodep = nodep->parent; 
		grand_parent = nodep->parent->parent; 

		if (parent_nodep == grand_parent->right) { //if parent is right child of grand_parent
			Node *uncle_nodep = grand_parent->left; //so uncle is left child

			if ((uncle_nodep != NULL) && (uncle_nodep->color ==0)) { //if uncle is red and uncle is not NULL,just recoloring
				grand_parent->color = 0;  //grand_parent set red
				parent_nodep->color = 1;  //parent is black
				uncle_nodep->color = 1;  //uncle is black
				nodep = grand_parent;  //node assign grand_parent
			} 
			else {  //if uncle is NULL or uncle is black
				if (nodep == parent_nodep->left) {   //check node is left child of parent
					right_rotate(root, parent_nodep);   //first right rotate
					nodep = parent_nodep;  //then node set parent_nodep
					parent_nodep = nodep->parent;  //parent node set  parent of node
				} 
				left_rotate(root, grand_parent); //left rotate  grand_parent
				swap(parent_nodep->color,grand_parent->color);  //swap color grand_parent and parent
				nodep = parent_nodep;  //node set parent
			}
		} 
		else {  //if parent is left child of grand_parent
			Node *uncle_nodep = grand_parent->right;  //so uncle is right child

			if (uncle_nodep != NULL && uncle_nodep->color ==0) { //if uncle is not null and uncle is red,just recoloring
				grand_parent->color = 0; //grandparent is red
				parent_nodep->color = 1;  //parent is black
				uncle_nodep->color = 1;  //uncle is black
				nodep = grand_parent;  //node set grand_parent
			} 

			else {  //if uncle is null or uncle is black
				if (nodep == parent_nodep->right) { //node is right child of parent
					left_rotate(root, parent_nodep);  //left rotate of parent 
					nodep = parent_nodep;  //node set parent 
					parent_nodep = nodep->parent;  //parent node set parent of node
				} 
				right_rotate(root, grand_parent); //right rotate the grand_parent
				swap(parent_nodep->color,grand_parent->color);  //swap color of parent and grand_parent
				nodep = parent_nodep;  //node set parent
			} 
		} 
	} 
	root->color = 1; //finally root is black
} 

// Function to insert a new node with given data 
void RedBlackTree::insert(const string &name,const string &season,const string &team,const int &rebound,const int &assist,const int &point)  {  //insert new node 
	Node *new_node = new Node;  //create new node and assign parameters value to new node
	new_node->color=0;
	new_node->name=name;
	new_node->season=season;
	new_node->team=team;
	new_node->rebound=rebound;
	new_node->assist=assist;
	new_node->point=point;
	new_node->left=NULL;  //left,right childs and parent is null
	new_node->right=NULL;
	new_node->parent=NULL;
	root = bst_insert(root,new_node); //insert new node to tree
	again_balance(root, new_node);  //and balance the tree according to new node
} 



int main(int argc, char **argv) {
	ifstream file;
	file.open(argv[1]);  //open file
	
	if (!file){
		cerr << "File cannot be opened!";  //if file cannot open
		exit(1);
	}
	string line;
	getline(file,line);  //get first line 
//	cout << line << endl; 
	string season; //season
	string name;   //player_name
	string team;   //team
	string line4; //string rebound
	string line5; //string assist
	string line6;  //string point
	int rebound;  //rebound
	int assist; 
	int point;
	string firstSeason;  //firstSeason name
	string currentSeason;  //currentSeason check new season
	string newSeason; //newSeason
	int season_counter=0;  //number of season to control
	int inside_counter=0;  //number of add player in same season to control
	RedBlackTree tree; //create new tree
	
	while(!file.eof()){
		getline(file, season, ','); //season (string)
		getline(file, name, ','); //player name (string)
		getline(file, team, ','); //team (string)
		getline(file, line4, ','); //rebound (string)
		getline(file, line5, ','); //assist (string)
		getline(file, line6, '\n'); //point (string)
		stringstream s1(line4);
		stringstream s2(line5);
		stringstream s3(line6);
		s1 >> rebound;  //convert rebound,assist and point string to int 
		s2 >> assist;
		s3 >> point;
		newSeason=season;  //first newSeason set season
		if (season_counter == 0 && inside_counter == 0)	{  //if we read first player from file 
			firstSeason=season; 
			currentSeason==season;
		}
		else{
			if (currentSeason != newSeason || file.eof()){  //if season is changed, print max point,rebound,assist of seasons 
				cout << "End of the "<< currentSeason << " Season"<< endl;
				Node* max_point=tree.find_max("point");  //call find_max function type is point
				Node* max_assist=tree.find_max("assist"); //call find_max function type is assist
				Node* max_rebound=tree.find_max("rebound"); //call find_max function type is rebound
				cout << "Max Points: " << max_point->point << " - Player Name: "<< max_point->name  << endl;  //print max point and name
				cout << "Max Assists: " << max_assist->assist << " - Player Name: "<< max_assist->name  << endl; //print max assist and name 
				cout << "Max Rebs: " << max_rebound->rebound << " - Player Name: "<< max_rebound->name  << endl; //print max rebound and name
				if (firstSeason == currentSeason) tree.preorder_traversal();  //if season is first we print first season player name using pre-order traversal
				inside_counter=0;  //new inside counter is 0
				season_counter+=1; //season_counter is increment
			}
		}
		Node* is_there=tree.search(name);  //before insert, we search player name 
		if (is_there == NULL){ //if player is not exist 
			tree.insert(name,season,team,rebound,assist,point); //insert to tree
		}
		else{  //if player is exist 
			tree.update(is_there,season,team,rebound,assist,point); //update player
		}
		currentSeason=season;  //end of loop,assign season to currentSeason
		inside_counter += 1; //and incremet inside counter
	}
	file.close();
	return 0; 
} 

