#include <bits/stdc++.h>
using namespace std;

#define dbg(x) cout << "x" << endl

class Bucket
{
private:
    int local_depth;
    int bucket_capacity;
    vector<int> data;

public:
    Bucket(int bucket_cap)
    {
        local_depth = 1;
        bucket_capacity = bucket_cap;
        //data.clear();
        data.push_back(5);
        //data.pop_back();
        cout << data.size() << endl;
    }
    ~Bucket()
    {
        // dtor
        //data.clear();
    }

    int get_local_depth()
    {
        return local_depth;
    }

    void increase_local_depth()
    {
        local_depth++;
    }

    int get_size_of_bucket()
    {
        return data.size();
    }

    vector<int> get_bucket_elements(){
        return data;
    }

    void clear_bucket(){
        data.clear();
        return;
    }

    bool is_bucket_full()
    {
        return data.size() == bucket_capacity;
    }

    int find_value_in_bucket(int value)
    {
        //dbg(data.size());
        for (int i = 0; i < data.size(); i++)
        {
            //dbg(i);
            if (data[i] == value)
            {
                
                return i;
            }
        }
        // not found
        return -1;
    }

    void insert_into_bucket(int value)
    {
        if (!is_bucket_full())
        {
            //cout << "here\n";
            data.push_back(value);
            //cout << data.size() << endl;
        }
        else
        {
            cout << "Bucket full" << endl;
        }
    }

    void delete_value_from_bucket(int value){
        data.erase(find(data.begin(), data.end(), value));
    }

    void print_bucket()
    {
        cout << "Local depth of bucket = " << local_depth << endl
             << "Contents: ";
        for (int i = 0; i < data.size(); i++)
        {
            cout << data[i] << " ";
        }
        cout << endl
             << endl;
    }
};


class ExtendibleHashing
{
private:
    int global_depth;
    int bucket_cap;
    int max_global_depth;
    vector<Bucket *> directory;

public:
    // constructor
    ExtendibleHashing(int global_depth_in, int bucket_capacity)
    {
        global_depth = global_depth_in;
        max_global_depth = 20;
        bucket_cap = bucket_capacity;
        //directory.clear();

        // inserting two buckets into the directory
        for(int i = 0; i < (1<<global_depth_in); i++){
            add_new_bucket();
            //cout << directory[i] << endl;
        }
    }

    ~ExtendibleHashing()
    {
        // dtor
        //directory.clear();
    }

    int get_global_depth()
    {
        return global_depth;
    }

    int get_bucket_cap()
    {
        return bucket_cap;
    }

    int hash_fn(int value)
    {
        return value & ((1 << global_depth) - 1);
    }

    void set_global_depth(int value)
    {
        global_depth = value;
    }

    void set_bucket_cap(int value)
    {
        bucket_cap = value;
    }

    void add_new_bucket(){
        directory.push_back(new Bucket(bucket_cap));
    }

    void insert_value(int value)
    {
        int dir_id = hash_fn(value);
        if (directory[dir_id]->is_bucket_full())
        {
            // bucket full

            //local depth = global depth => directory doubling and bucket rearrangement
            if(directory[dir_id]->get_local_depth() == get_global_depth()){
                int old_gd = get_global_depth();

                if(old_gd == max_global_depth){
                    //Maximum limit reached
                    cout << "Maximum global depth reached. Cannot expand directory further." << endl;
                }

                //doubling the directory with two pointers pointing to a single bucket
                for(int di = 0; di < (1<<old_gd); di++){
                   directory.push_back(directory[di]);
                }

                //update global depth
                set_global_depth(old_gd+1);

                //create copy of prev elements of overflowing bucket
                vector<int> elements = directory[dir_id]->get_bucket_elements();

                
                int new_dir_id = (1<<get_global_depth())&dir_id;

                directory[new_dir_id] = new Bucket(bucket_cap);

                directory[dir_id]->clear_bucket();
                directory[new_dir_id]->clear_bucket();

                directory[dir_id]->increase_local_depth();

                directory[new_dir_id]->increase_local_depth();

                for(int x : elements){
                    int id = hash_fn(x);
                    insert_value(x);
                }

            } else {
                // local depth < global depth, then bucket splitting
                vector<int> elements = directory[dir_id]->get_bucket_elements();

                int old_hf = hash_fn(value);
                int new_dir_id = (1<<get_global_depth())&dir_id;

                directory[new_dir_id] = new Bucket(bucket_cap);

                directory[dir_id]->clear_bucket();
                directory[new_dir_id]->clear_bucket();

                directory[dir_id]->increase_local_depth();

                directory[new_dir_id]->increase_local_depth();

                for(int x : elements){
                    int id = hash_fn(x);
                    directory[id]->insert_into_bucket(x);
                }
            }
        }
        else
        {
            // bucket not full
            directory[dir_id]->insert_into_bucket(value);
            return;
        }
    }
    bool search_value(int value, bool print = true)
    {
        int dir_id = hash_fn(value);
        int index = directory[dir_id]->find_value_in_bucket(value);

        if (index != -1)
        {
            if (print)
                cout << "Found " << value << " in DirectoryID: " << dir_id << endl;
            return true;
        }
        else
        {
            if (print)
                cout << "Could not find " << value << " in any bucket!\n";
            return false;
        }
    }
    void delete_value(int value)
    {
        //Lazy delete
        int found = search_value(value, false);
        if(!found){
            cout << "Could not find " << value << " in any bucket!\n";
            //continue;
        } else {
            int dir_id = hash_fn(value);
            directory[dir_id]->delete_value_from_bucket(value);
        }
    }
    void display_status()
    {
        for (int di = 0; di < directory.size(); di++)
        {
            cout << "Directory ID: " << di << endl;
            directory[di]->print_bucket();
        }
    }
    void display_status2()
    {
        cout << get_global_depth() << endl;
        cout << directory.size() << endl;
        for (int di = 0; di < directory.size(); di++)
        {
            cout << "Bucket ID: " << di << endl;
            // directory[di]->print_bucket();
            cout << directory[di]->get_size_of_bucket() << " " << directory[di]->get_local_depth() << endl;
        }
    }

};



int main(){
    int gd, bc;
    cin >> gd >> bc;

    ExtendibleHashing hash(gd, bc);
    int input;
    while(cin >> input){
        if(input == 6){
            break;
        }
        if(input == 2){
            int val;
            cin >> val;
            hash.insert_value(val);
            continue;
        }
        if(input==3){
            int val;
            cin >> val;
            hash.search_value(val);
            continue;
        }
        if(input == 4){
            int val;
            cin >> val;
            hash.delete_value(val);
            continue;
        }
        if(input == 5){
            hash.display_status2();
            continue;
        }
        cout << "Wrong input. Please select a value from 1 to 6" << endl;

    }
}
