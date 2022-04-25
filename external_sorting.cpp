#include <bits/stdc++.h>
#define max_int 2147483647
using namespace std;

// File contains duplicates
// To remove duplicates just make the following changes
// 1. multiset as set in line 140
// 2. uncomment lines 231,232

class Page
{
private:
    int capacity; // total records in a page
    int size;     // records currently present in a page
    int *record;  // storing records

public:
    Page() {}

    Page(int rec_count)
    {
        capacity = rec_count;
        size = 0;
        record = (int *)calloc(rec_count, sizeof(int));
        memset(record, -1, capacity);
    }

    bool Insertion(int key)
    {
        if (size < capacity)
        {
            record[size] = key;
            size++;
            // cout << "Inserting " << key << " in page\n";
        }
        else
            cout << "Cannot insert, page full\n";
        return true;
    }

    bool Display()
    {
        if (size != 0)
        {
            for (int i = 0; i < size; i++)
                cout << record[i] << " ";
            cout << "\n";
        }
        else
            cout << "Page Empty\n";
        return true;
    }

    bool Clear_Page()
    {
        memset(record, -1, size);
        size = 0;
        return true;
    }

    bool Is_Full()
    {
        if (size == capacity)
            return true;
        else
            return false;
    }

    bool Is_Empty()
    {
        if (size == 0)
            return true;
        else
            return false;
    }

    int GetElement(int index)
    {
        return record[index];
    }

    int Page_Size()
    {
        return size;
    }
};

class File
{
private:
    int buf_pages;
    int total_recs;
    int page_cap;
    int total_pages;
    Page *file;

public:
    File(int buffer_pages, int total_records, int page_capacity)
    {
        buf_pages = buffer_pages;
        total_recs = total_records;
        page_cap = page_capacity;
        total_pages = ceil((double)total_records / page_capacity);
        Page *b = new Page[total_pages];
        for (int i = 0; i < total_pages; i++)
            b[i] = Page(page_capacity);
        file = b;
    }

    bool Insertion(int *input)
    {
        int rec_no = 0;
        for (int i = 0; i < total_pages; i++)
        {
            for (int j = 0; j < page_cap; j++)
            {
                file[i].Insertion(input[rec_no]);
                // cout << "Inserting " << input[rec_no] << "\n";
                rec_no++;
                if (rec_no == total_recs)
                    break;
            }
        }
        return true;
    }

    bool External_Sorting()
    {
        First_Iteration();
        return true;
    }

    bool First_Iteration()
    {
        int runs = ceil((double)total_pages / buf_pages);
        int curr_page = 0;
        int insert_page;
        for (int i = 0; i < runs; i++)
        {
            multiset<int> s;
            for (int j = 0; j < buf_pages; j++)
            {
                if (curr_page < total_pages)
                {
                    for (int k = 0; k < file[curr_page].Page_Size(); k++)
                    {
                        s.insert(file[curr_page].GetElement(k));
                    }
                    file[curr_page].Clear_Page();
                    curr_page++;
                }
                else
                    break;
            }
            insert_page = buf_pages * i;
            for (auto x : s)
            {
                if (file[insert_page].Is_Full())
                    insert_page++;
                file[insert_page].Insertion(x);
            }
        }

        // cout << "\n\n";
        // Display();
        Recursive_Sort(buf_pages - 1, runs, buf_pages);
        // cout << "\n\n";
        // Display();

        return true;
    }

    bool Recursive_Sort(int input_buf, int prev_runs, int prev_run_size) // prev_run_size denotes number of pages sorted in previous run
    {
        if (prev_run_size < total_pages)
        {
            int runs = ceil((double)prev_runs / input_buf);
            Page *output = new Page[total_pages];
            for (int i = 0; i < total_pages; i++)
                output[i] = Page(page_cap);
            for (int i = 0; i < runs; i++)
            {
                int curr_page_index[input_buf]; // Mainting input page index of each previous run
                int curr_rec_index[input_buf];  // Mainting record index of each page
                memset(curr_page_index, -1, sizeof(curr_page_index));
                memset(curr_rec_index, -1, sizeof(curr_rec_index));
                int final_page_index[input_buf];
                int final_rec_index[input_buf];
                memset(final_page_index, -1, sizeof(final_page_index));
                memset(final_rec_index, -1, sizeof(final_rec_index));
                int pointed_value[input_buf];
                memset(pointed_value, -1, sizeof(pointed_value));
                int input_pages = 0;
                for (int j = 0; j < input_buf; j++)
                {
                    int curr_page = (i * input_buf + j) * prev_run_size;
                    int end_page;
                    if ((curr_page + prev_run_size - 1) < total_pages)
                        end_page = curr_page + prev_run_size - 1;
                    else
                        end_page = total_pages - 1;
                    while (file[end_page].Is_Empty())
                        end_page--;

                    if (curr_page < total_pages)
                    {
                        curr_page_index[input_pages] = curr_page;
                        curr_rec_index[input_pages] = 0;
                        pointed_value[input_pages] = file[curr_page].GetElement(curr_rec_index[input_pages]);
                        final_page_index[input_pages] = end_page;
                        final_rec_index[input_pages] = file[end_page].Page_Size();
                        input_pages++;
                    }
                }
                int all_done = 0;
                int curr_output = i * prev_run_size * input_buf;
                while (all_done != input_pages)
                {
                    int smallest = max_int;
                    set<int> indices;
                    for (int i = 0; i < input_pages; i++)
                    {
                        if (pointed_value[i] == -1)
                            continue;
                        else if (pointed_value[i] < smallest)
                        {
                            indices.clear();
                            indices.insert(i);
                            smallest = pointed_value[i];
                        }
                        // else if (pointed_value[i] == smallest)
                        //     indices.insert(i);
                    }
                    if (output[curr_output].Is_Full())
                        curr_output++;
                    output[curr_output].Insertion(smallest);
                    for (auto x : indices)
                    {
                        if (curr_rec_index[x] == (file[curr_page_index[x]].Page_Size() - 1)) // go to next page
                        {

                            if (curr_page_index[x] == final_page_index[x])
                            {
                                curr_page_index[x] = -1;
                                curr_rec_index[x] = -1;
                                pointed_value[x] = -1;
                                all_done++;
                            }
                            else
                            {
                                curr_page_index[x]++;
                                curr_rec_index[x] = 0;
                                pointed_value[x] = file[curr_page_index[x]].GetElement(curr_rec_index[x]);
                            }
                        }
                        else
                        {
                            curr_rec_index[x]++;
                            pointed_value[x] = file[curr_page_index[x]].GetElement(curr_rec_index[x]);
                        }
                    }
                }
            }
            file = output;
            // Display();
            Recursive_Sort(input_buf, runs, input_buf * prev_run_size);
        }
        return true;
    }

    void Display()
    {
        cout << "Total pages: " << total_pages << "\n";
        for (int i = 0; i < total_pages; i++)
            file[i].Display();
    }

    void Sorted_Order()
    {
        for (int i = 0; i < total_pages; i++)
        {
            if (!file[i].Is_Empty())
            {
                for (int j = 0; j < file[i].Page_Size(); j++)
                    cout << file[i].GetElement(j) << "\n";
            }
        }
    }
};

int main()
{
    int buf_pages, total_recs, page_cap;
    cin >> buf_pages >> page_cap >> total_recs;
    File object(buf_pages, total_recs, page_cap);
    int input[total_recs];
    for (int i = 0; i < total_recs; i++)
        cin >> input[i];
    object.Insertion(input);
    // object.Display();
    object.External_Sorting();
    object.Sorted_Order();
    return 0;
}