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
    int find_key(int ,char ,int);
    void remove(int ,char ,int);
    void removeFromLeaf(int);
    void removeFromNonLeaf(int,int);
    void fill(int); 
    void merge(int idx); 
    
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
        void remove(int key,char key_c,int type);
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
int BTreeNode::find_key(int key,char key_c,int type) 
{ 
    int index_first=0; 
    if (type == 1) {
    	while (index_first<current && keys[index_first].x < key) ++index_first; 
	}
	else if(type == 2){
		while (index_first<current && keys[index_first].y < key) ++index_first; 
	}
	else if(type == 3){
		while (index_first<current && keys[index_first].z < key_c) ++index_first; 
	}
    
    return index_first; 
} 
void BTreeNode::remove(int key,char key_c,int type) 
{ 
    int index_first = find_key(key,key_c,type);
    if (type == 1){
	    if (index_first < current && keys[index_first].x == key) 
	    { 
	        if (leaf == true) removeFromLeaf(index_first); 
	        else removeFromNonLeaf(index_first,type); 
	    } 
	    else
	    {
	        if (leaf == false){
	        	bool flag = ( (index_first==current)? true : false ); 
	  
		        if (this->child_p[index_first]->current < this->degree) fill(index_first); 
		
		        if (flag == true && index_first > current)  child_p[index_first-1]->remove(key,key_c,type); 
		        else  child_p[index_first]->remove(key,key_c,type);
			} 
			else return;     
	    }
	}
	else if (type == 2 ){
		if (index_first < current && keys[index_first].y == key) 
	    { 
	        if (leaf  == true) removeFromLeaf(index_first); 
	        else  removeFromNonLeaf(index_first,type); 
	    } 
	    else
	    { 
	        if (leaf == false){
	        	bool flag = ( (index_first==current)? true : false ); 
	  
		        if (this->child_p[index_first]->current < this->degree) fill(index_first); 
		
		        if (flag == true && index_first > current)  child_p[index_first-1]->remove(key,key_c,type); 
		        else   child_p[index_first]->remove(key,key_c,type);	
			}
			else return;
	    }
				
	}
	else if (type == 3){
		if (index_first < current && keys[index_first].z == key_c) 
	    { 
	
	        if (leaf == true) removeFromLeaf(index_first); 
	        else removeFromNonLeaf(index_first,type); 
	    } 
	    else
	    { 
	        if (leaf == false) { 
	            bool flag = ( (index_first==current)? true : false ); 
	  
		        if (this->child_p[index_first]->current < this->degree) fill(index_first); 
		
		        if (flag == true && index_first > current)  child_p[index_first-1]->remove(key,key_c,type); 
		        else	child_p[index_first]->remove(key,key_c,type); 
	        } 
			else return;
	    }
	}
    return; 
} 
void BTreeNode::removeFromLeaf (int first_index) 
{ 
    for (int i=first_index+1; i<current; ++i)  keys[i-1] = keys[i]; 
    current--; 
    return; 
} 
  
void BTreeNode::removeFromNonLeaf(int first,int type) 
{ 
  
    Node k = keys[first]; 

    if (child_p[first]->current >= this->degree) 
    { 
    	BTreeNode *temp=child_p[first]; 
    	while (!temp->leaf)  temp = temp->child_p[temp->current]; 
    	Node prev = temp->keys[temp->current-1];
        keys[first] = prev; 
        if (type == 1)  child_p[first]->remove(prev.x,prev.z,1); 
        else if(type == 2)  child_p[first]->remove(prev.y,prev.z,2);
    	else if(type == 3)   child_p[first]->remove(prev.x,prev.z,3);
	} 
 
    else if  (child_p[first+1]->current >= degree) 
    { 
    	BTreeNode *temp = child_p[first+1]; 
    	while (!temp->leaf)  temp = temp->child_p[0]; 
        Node next = temp->keys[0]; 
        keys[first] = next; 
        if (type == 1)	child_p[first+1]->remove(next.x,next.z,1); 
        else if (type == 2)	child_p[first+1]->remove(next.y,next.z,2); 
        else if (type == 3)	child_p[first+1]->remove(next.x,next.z,3); 
    } 
    else
    { 
        merge(first); 
        if (type == 1) child_p[first]->remove(k.x,k.z,1);
        else if (type == 2) child_p[first]->remove(k.y,k.z,2);
        else if (type == 3) child_p[first]->remove(k.x,k.z,3);  
    } 
    return; 
} 

  
void BTreeNode::fill(int first) 
{ 
    if (first!=0 && child_p[first-1]->current>=degree){
    	BTreeNode *child=child_p[first]; 
	    BTreeNode *sibling=child_p[first-1];  
	    for (int i=child->current-1; i>=0; --i) child->keys[i+1] = child->keys[i]; 
	    
	    if (!child->leaf) { 
	        for(int i=child->current; i>=0; --i) child->child_p[i+1] = child->child_p[i]; 
	    } 
	    child->keys[0] = keys[first-1]; 
	  
	    if(!child->leaf) child->child_p[0] = sibling->child_p[sibling->current]; 
	 
	    keys[first-1] = sibling->keys[sibling->current-1]; 
	  
	    child->current += 1; 
	    sibling->current -= 1; 
	} 

    else if (first!=current && child_p[first+1]->current>=degree){
    	
	    BTreeNode *child=child_p[first]; 
	    BTreeNode *sibling=child_p[first+1]; 
	  
	    child->keys[(child->current)] = keys[first]; 
	  
	
	    if (!(child->leaf)) child->child_p[(child->current)+1] = sibling->child_p[0]; 
	  
	    keys[first] = sibling->keys[0]; 
	  
	    for (int i=1; i<sibling->current; ++i) sibling->keys[i-1] = sibling->keys[i]; 
	  
	    if (!sibling->leaf) { 
	        for(int i=1; i<=sibling->current; ++i) sibling->child_p[i-1] = sibling->child_p[i]; 
	    } 
	
	    child->current += 1; 
	    sibling->current -= 1; 
    	
	} 

    else
    { 
        if (first != current) merge(first); 
        else  merge(first-1); 
    } 
    return; 
} 

void BTreeNode::merge(int first) 
{ 
    BTreeNode *child = child_p[first]; 
    BTreeNode *sibling = child_p[first+1]; 
  
    child->keys[degree-1] = keys[first]; 
  
    for (int i=0; i<sibling->current; ++i) child->keys[i+degree] = sibling->keys[i]; 
  
    if (!child->leaf) { 
        for(int i=0; i<=sibling->current; ++i) child->child_p[i+degree] = sibling->child_p[i]; 
    } 

    for (int i=first+1; i<current; ++i) keys[i-1] = keys[i]; 
  

    for (int i=first+2; i<=current; ++i) child_p[i-1] = child_p[i]; 

    child->current += sibling->current+1; 
    current--; 
    delete(sibling); 
    return; 
}
void BTree::remove(int key,char key_c,int type) 
{ 
    if (!root) return;
  
    root->remove(key,key_c,type); 

    if (root->current==0) 
    { 
        BTreeNode *temp = root; 
        if (root->leaf) root = NULL; 
        else root = root->child_p[0]; 
        delete temp; 
    } 
    return; 
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
    char delete_key_c;
    int delete_key;
    if (key=='x'){
    	cin >> delete_key ;
    	t.remove(delete_key,delete_key_c,1);
	}
	else if(key == 'y'){
		cin >> delete_key;
		t.remove(delete_key,delete_key_c,2);
	}
	else if(key == 'z'){
		cin >> delete_key_c;
		t.remove(0,delete_key_c,3);
	}

    t.prefix();

    return 0;
}








