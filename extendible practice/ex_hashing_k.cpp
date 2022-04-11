#include<bits/stdc++.h>
using namespace std;

class bucket
{
    public:
        bucket(int,int);
        bool bucket_insert(int);
        int get_no_keys();
        int get_capacity();
        int get_local_depth();
        int* empty_bucket();
        void increase_local_depth();
        int get_key(int);
        virtual ~bucket();
    private:
        int capacity;
        int no_keys;
        int local_depth;
        int* elements;
};

bucket::bucket(int bucket_size,int depth){
    capacity = bucket_size;
    no_keys = 0;
    local_depth = depth;
    elements = new int[bucket_size];
}
int* bucket::empty_bucket(){
    int* tmp = new int[capacity];
    for(int i=0;i<no_keys;i++){
        tmp[i] = elements[i];
    } 
    // delete[] elements;
    no_keys = 0;
    return tmp;
}

int bucket::get_capacity(){
    return capacity;
}
int bucket::get_local_depth(){
    return local_depth;
}
void bucket::increase_local_depth(){
    local_depth++;
}
int bucket::get_no_keys(){
    return no_keys;
}

int bucket::get_key(int index){
    if(index >= no_keys) return -1;
    return elements[index];
}

bool bucket::bucket_insert(int value){
    if(no_keys == capacity) return false;
    elements[no_keys] = value;
    no_keys++;
    return true;
}

bucket::~bucket(){
    delete[] elements;
}

class directory
{
    public:
        directory(int,int);
        void assign_bucket(int,bucket*);
        bucket* getbucket(int);
        void print();
        void print_directory();
        void insert(int);
        void dir_double();
        bool search(int);
        void delete_value(int);
        virtual ~directory();
    private:
        int global_depth;
        bucket** pointers;
        vector<int> creation;
};

directory::directory(int depth,int bucket_size){
    global_depth = depth;
    pointers = new bucket*[(1 << global_depth)];

    bucket * b;
    for(int i=0;i<(1 << global_depth);i++){
        b = new bucket(bucket_size,depth);
        assign_bucket(i,b);
        creation.push_back(i);
    }
}

void directory::dir_double(){
    global_depth++;
    bucket** tmp = new bucket*[(1 << global_depth)];
    for(int i=0;i<(1 << (global_depth-1));i++){
        tmp[i] = pointers[i];
        tmp[(1 << (global_depth-1))+i] = pointers[i];
    }
    delete[] pointers;
    pointers = tmp;
}

bool directory::search(int value){
    bucket * buck = pointers[value%(1 << global_depth)];
    for(int i=0;i<buck->get_no_keys();i++){
        if(buck->get_key(i) == value) return true;
    }
    return false;
}

void directory::delete_value(int value){
    if(search(value) == 1){
        bucket * buck = pointers[value%(1 << global_depth)];
        int* tmp = buck->empty_bucket();
        for(int i=0;i<buck->get_no_keys();i++){
            if(tmp[i] != value) buck->bucket_insert(tmp[i]);
        }        
        delete[] tmp;                   
    }
}

void directory::insert(int value){
    bucket * buck = pointers[value%(1 << global_depth)];
    if(buck->get_capacity() != buck->get_no_keys()){
        buck->bucket_insert(value);
    }else{
        bucket * b = new bucket(buck->get_capacity(),buck->get_local_depth());
        int earlier = value%(1<<buck->get_local_depth());
        int newer = (1<<buck->get_local_depth())|earlier;

        if(buck->get_local_depth() < global_depth){
            assign_bucket(newer,b);
            for(int i = 1;((i<<(buck->get_local_depth()+1))|newer)<(1<<global_depth);i++){
                assign_bucket((i<<(buck->get_local_depth()+1))|newer,b);
            }
        }else{
            if(global_depth == 20){
                cout<<"error"<<endl;
                return;
            }
            dir_double();
            assign_bucket(newer,b);    
        }
        creation.push_back(newer);
        pointers[earlier]->increase_local_depth();
        pointers[newer]->increase_local_depth();  
        int* tmp = pointers[earlier]->empty_bucket();
        for(int i=0;i<pointers[earlier]->get_capacity();i++){
            pointers[tmp[i]%(1 << global_depth)]->bucket_insert(tmp[i]);
        }
        delete[] tmp;

        insert(value); 
    }
}

void directory::assign_bucket(int index,bucket* p){
    pointers[index] = p;
}

bucket* directory::getbucket(int index){
    return pointers[index];
}

void directory::print(){
    for(int i=0;i<(1<<global_depth);i++){
        cout<<pointers[i]->get_no_keys()<<" "<<pointers[i]->get_local_depth()<<" ";
        for(int j=0;j<pointers[i]->get_no_keys();j++){
            cout<<pointers[i]->get_key(j)<<" ";
        }
        cout<<endl;
    }
}

void directory::print_directory(){
    cout<<global_depth<<endl;
    cout<<creation.size()<<endl;
    for(auto u : creation){
        cout<<pointers[u]->get_no_keys()<<" "<<pointers[u]->get_local_depth()<<endl;
    }
}

directory::~directory(){
    delete[] pointers;
}

int main(){
    int g_depth,b_capacity;
    cin>>g_depth>>b_capacity;
    directory d(g_depth,b_capacity);
    int x,y;
    cin>>x;
    while(x != 6){
        if(x == 2){
            cin>>y;
            d.insert(y);
        }else if(x == 3){
            cin>>y;
            //cout<<d.search(y)<<endl;
        }else if(x == 4){
            cin>>y;
            d.delete_value(y);
        }else if(x == 5){
            d.print_directory();
        }
        cin>>x;
    }
    // directory d(1,2);
    // d.insert(16);
    // d.insert(32);
    // //d.insert(48);
    // // d.insert(16);
    // // d.insert(16);
    // d.print_directory();

}