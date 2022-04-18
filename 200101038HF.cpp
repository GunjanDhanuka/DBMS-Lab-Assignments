#include <bits/stdc++.h>
using namespace std;

class Page
{
private:
    int count;
    int remaining;
    Page *prev;
    Page *next;
    map<int, int> data;
    map<int, int> order;

public:
    Page(int capacity)
    {
        count = 0;
        remaining = capacity - 16;
        prev = nullptr;
        next = nullptr;
    }

    bool Insertion(int key, int storage)
    {
        data.insert(make_pair(key, storage));
        order.insert(make_pair(count, key));
        count++;
        remaining -= (storage + 4);
        return true;
    }

    bool Accomodation_Possible(int storage)
    {
        if (remaining >= (storage + 4))
            return true;
        else
            return false;
    }

    int Record_Count()
    {
        return count;
    }

    bool SetPrev(Page *curr)
    {
        prev = curr;
        return true;
    }

    bool SetNext(Page *curr)
    {
        next = curr;
        return true;
    }

    Page *GetPrev()
    {
        return prev;
    }

    Page *GetNext()
    {
        return next;
    }

    int Search(int key)
    {
        for (int i = 0; i < count; i++)
        {
            if (order[i] == key)
                return i;
        }
        return -1;
    }

    bool Status()
    {
        for (int i = 0; i < count; i++)
            cout << i << " : " << order[i] << ", storage size " << data[order[i]] << "\n";
        return true;
    }
};

class Heap_File
{
private:
    Page *start;
    Page *end;
    int page_count;
    int page_size;

public:
    Heap_File(int size)
    {
        start = nullptr;
        end = nullptr;
        page_count = 0;
        page_size = size;
    }

    bool Insertion(int key, int storage)
    {
        if (start == nullptr)
        {
            Page *p = new Page(page_size);
            page_count++;
            p->Insertion(key, storage);
            start = p;
            end = p;
        }
        else
        {
            Page *curr = start;

            while (curr != nullptr)
            {
                if (curr->Accomodation_Possible(storage))
                {
                    curr->Insertion(key, storage);
                    break;
                }
                else
                {
                    curr = curr->GetNext();
                }
            }
            if (curr == nullptr)
            {
                Page *p = new Page(page_size);
                page_count++;
                p->Insertion(key, storage);
                end->SetNext(p);
                p->SetPrev(end);
                end = p;
            }
        }
        return true;
    }

    bool Search(int key)
    {
        Page *curr = start;
        for (int i = 0; i < page_count; i++)
        {
            int slot_id = curr->Search(key);
            if (slot_id != -1)
            {
                cout << i << " " << slot_id << "\n";
                return true;
            }
            curr = curr->GetNext();
        }
        cout << "-1 -1\n";
        return true;
    }

    bool Display()
    {
        cout << page_count << " ";
        Page *curr = start;
        for (int i = 0; i < page_count; i++)
        {
            cout << curr->Record_Count() << " ";
            curr = curr->GetNext();
        }
        cout << "\n";
        return true;
    }

    // For seeing the internal structure
    bool Status()
    {
        Page *curr = start;
        for (int i = 0; i < page_count; i++)
        {
            cout << "Page No.:" << i << "\n";
            cout << "No. of records are:" << curr->Record_Count() << "\n";
            curr->Status();
            curr = curr->GetNext();
        }
        return true;
    }
};

int main()
{
    int storage_size;
    cin >> storage_size;
    Heap_File object(storage_size);
    int operation;
    bool quit = false;
    while (cin >> operation)
    {
        switch (operation)
        {
        case 1:
        {
            int size, pkey;
            ;
            cin >> size >> pkey;
            object.Insertion(pkey, size);
            break;
        }

        case 2:
        {
            object.Display();
            break;
        }

        case 3:
        {
            int key;
            cin >> key;
            object.Search(key);
            break;
        }

        case 4:
        {
            quit = true;
            return 0;
        }
        default:
            cout << "Please enter a valid operation\n";
        }
        if (quit)
            break;
    }
    return 0;
}