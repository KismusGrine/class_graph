#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Graph {
    vector<list<int>> Vertex;

// Количество вершин и ребер/дуг
    int count_vertex;
    int count_edge;

// Конструкторы
public:
    Graph();

    Graph(int num);

    Graph(int **A, int num);

    void Add_Edge(int s, int f);

};

Graph::Graph() {
    count_edge = 0;
    count_vertex = 0;
}

Graph::Graph(int num) {
    count_vertex = num;
    count_edge = 0;

    list<int> ver;
    for (int i = 0; i < num; i++)
        Vertex.push_back(ver);
}

Graph::Graph(int **A, int num) {
    count_vertex = num;
    count_edge = 0;


}


int main() {

    return 0;
}