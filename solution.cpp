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
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    
    vector<int> color(n + 1, -1);  // -1: unvisited, 0/1: colors
    vector<int> component_nodes;
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
            
            while (!q.empty() && is_bipartite) {
                int u = q.front();
                q.pop();
                
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                        component_nodes.push_back(v);
                    } else if (color[v] == color[u]) {
                        is_bipartite = false;
                        break;
                    }
                }
            }
            
            // If component is bipartite, all nodes in it cannot be invited
            if (is_bipartite) {
                answer += component_nodes.size();
            }
        }
    }
    
    cout << answer << endl;
    
    return 0;
}