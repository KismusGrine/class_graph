#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <queue>

using namespace std;

struct Node {
    int vertex;
    unsigned int len;
};

class Graph {
    // Список смежности
    vector<list<int>> Vertex;

// Количество вершин и ребер/дуг
    int count_vertex;
    int count_edge;

// Конструкторы
public:
    Graph();

    Graph(int num);

    Graph(int **matrix, int num);

    void Add_vertex();

    void Add_edge(int s, int f);

    void Delete_vertex();

    // Сделать
    void Delete_edge(int s, int f);

    unsigned int bfs(int start, int finish);

    unsigned int dfs();
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

Graph::Graph(int **matrix, int num) {
    count_vertex = 0;
    count_edge = 0;

    for (int i = 0; i < num; i++) {
        Add_vertex();
    }

    for (int i = 0; i < num; i++) {
        for (int j = i + 1; j < num; j++) {
            if (matrix[i][j] == 1) {
                Add_edge(i, j); //функция добавляет ребро и в ту и в другую сторону, увеличивает количество ребер
            }
        }
    }
} //в коментах есть вопросы

void Graph::Add_vertex() {
    list<int> ver; //Пустой лист?
    Vertex.push_back(ver);
    count_vertex++;
}

void Graph::Add_edge(int s, int f) {
    if (Vertex.size() >= s && Vertex.size() >= f) { 
        Vertex[s-1].push_back(f);
        count_edge++;
    }
} // Может так сделать? Не будет дубликатов в листах и будет правильный список смежности

// TODO пересмотреть
void Graph::Delete_edge(int s, int f) {
    if (Vertex.size() >= s) {
        for (auto iter = Vertex[s].begin(); iter != Vertex[s].end(); iter++) {
            if (*iter == f)
                iter = Vertex[s].erase(iter);
        }
    }

    if (Vertex.size() >= f) {
        for (auto iter = Vertex[s].begin(); iter != Vertex[s].end(); iter++) {
            if (*iter == s)
                iter = Vertex[s].erase(iter);
        }
    }
}

unsigned int Graph::bfs(int start, int finish) {
    queue<Node> q;
    Node tmp = {start, 0};
    q.push(tmp);

    while (!q.empty()) {
        tmp = q.front();
        tmp.len++;
        q.pop();
        int vert = tmp.vertex;


        for (auto iter = Vertex[vert].begin(); iter != Vertex[vert].end(); iter++) {
            if (*iter == finish) {
                return tmp.len;
            }

            tmp.vertex = *iter;
            q.push(tmp);
        }
    }

    return -1;
}

int main() {
    cout << "Hello, world" << endl;
    return 0;
}
