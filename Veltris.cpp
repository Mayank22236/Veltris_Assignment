#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};

int makeConnected(int n, vector<vector<int>>& connections) {
    int edges = connections.size();
    // Not enough connections to connect all workstations
    if (edges < n - 1) {
        return -1; 
    }
    
    UnionFind uf(n);
    for (const auto& conn : connections) {
        uf.unite(conn[0], conn[1]);
    }
    
    int components = 0;
    for (int i = 0; i < n; ++i) {
        if (uf.find(i) == i) {
            components++;
        }
    }
    
     // All workstations are already connected
    if (components == 1) {
        return 0; 
    }
    
     // Kruskal's algorithm to find the minimum number of edges 
    sort(connections.begin(), connections.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[2] < b[2];
    });
    
    UnionFind uf_mst(n);
    int extraEdges = 0;
    
    for (const auto& conn : connections) {
        int u = conn[0];
        int v = conn[1];
        
        if (uf_mst.find(u) != uf_mst.find(v)) {
            uf_mst.unite(u, v);
            extraEdges++;
        // If we cannot connect all workstations
        // Number of extra edges needed to connect all components
            if (extraEdges == n - components) {
                return components - 1; 
            }
        }
    }
    return -1; 
}

int main() {
    int n = 4;
    vector<vector<int>> connections = {{0, 1}, {0, 2}, {1, 2}};
    
    int result = makeConnected(n, connections);
    cout << "Minimum number of cable modifications: " << result << endl;
    
    return 0;
}