#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
struct Node{
    int x;
    int y;
    char z;
};
struct BTreeNode{
    Node *keys;   //array of keys
    int degree;  // tree degree
    BTreeNode **child_p;  // array of child_pointers
    int current;  //current number 
    bool leaf; // leaf is exists
    
    BTreeNode (int degree,bool leaf);
    void insert_non_full(int x1,int y1,char z1,int key_type);
    void split_child(int i,BTreeNode *y);
    void prefix();
    
};
void BTreeNode::prefix(){
    
    for (int i=0;i<current;i++){
        cout << "(" << keys[i].x << "," << keys[i].y << "," << keys[i].z << ")";
    }
    cout << endl;
    int i;
    for (i=0;i<current;i++){
    	if(leaf==false) child_p[i]->prefix();
	}
	if (leaf == false) child_p[i]->prefix();
}

class BTree{
        BTreeNode *root;
        int degree;
    public:
        BTree(int degree){
            this->root=NULL;
            this->degree=degree;
        }
        void insert(int,int,char,int );
        void prefix(){
            if (root != NULL) root->prefix();
        }
};
BTreeNode::BTreeNode(int t1,bool leaf1){
    this->degree=t1;
    this->leaf=leaf1;
    keys=new Node[2*degree-1];
    child_p=new BTreeNode *[2*degree];
    this->current=0;
}
void BTree::insert(int x1,int y1,char z1,int key_type){
    
    if (root == NULL){
        root=new BTreeNode(degree,true);
        root->keys[0].x=x1;
        root->keys[0].y=y1;
        root->keys[0].z=z1;
        root->current=1;
    }
    else {
        if (root->current == 2*degree-1){
            BTreeNode *s=new BTreeNode(degree,false);
            
            s->child_p[0] = root;
            s->split_child(0,root);
            int i=0;
            if (key_type==1){
                if (s->keys[0].x < x1) i++;
            }
            else if(key_type==2){
                if (s->keys[0].y < y1) i++;
            }
            else{
                if (s->keys[0].z < z1) i++;
            }
            s->child_p[i]->insert_non_full(x1,y1,z1,key_type);
            
            root=s;
        }
        else root->insert_non_full(x1,y1,z1,key_type);
    }
    
}
void BTreeNode::insert_non_full(int x1,int y1,char z1,int key_type){
    int i=current-1;
    if (leaf==true){
        if (key_type == 1){
            while (i>=0 && keys[i].x > x1){
                keys[i+1]=keys[i];
                i--;
            }
        }
        else if(key_type == 2){
            while (i>=0 && keys[i].y > y1){
                keys[i+1]=keys[i];
                i--;
            }
        }
        else{
            while (i>=0 && keys[i].z > z1){
                keys[i+1]=keys[i];
                i--;
            }
        }
        
        keys[i+1].x=x1;
        keys[i+1].y=y1;
        keys[i+1].z=z1;
        current=current+1;
    }
    else{
        if (key_type == 1){
            while(i >= 0 && keys[i].x > x1) i--;
            if (child_p[i+1]->current == 2*degree-1){
                this->split_child(i+1,child_p[i+1]);
                if (keys[i+1].x < x1) i++;
            }
        }
        else if(key_type == 2){
            while(i >= 0 && keys[i].y > y1) i--;
            if (child_p[i+1]->current == 2*degree-1){
                this->split_child(i+1,child_p[i+1]);
                if (keys[i+1].y < y1) i++;
            }
            
        }
        else{
            while(i >= 0 && keys[i].z > z1) i--;
            if (child_p[i+1]->current == 2*degree-1){
                this->split_child(i+1,child_p[i+1]);
                if (keys[i+1].z < z1) i++;
            }
        }
        
        child_p[i+1]->insert_non_full(x1,y1,z1,key_type);
    }
}
void BTreeNode::split_child(int i,BTreeNode *y){
	
    BTreeNode *z=new BTreeNode(y->degree,y->leaf);
    z->current = degree-1;
    for(int j=0; j< degree-1;j++) z->keys[j]=y->keys[j+degree];
    
    if (y->leaf == false){
        for (int j = 0; j < degree; j++) z->child_p[j] = y->child_p[j+degree];
    }
    y->current = degree-1;
    
    for (int j = current; j >= i+1; j--)  child_p[j+1] = child_p[j]; 
  
    child_p[i+1] = z; 

    for (int j = current-1; j >= i; j--) keys[j+1] = keys[j]; 
  
    keys[i] = y->keys[degree-1]; 
    current = current + 1; 
}


int main() {
    int node_count,degree;
    char key;
    cin >> node_count;
    cin >> degree;
    cin >> key;
    BTree t(degree);
    int x,y;
    char z;
    for (int i=0;i<node_count;i++){
        cin >> x >> y >> z;
        if (key=='x') t.insert(x,y,z,1);
        else if(key == 'y') t.insert(x,y,z,2);
        else if (key == 'z') t.insert(x,y,z,3);
    }
    t.prefix();

    return 0;
}








