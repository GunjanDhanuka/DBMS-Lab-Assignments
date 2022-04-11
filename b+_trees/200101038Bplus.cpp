#include <bits/stdc++.h>
using namespace std;

int MAX_DATA = 4;
int MAX_INDEX = 3;
int d_count = 0;
int i_count = 0;

class Node
{
    bool is_leaf;
    int *key;
    int size;
    Node **ptr;
    friend class BPTree;

public:
    Node(bool);
};

class BPTree
{
    Node *root;
    void insertInternal(int, Node *, Node *);
    Node *findParent(Node *, Node *);
    pair<int, int> countNodes(Node*, pair<int, int>);

public:
    BPTree();
    void search(int);
    void insert(int);
    void display();
    Node *getRoot();
};

Node::Node(bool b)
{
    // create the array of keys and pointers
    // this->is_leaf = is_leaf;
    // if(!is_leaf){
    //     key = new int[MAX_INDEX];
    //     ptr = new Node *[MAX_INDEX + 1];
    // } else {
    //     key = new int[MAX_DATA];
    //     ptr = new Node *[MAX_DATA + 1];
    // }
    int MAX = max(MAX_DATA, MAX_INDEX);
    key = new int[MAX];
    ptr = new Node* [MAX + 1];
}

BPTree::BPTree()
{
    root = NULL;
}

void BPTree::search(int value)
{
    // empty tree
    if (root == NULL)
    {
        cout << "Tree is empty!" << endl;
    }
    else
    {
        Node *n = root;
        while (n->is_leaf == false)
        {
            for (int i = 0; i < n->size; i++)
            {
                if (value < n->key[i])
                {
                    // value is smallest than the smallest element
                    // hence not present in this node but may be in its left child
                    n = n->ptr[i];
                    break;
                }

                if (i == n->size - 1)
                {
                    // reached end of node
                    // look in the rightmost child of this node
                    n = n->ptr[i + 1];
                    break;
                }
            }
        }

        // search through the node to find the value
        for (int i = 0; i < n->size; i++)
        {
            if (n->key[i] == value)
            {
                cout << "Found element " << value << endl;
                return;
            }
        }

        cout << "Not found " << value << endl;
    }
}

// Insert in a B+ tree
void BPTree::insert(int x)
{
    // if root is null, then create root and return it
    if (root == NULL)
    {
        root = new Node(true);
        root->key[0] = x;
        d_count++;
        root->is_leaf = true;
        root->size = 1;
    }
    else
    {
        Node *cursor = root;
        Node *parent;

        // move through the tree until you reach the leaf node
        while (cursor->is_leaf == false)
        {
            parent = cursor;

            for (int i = 0; i < cursor->size; i++)
            {
                // go down the appropriate child of the node
                if (x < cursor->key[i])
                {
                    cursor = cursor->ptr[i];
                    break;
                }

                // if we reach the end of the node
                if (i == cursor->size - 1)
                {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }

        // at this point, cursor is a leaf node where we should place x (if it is not full)
        if (cursor->size < MAX_DATA)
        {
            // Leaf node is NOT full
            int i = 0;
            while (x > cursor->key[i] && i < cursor->size)
            {
                i++;
            }
            for (int j = cursor->size; j > i; j--)
            {
                // shift all elements greater than x to one position to right
                cursor->key[j] = cursor->key[j - 1];
            }

            // insert in the leaf node done
            cursor->key[i] = x;
            cursor->size++;

            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = NULL;
        }
        else
        {
            //cout << "Leaf node was full\n";
            // appropriate leaf node was full
            Node *newLeaf = new Node(true);
            d_count++;

            int tempNode[MAX_DATA + 1];

            // copy contents of cursor into temporary Node
            for (int i = 0; i < MAX_DATA; i++)
            {
                tempNode[i] = cursor->key[i];
            }
            int i = 0, j = 0;

            while (x > tempNode[i] && i < MAX_DATA)
            {
                i++;
            }

            // shift all elements greater than x to one position to right
            // for (j = MAX_DATA + 1; j > i; j--)
            // {
            //     tempNode[j] = tempNode[j - 1];
            // }

            for (j = MAX_DATA+1; j > i; j--)
            {
                tempNode[j] = tempNode[j - 1];
            }

            // inserted the value into the temp node
            tempNode[i] = x;
            newLeaf->is_leaf = true;

            // splitting of the temp node
            cursor->size = (MAX_DATA + 1) / 2;
            newLeaf->size = MAX_DATA + 1 - (MAX_DATA + 1) / 2;

            // connect new leaf to the right of cursor
            cursor->ptr[cursor->size] = newLeaf;

            // copy the rightmost ptr from cursor to newLeaf
            newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX_DATA];

            cursor->ptr[MAX_DATA] = NULL;

            for (i = 0; i < cursor->size; i++)
            {
                cursor->key[i] = tempNode[i];
            }

            for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
            {
                newLeaf->key[i] = tempNode[j];
            }

            if (cursor == root)
            {
                // create a new root
                Node *newRoot = new Node(false);
                i_count++;

                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->is_leaf = false;
                newRoot->size = 1;

                root = newRoot;
            }
            else
            {
                // recursive call for insert in internal (separator in index node)
                insertInternal(newLeaf->key[0], parent, newLeaf);
            }
        }
    }
}

void BPTree::insertInternal(int x, Node *cursor, Node *child)
{
    // if there is free space in the node
    if (cursor->size < MAX_INDEX)
    {
        int i = 0;
        while (x > cursor->key[i] && i < cursor->size)
        {
            i++;
        }

        for (int j = cursor->size; j > i; j--)
        {
            cursor->key[j] = cursor->key[j - 1];
        }

        for (int j = cursor->size + 1; j > i + 1; j--)
        {
            cursor->ptr[j] = cursor->ptr[j - 1];
        }

        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child;
    }
    else
    {
        // if the node is full
        Node *newInternal = new Node(false);
        i_count++;
        int virtualKey[MAX_INDEX + 1];
        Node *virtualPtr[MAX_INDEX + 2];

        for (int i = 0; i < MAX_INDEX; i++)
        {
            virtualKey[i] = cursor->key[i];
        }

        for (int i = 0; i < MAX_INDEX + 1; i++)
        {
            virtualPtr[i] = cursor->ptr[i];
        }

        int i = 0, j = 0;

        while (x > virtualKey[i] && i < MAX_INDEX)
        {
            i++;
        }

        for (int j = MAX_INDEX + 1; j > i; j--)
        {
            virtualKey[j] = virtualKey[j - 1];
        }

        virtualKey[i] = x;

        for (int j = MAX_INDEX + 2; j > i + 1; j--)
        {
            virtualPtr[j] = virtualPtr[j - 1];
        }

        virtualPtr[i + 1] = child;
        newInternal->is_leaf = false;

        cursor->size = (MAX_INDEX) / 2;
        newInternal->size = MAX_INDEX - (MAX_INDEX / 2);

        // copy over the keys and pointers from virtual node to new node
        for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
        {
            newInternal->key[i] = virtualKey[j];
        }

        for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
        {
            newInternal->ptr[i] = virtualPtr[j];
        }

        if (cursor == root)
        {
            Node *newRoot = new Node(false);
            i_count++;
            newRoot->key[0] = virtualKey[cursor->size];
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->is_leaf = false;
            newRoot->size = 1;
            root = newRoot;
        }
        else
        {
            // push one value to the parent for insertion
            // recursive call
            insertInternal(cursor->key[cursor->size + 1], findParent(root, cursor), newInternal);
        }
    }
}

Node *BPTree::findParent(Node *cursor, Node *child)
{
    Node *parent;

    if (cursor->is_leaf || cursor->ptr[0]->is_leaf)
    {
        //if we reach end of tree, return Null
        return NULL;
    }

    for (int i = 0; i < cursor->size + 1; i++)
    {
        if (cursor->ptr[i] == child)
        {
            parent = cursor;
            return parent;
        }
        else
        {
            //recursively find parent in each child of the cursor
            parent = findParent(cursor->ptr[i], child);

            //if parent found, return it
            if (parent != NULL)
            {
                return parent;
            }
        }
    }

    return parent;
}

Node* BPTree::getRoot(){
    return root;
}

void BPTree::display(){
    //cout << "here\n";
    Node* x = root;
    //int index_nodes = 0, data_nodes = 0;

    // pair<int, int> counts = {0,0};
    // counts = countNodes(x, counts);

    // cout << counts.first << " " << counts.second << " ";
    cout << i_count << " " << d_count << " ";
    for(int i = 0; i < x->size; i++){
        cout << x->key[i] << " ";
    }
    cout << endl;
}

pair<int, int> BPTree::countNodes(Node* cursor, pair<int, int> p){
    if(cursor->is_leaf){
        p.second++;
        return p;
    }
    p.first++;
    for(int i = 0; i < cursor->size + 1; i++){
        Node* tmp = cursor->ptr[i];
        if(tmp != NULL) p = countNodes(tmp, p);
    }
    return p;
}

int main(){
    int d,t;
    cin >> d >> t;
    MAX_INDEX = 2*t + 1;
    MAX_DATA = 2*d;
    
    BPTree bpt;
    while(true){
        int choice;
        cin >> choice;

        switch(choice){
            case 1: int x; cin >> x; bpt.insert(x); break;
            case 2: bpt.display(); break;
            case 3: exit(0);
        }
    }
}