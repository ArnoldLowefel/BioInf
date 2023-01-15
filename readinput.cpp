#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

#define MIN_OVERLAP 20
#define SMALL_MISMATCH_PENALTY 1
#define BIG_MISMATCH_PENALTY 1
#define MAX_AMOUNT_OF_MISMATCHES 0.2


int score(char a, char b){
    if(a == b)return 0;
    if(((a == 'A' || a == 'G') && (b == 'C' || b == 'T')))return BIG_MISMATCH_PENALTY;
    if(((a == 'C' || a == 'T') && (b == 'A' || b == 'G')))return BIG_MISMATCH_PENALTY;
    return SMALL_MISMATCH_PENALTY;
}

int find_index_of_min(int array[], int size){
    int pos = 0;
    for(int i = 1; i <= size; i++){
        if((float) array[i] / i <= MAX_AMOUNT_OF_MISMATCHES){
            pos = i;
        }
    }
    return pos;
}

int calculate_overlap(string a, string b){
    int x = a.length() + 1;
    int y = b.length() + 1;
    int alignment[x][y];
    for(int i = 0; i < y; i++){
        alignment[0][i] = INT_MAX / 2;
    }
    for(int i = 0; i < x; i++){
        alignment[i][0] = 0;
    }
    for(int i = 0; i < MIN_OVERLAP; i++){
        alignment[x-i-1][0] = INT_MAX / 2;
    }
    for(int i = 1; i < x; i++){
        for(int j = 1; j < y; j++){
            alignment[i][j] = alignment[i-1][j-1] + score(a[i-1], b[j-1]);
        }
    }
    return find_index_of_min(alignment[x-1], y-1);
}

class Graph
{
public:
        vector<set<pair<int, int>>> adjList;
        vector<int> number_of_neighbours;
        vector<int> visited;
        int size;
        int number_of_visited = 0;

        Graph(int n)
        {
            adjList.resize(n);
            for(int i=0; i < n;i++){
                visited.push_back(0);
                number_of_neighbours.push_back(0);
            }
            size = n;
        }

        void addEdge(int src, int dest, int weight){
            adjList[src].insert(make_pair(weight, dest));
            number_of_neighbours[dest]++;
        }

        int findNumberOfStarts(){
            int r = 0;
            for(int i = 0; i < size; i++){
                if(number_of_neighbours[i] <= 1){
                    r++;
                }
            }
            return r;
        }
        vector<int> findStarts(){
            vector<int> result;
            for(int i = 0; i < size; i++){
                if(number_of_neighbours[i] == 0){
                    result.push_back(i);
                }
            }
            return result;
        }
};

void printGraph(Graph const &graph, int n){
    for(int i = 0; i < n; i++){
        cout << i << " num of neigh: " << graph.number_of_neighbours[i] << " ---> ";

        for(pair<int, int> v: graph.adjList[i]){
            cout << "{" << v.second << " " << -v.first << "} ";
        }
        cout << "\n";
    }
}

void dfs(Graph &graph, int read, vector<pair<int, int>> &path){
    graph.number_of_visited++;
    graph.visited[read] = 1;
    for(auto x: graph.adjList[read]){
        if(!graph.visited[x.second]){
            path.push_back({x.second, -x.first});
            dfs(graph, x.second, path);
            break;
        }
    }    
}

string tokenize(const string& s) {
    if (!s.size()) {
        return "";
    }
    stringstream ss;
    ss << s[0];
    for (int i = 1; i < s.size(); i++) {
        if (i%60==0) ss << '\n';
        ss << s[i];
    }
    return ss.str();
}

int main(){
    vector<string> input;
    string line;
    int size = 0;
    while(getline(cin, line)) {
        if(line[0] != '>'){
            input.push_back(line);
            size++;
        }
    }

    Graph graph(size);

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(i != j){
                int w = calculate_overlap(input[i], input[j]);
                if(w > 0){
                    graph.addEdge(i, j, -w);
                }
            }
        }
    }
    //printGraph(graph, size); 

    //cout << graph.findNumberOfStarts() << "\n";
    vector<pair<int, int>> path;

    priority_queue<pair<int, int>> starts;
    for(int i = 0; i < size; i++){
        starts.push({-graph.number_of_neighbours[i], i});
    }

    ofstream OutputFile("contigs.fasta");
    int substring;
    pair<int, int> current;

    while((float) graph.number_of_visited / (float) graph.size < 0.95){
        //cout << "im starting building contig\n";
        current = starts.top();
        starts.pop();
        if(!graph.visited[current.second]){
            path.push_back({current.second, 0});
            dfs(graph, current.second, path);
            string contig = "";
            for(auto y: path){
                contig = contig + input[y.first].substr(y.second, 80);
            }
            OutputFile << ">my contig\n" + tokenize(contig) + "\n";
            path.clear();
            //cout << "coverage lvl " << (float) graph.number_of_visited / (float) graph.size << "\n";
        }
    }

    //cout << graph.findNumberOfStarts();
}