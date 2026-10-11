#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

vector<vector<int>> adj;
vector<int> color, parent, dist;
vector<bool> in_odd_cycle;

void find_odd_cycles_in_component(int start) {
    queue<int> q;
    q.push(start);
    color[start] = 0;
    dist[start] = 0;
    parent[start] = -1;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : adj[u]) {
            if (color[v] == -1) {
                color[v] = 1 - color[u];
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.push(v);
            } else if (color[v] == color[u] && parent[u] != v) {
                // Found an odd cycle
                int cycle_length = dist[u] + dist[v] + 1;
                if (cycle_length % 2 == 1) {
                    // Mark all nodes in this cycle
                    vector<int> path_u, path_v;
                    int curr = u;
                    while (curr != -1) {
                        path_u.push_back(curr);
                        curr = parent[curr];
                    }
                    curr = v;
                    while (curr != -1) {
                        path_v.push_back(curr);
                        curr = parent[curr];
                    }
                    
                    // Find LCA
                    int lca = -1;
                    for (int i = 0; i < path_u.size() && i < path_v.size(); i++) {
                        if (path_u[path_u.size() - 1 - i] == path_v[path_v.size() - 1 - i]) {
                            lca = path_u[path_u.size() - 1 - i];
                        } else {
                            break;
                        }
                    }
                    
                    // Mark nodes in the cycle
                    curr = u;
                    while (curr != lca) {
                        in_odd_cycle[curr] = true;
                        curr = parent[curr];
                    }
                    in_odd_cycle[lca] = true;
                    
                    curr = v;
                    while (curr != lca) {
                        in_odd_cycle[curr] = true;
                        curr = parent[curr];
                    }
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    adj.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    
    color.assign(n + 1, -1);
    parent.assign(n + 1, -1);
    dist.assign(n + 1, 0);
    in_odd_cycle.assign(n + 1, false);
    
    for (int i = 1; i <= n; i++) {
        if (color[i] == -1) {
            find_odd_cycles_in_component(i);
        }
    }
    
    // Count ministers who cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!in_odd_cycle[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}