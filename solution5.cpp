#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

vector<int> parent, color, dist;
vector<vector<int>> adj;

bool dfs_find_odd_cycle(int u, int pu, int& cycle_start, int& cycle_end) {
    color[u] = 1; // visiting
    parent[u] = pu;
    
    for (int v : adj[u]) {
        if (v == pu) continue;
        
        if (color[v] == 0) {
            if (dfs_find_odd_cycle(v, u, cycle_start, cycle_end)) {
                return true;
            }
        } else if (color[v] == 1) {
            // Found a cycle
            int cycle_length = dist[u] - dist[v] + 1;
            if (cycle_length % 2 == 1) {
                // Found odd cycle
                cycle_start = v;
                cycle_end = u;
                return true;
            }
        }
    }
    
    color[u] = 2; // visited
    return false;
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
    
    vector<bool> can_be_invited(n + 1, false);
    vector<bool> visited(n + 1, false);
    
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            // Find connected component
            vector<int> component;
            queue<int> q;
            q.push(i);
            visited[i] = true;
            component.push_back(i);
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                        component.push_back(v);
                    }
                }
            }
            
            // Check if this component has an odd cycle
            color.assign(n + 1, 0);
            parent.assign(n + 1, -1);
            dist.assign(n + 1, 0);
            
            bool found_odd_cycle = false;
            for (int start : component) {
                if (color[start] == 0) {
                    queue<int> bfs_q;
                    bfs_q.push(start);
                    color[start] = 1;
                    dist[start] = 0;
                    
                    while (!bfs_q.empty() && !found_odd_cycle) {
                        int u = bfs_q.front();
                        bfs_q.pop();
                        
                        for (int v : adj[u]) {
                            if (color[v] == 0) {
                                color[v] = 3 - color[u]; // alternate between 1 and 2
                                dist[v] = dist[u] + 1;
                                parent[v] = u;
                                bfs_q.push(v);
                            } else if (color[v] == color[u]) {
                                // Found odd cycle
                                found_odd_cycle = true;
                                break;
                            }
                        }
                    }
                    if (found_odd_cycle) break;
                }
            }
            
            // Debug output
            cout << "Component: ";
            for (int node : component) {
                cout << node << " ";
            }
            cout << " (size=" << component.size() << ", has_odd_cycle=" << found_odd_cycle << ")" << endl;
            
            if (found_odd_cycle && component.size() >= 3) {
                // All nodes in this component can be invited
                cout << "  -> All nodes can be invited" << endl;
                for (int node : component) {
                    can_be_invited[node] = true;
                }
            } else {
                cout << "  -> Nodes cannot be invited" << endl;
            }
        }
    }
    
    // Count ministers who cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!can_be_invited[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}