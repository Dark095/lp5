// Parallel BFS and DFS using OpenMP
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

vector<vector<int>> graph;

void parallelBFS(int start, int n) {
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "Parallel BFS Traversal: ";

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            int node;
            #pragma omp critical
            {
                node = q.front();
                q.pop();
                cout << node << " ";
            }

            for (auto neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    cout << endl;
}

void parallelDFS(int start, int n) {
    vector<bool> visited(n, false);
    stack<int> s;
    s.push(start);

    cout << "Parallel DFS Traversal: ";

    while (!s.empty()) {
        int node;
        #pragma omp critical
        {
            node = s.top();
            s.pop();
        }

        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";

            #pragma omp parallel for
            for (int i = graph[node].size() - 1; i >= 0; i--) {
                int neighbor = graph[node][i];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        s.push(neighbor);
                    }
                }
            }
        }
    }
    cout << endl;
}

int main() {
    int n = 6;
    graph.resize(n);

    graph[0] = {1, 2};
    graph[1] = {0, 3, 4};
    graph[2] = {0, 4};
    graph[3] = {1, 5};
    graph[4] = {1, 2, 5};
    graph[5] = {3, 4};

    parallelBFS(0, n);
    parallelDFS(0, n);

    return 0;
}

