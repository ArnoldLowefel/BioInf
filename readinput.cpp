#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int score(char a, char b){
    if(a == b)return 0;
    if(((a == 'A' || a == 'G') && (b == 'C' || b == 'T')))return 4;
    if(((a == 'C' || a == 'T') && (b == 'A' || b == 'G')))return 4;
    return 2;
}
int calculate_overlap(string a, string b){
    cout << a << " " << b << "\n";
    int MIN_OVERLAP = 5;
    int x = a.length() + 1;
    int y = b.length() + 1;
    int alignment[x][y];
    for(int i = 0; i < y; i++){
        alignment[0][i] = INT_MAX / 2;
    }
    for(int i = 0; i < x; i++){
        alignment[i][0] = 0;
    }
    for(int i = 1; i < x; i++){
        for(int j = 1; j < y; j++){
            alignment[i][j] = min({alignment[i-1][j]+8, alignment[i][j-1]+8, alignment[i-1][j-1] + score(a[i-1], b[j-1])});
        }
    }
    /*
    for(int i = 0; i < MIN_OVERLAP; i++){
        alignment[x][i] = INT_MAX / 2;
    }
    */
    cout << "=====result alignment ====\n\n";
    cout << "        ";
    for(auto x: b){
        cout << x << "    ";
    }
    cout << "\n";
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if(1073741823 == alignment[i][j]){
                cout << "+inf ";
            }else{
                cout << alignment[i][j];
                if(alignment[i][j] < 10){
                    cout << "    ";
                }else{
                    cout << "   ";
                }
            }
        }
        cout << "\n";
        cout << a[i] << "  ";
    }
    return 0;
}

int main(){
    vector<string> input;
    string line;
    int size = 0;
    while(getline(cin, line)) {
        input.push_back(line);
        size++;
    }
    cout << size << "\n";
    for(auto x: input){
        cout << x << "\n";
    }

    int graph[size][size];

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i != j){
                graph[i][j] = calculate_overlap(input[i], input[j]);
            }else{
                graph[i][j] = 0;
            }
        }
    } 
}