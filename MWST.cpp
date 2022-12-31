//#!/usr/local/bin/python3
#include <iostream>   // io
#include <string>     // strings
#include <cctype>     // isdigit
#include <typeinfo>   // checking data types
#include <utility>    // pairs
#include <fstream>    // FILE* objs
#include <vector>     // vector push back
#include <algorithm>  // sort
#include <iomanip>    // fixed, setprecision()
using namespace std;

typedef pair<int, int> edgePair; //include typedef?
typedef pair<double, int> indexWeightPair;

struct GraphObj{
    int Vertices;
    int Edges;

    vector< pair<indexWeightPair, edgePair>> GraphEdges;
    GraphObj(int Vert, int Edg){
        this->Vertices = Vert;
        this->Edges = Edg; 
        GraphEdges.push_back({{-1.00, -1},{-1,-1}}); //first element means nothing
    }

    void addEdge(int u, int v, double wght, int idx){
        GraphEdges.push_back({{wght, idx}, {u, v}}); // is this valid?
    }

    double Kruskal(ofstream &outputFile);
};

struct DisjointSet{
        int *parent; //dynamically create an array of size V vertices
        int size;   
        int *rank;   //sort by rank, not size

    DisjointSet(int numVert){
        this->size = numVert;
        rank = new int[numVert + 1];
        parent = new int[numVert + 1];
        makeSet(numVert);
    }

    //returns the abs root of a disjoint set (top most node that points to itself)
    //keep recurring if the value isn't itself
    //will either be itself if it is the root node of a forest aka size > 1
    //or a root note of a singleton 
    void makeSet(int numVert){
        this->parent[0] = -1;
        this->rank[0] = -1;  //dummy value to start counting Vs from one

        for(int i = 1; i <= numVert; i++){
            this->parent[i] = i; //make it parent of itself, or undef (-1), pick later
            this->rank[i] = 0;
        }
    }

    int findSet(int vert){
        if(parent[vert] != vert){
            return parent[vert] = findSet(parent[vert]); //recur
        }
        else{
            return vert; //return the vertex itself, aka did not find 
        }
    }

    //union by rank
    void unionSet(int x, int y){
        int xRoot = findSet(x), yRoot = findSet(y);

        //if the root nodes are the same, do no work
        if(xRoot == yRoot){
            return;
        }   
        if(rank[xRoot] > rank[yRoot]){
            parent[y] = x;
        }  
        else if (rank[xRoot] < rank[yRoot]){
            parent[x] = y;
        }
        else{
            parent[xRoot] = yRoot;
            rank[yRoot] = rank[xRoot] + 1; 
        }       
    }    
};

double GraphObj::Kruskal(ofstream &outputFile){

    double minWeight = 0.00;

    //outputFile << "test" << endl;

    sort(GraphEdges.begin() + 1, GraphEdges.end()); //sort weight-wise asc

    DisjointSet inGraph(Vertices);
    vector<pair<indexWeightPair, edgePair>>::iterator it;

    // ignore the first index i = 0. hold dummy values and move to i=1 to size
    // GraphEdges has a pair of pairs: edgepair (u,v), index weight pair (w, i)

    // vector< pair<indexWeightPair, edgePair>> GraphEdges;
    //                        {i,w},{u,v}

    for(it = GraphEdges.begin() + 1; it != GraphEdges.end(); it++){
        int u = inGraph.findSet(it->second.first);
        int v = inGraph.findSet(it->second.second);

        if(u != v){
            minWeight += it->first.first;

            inGraph.unionSet(u, v);
            double weightAsDouble = it->first.first;

            outputFile << setfill(' ') << setw(4) << it->first.second << ": (" << it->second.first << ", " << it->second.second;
            outputFile << ") " << fixed << setprecision(1) << weightAsDouble << endl;
        }
    }
    return minWeight;
}

int main(int argc, char *argv[]){

    ifstream in;
    ofstream out;
    int vertices;
    int edges;

    if (argc != 3) { //check for valid command line input
        cout << "Usage: " << argv[0] << "<input file> <output file> " << endl;
        exit(EXIT_FAILURE);
    }
    in.open(argv[1]);
    if(!in){
        cout << "Unable to open file " << argv[1] << " for reading " << endl;
        exit(EXIT_FAILURE);
    }
    out.open(argv[2]);
    if (!out) {
        cout << "Unable to open file " << argv[2] << " for writing " << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    int vertStart;
    int vertEnd;
    double weight;
    int index;

    if(in.is_open()){
        getline(in, line);
            vertices = stoi(line);
        getline(in, line);
            edges = stoi(line);

        GraphObj G(vertices, edges);
        
        //cout << "got here" << endl;
        while(!in.eof()){

            //first two lines stored vert and edges
            //for the cases m + 2 to last line of input file
            for(int i = 1; i < edges; i++){

                getline(in, line,' ');
                vertStart = stoi(line);

                getline(in, line,' ');
                vertEnd = stoi(line);

                getline(in, line,'\n');
                weight = stod(line);

                index = i;
                G.addEdge(vertStart, vertEnd, weight, index);
            }

            //GETS THE LAST LINE 
            //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
            index++;

            getline(in, line,' ');
            vertStart = stoi(line); 

            getline(in, line,' ');
            vertEnd = stoi(line);

            getline(in, line, ' ');
            weight = stod(line);

            G.addEdge(vertStart, vertEnd, weight, index);
            //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

            double minWeight = G.Kruskal(out);
            out <<"Total Weight = " << fixed << showpoint << setprecision(2) << minWeight;
            //printf("Total Weight = %0.2f", minWeight);
        }  
    }
    in.close();
    out.close();
    return 0;
}
