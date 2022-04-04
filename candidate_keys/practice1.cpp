#include <bits/stdc++.h>

using namespace std;


vector<int> create_vector(string s){
    vector<int> v;
    istringstream is(s);
    int num;
    while(is >> num){
        v.push_back(num);

    }

    return v;
}

int int_from_line(string s){
    istringstream is(s);
    int num;
    is >> num;
    return num;
}

int num_bits(int n){
    return __builtin_popcount(n);
}

bool cand_compare(int a, int b){
    if(num_bits(a) != num_bits(b)){
        return num_bits(a) < num_bits(b);
    }
    return a < b;
}

void print_as_vector(int n, int num_att){
    for(int i = 0; i < num_att; i++){
        if((n) & (1<<i)){
            cout << (i+1) << " ";
        }
    }
}

int main(){
    int num_att, num_fd;
    cin >> num_att >> num_fd;
    string lhs, rhs;

    vector<pair<vector<int>, vector<int>>> fd;
    for(int i = 0; i < num_fd; i++){
        getline(lhs, cin);
        getline(rhs, cin);

        fd.push_back({create_vector(lhs), create_vector(rhs)});
    }

    vector<int> ck;

    int num_sets = 1<<num_att;
    for(int s = 0; s < num_sets; s++){
        bool superkey = false;

        for(int i = 0; i < ck.size(); i++){
            if(s & ck[i] == ck[i]){
                superkey = true;
                break;
            }
        }
        if(superkey) continue;

        int prev = 0;
        int next = s;

        while(prev!=next){
            prev = next;

            for(int i = 0; i < num_fd; i++){
                bool satisfy_fd = true;
                for(int j = 0; j < fd[i].first.size(); j++){
                    if(!((next) & (1 << fd[i].first[j]-1))){
                        satisfy_fd = false;
                        break;
                    }
                }

                if(satisfy_fd){
                    for(int j = 0; j < fd[i].second.size(); j++){
                        next |= (1<<(fd[i].second[j]-1));
                    }
                }
            }

            if(next == num_sets - 1){
                ck.push_back(s);
            }
        
        }
    }

    sort(ck.begin(), ck.end(), cand_compare);
    cout << ck.size() << endl;
    for(int i = 0; i < ck.size(); i++){
        print_as_vector(ck[i], num_att);
        cout << endl;
    }

}