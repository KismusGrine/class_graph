#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <list>

// ориентированный, не взвешенный граф
/*
    данные:
        n - число вершин в графе
        для i = 0..n-1:
            adj[i] - список смежности вершины i, т.е.
                список вершин { j | есть ребро i -> j }
    операции:
        конструктор (n - число вершин)
        добавление ребра в граф
        поиск пути из пункта a в пункт b (поиск в глубину)
        поиск кратчайшего пути из пункта a в пункт b (поиск в ширину)
*/
struct Graph {
    int n;  // число вершин
    std::vector<std::list<int> > adj;  // списки смежности вершин графа
    // для i = 0..n-1 : adj[i] - это list<int> - список вершин,
    //   в которые идет ребро из вершины i

    // путь в графе
    typedef std::vector<int> Path;

    // конструктор класса Graph
    // создает граф с n вершинами
    Graph (int n_arg)
        : n(n_arg), adj(n_arg)  // adj - вектор из n пустых списков
    {}

    // добавить в граф ребро из вершины u в вершину v
    // (ребро из вершины v в вершину u не добавляется)
    void add_edge (int u, int v)
    {
        adj[u].push_back(v);
    }

    // найти путь в графе из вершины a в вершину b (поиск в глубину)
    //   path[0..k]:  path[0] = a, path[k] = b
    Path find_dfs_path (int a, int b);

    // найти кратчайший путь в графе из вершины a в вершину b
    //   path[0..k]:  path[0] = a, path[k] = b
    Path find_shortest_path (int a, int b);
};

#endif // GRAPH_H_INCLUDED
/*
    Модуль поиска путей в графе

    Реализации методов
        Graph
            find_dfs_path
            find_shortest_path
*/

#include <stack>
#include <algorithm>
#include "graph.h"

using namespace std;

namespace {

    vector<bool> vert_visited;

    // поиск в глубину
    /*
        вход
            v - текущая вершина, b - конечная вершина
        выход
            в s добавится отрезок пути из v до b
            если пути нет, то s останется прежним
    */
    bool dfs (Graph &graph, int u, int b, stack<int> &s)
        // vert_visited[u] = false
        // s.back() != u
    {
        s.push(u);
        vert_visited[u] = true;
        // vert_visited[u] = true
        // s.back() = u
        if (u == b)
            return true;

        // перечисляем ребра графа (u, v) из вершины u
        for (auto v : graph.adj[u]) {
            if (vert_visited[v])
                continue;
            bool result = dfs(graph, v, b, s);
            if (result)
                return true;
        }

        vert_visited[u] = false;
        s.pop();
        return false;
    }

}


// найти путь в графе из вершины a в вершину b (поиск в глубину)
//   path[0..k]:  path[0] = a, path[k] = b
//   если пути нет, то path - пустой вектор
Graph::Path Graph::find_dfs_path (int a, int b)
{
    vert_visited.resize(n);  // vert_visited[i] = false forall i
    stack<int> s;
    bool is_path = dfs(*this, a, b, s);
    // s содержит путь из a в b
    // если пути нет, то s пуст, is_path = 1

    Path path;
    // сложить вершины из s в path от последней до первой
    while (!s.empty()) {
        path.push_back(s.top());
        s.pop();
    }
    // перевернуть path
    reverse(path.begin(), path.end());
    return path;
}

// найти кратчайший путь в графе из вершины a в вершину b
//   path[0..k]:  path[0] = a, path[k] = b
Graph::Path Graph::find_shortest_path (int a, int b)
{
    // TODO


    // заглушка
    return find_dfs_path(a, b);
}
