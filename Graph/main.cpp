#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Graph {
    // Список смежности
    vector <list<int>> Vertex;

// Количество вершин и ребер/дуг
    int count_vertex;
    int count_edge;

// Конструкторы
public:
    Graph();

    Graph(int num);

    Graph(int **A, int num);

    void Add_vertex();

    void Add_edge(int s, int f);

    void Delete_vertex();

    // Сделать
    void Delete_edge();

    void bfs();

    void dfs();
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

void Graph::Delete_edge() {

}

int main() {


    return 0;
}