#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    // Build adjacency list for complement graph
    vector<vector<int>> adj(n + 1);
    vector<int> degree(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
        degree[x]++;
        degree[y]++;
    }
    
    vector<int> color(n + 1, -1);  // -1: unvisited, 0/1: colors
    vector<int> component_nodes;
    vector<bool> can_be_invited(n + 1, false);
    int answer = 0;
    
    for (int i = 1; i <= n; i++) {
        if (color[i] == -1) {
            // Start BFS for new component
            queue<int> q;
            q.push(i);
            color[i] = 0;
            component_nodes.clear();
            component_nodes.push_back(i);
            
            bool is_bipartite = true;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                        component_nodes.push_back(v);
                    } else if (color[v] == color[u]) {
                        is_bipartite = false;
                    }
                }
            }
            
            if (!is_bipartite && component_nodes.size() >= 3) {
                // This component contains an odd cycle
                // A node can be part of an odd cycle if and only if it has degree >= 2
                // and is in a non-bipartite component
                for (int node : component_nodes) {
                    if (degree[node] >= 2) {
                        can_be_invited[node] = true;
                    }
                }
            }
        }
    }
    
    // Count ministers who cannot be invited
    for (int i = 1; i <= n; i++) {
        if (!can_be_invited[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}