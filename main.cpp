#include <iostream>
#include <vector>
#include <exception>
using std::vector;
using std::cin;
using std::cout;

struct  edge{
    int finish, edge_id;
    edge() = default;
    edge(int finish, int id): finish(finish), edge_id(id) {}
};

vector <vector <edge> > read_graph(int& n, int& m) {
    int d;
    cout << "Give the number of vertexes in the ribbon graph N\n";
    cin >> n;
    cout << "Give the number of edges in the ribbon graph M\n";
    cin >> m;
    vector <vector <int> > expension(n);
    cout << "For each vertex give it's degree d and then d numbers between 1 and m - indexes of edges in right order\n";
    for (int i = 0; i < n; ++i) {
        cin >> d;
        expension[i].resize(d);
        for (int j = 0; j < d; ++j) {
            cin >> expension[i][j];
            --expension[i][j];
        }
    }
    vector <std::pair <int, int> > edges(m, std::make_pair(-1, -1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < expension[i].size(); ++j) {
            int e = expension[i][j];
            if (edges[e].first == -1) {
                edges[e].first = i;
            } else if (edges[e].second == -1) {
                edges[e].second = i;
            } else {
                cout << "Wrong input\n";
                throw std::logic_error("Invalid input data");
            }
        }
    }
    vector <vector <edge> > res(n);
    for (int i = 0; i < n; ++i) {
        res[i].resize(expension[i].size());
        for (int j = 0; j < expension[i].size(); ++j) {
            int e = expension[i][j];
            if (edges[e].first == i) {
                res[i][j] = edge(edges[e].second, e);
            } else {
                res[i][j] = edge(edges[e].first, e);
            }
        }
    }
    return res;
}

void make_circle_dfs(vector <vector <edge> >& expenion, vector <int>& circle, vector <bool>& used, int edge_id, int u) {
    used[u] = true;
    int deg = expenion[u].size();
    int start = -1;
    for (int i = 0; i < deg; ++i) {
        if (expenion[u][i].edge_id == edge_id) {
            start = i;
            break;
        }
    }
    for (int i = start + 1; i < deg; ++i) {
        int w = expenion[u][i].finish;
        if (used[w]) {
            circle.push_back(expenion[u][i].edge_id);
        } else {
            make_circle_dfs(expenion, circle, used, expenion[u][i].edge_id, w);
        }
    }
    for (int i = 0; i < start; ++i) {
        int w = expenion[u][i].finish;
        if (used[w]) {
            circle.push_back(expenion[u][i].edge_id);
        } else {
            make_circle_dfs(expenion, circle, used, expenion[u][i].edge_id, w);
        }
    }
}

vector <int> make_circle(vector <vector <edge> >& expention, int n, int m) {
    vector <bool> used(n,false);
    vector <int> circle;
    make_circle_dfs(expention, circle ,used, -1, 0);
    vector <bool> is_rest_edge(m, false);
    if (circle.size() != 2 * (m - n + 1)) {
        throw std::logic_error("graph is not connected");
    }
    for (int i = 0; i < circle.size(); ++i) {
        is_rest_edge[circle[i]] = true;
    }
    vector <int> new_edge_ids(m);
    int now_id = 0;
    for (int i = 0; i < m; ++i) {
        if (is_rest_edge[i]) {
            new_edge_ids[i] = now_id;
            ++now_id;
        } else {
            new_edge_ids[i] = -1;
        }
    }
    for (int i = 0; i < circle.size(); ++i) {
        circle[i] = new_edge_ids[circle[i]];
    }
    return circle;
}

void add_edge(vector <vector <int> >& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

vector <vector <int> > make_graph(vector <int>& circle) {
    int n = circle.size();
    vector <vector <int> > positions(n / 2);
    for (int i = 0; i < n; ++i) {
        positions[circle[i]].push_back(i);
    }
    vector <vector <int> > graph(n);
    for (int i = 0; i < n / 2; ++i) {
        add_edge(graph, positions[i][0], (positions[i][1] + 1) % n);
        add_edge(graph, (positions[i][0] + 1) % n, positions[i][1]);
    }
    return graph;
}

void count_components_dfs(vector <vector <int> >& graph, vector <bool>& used, int u) {
    used[u] = true;
    for (int i = 0; i < graph[i].size(); ++i) {
        int w = graph[u][i];
        if (!used[w]) {
            count_components_dfs(graph, used, w);
        }
    }
}

int count_components(vector <vector <int> >& graph) {
    int n = graph.size();
    if (n == 0) {
        return 1;
    }
    vector <bool> used(n, false);
    int res = 0;
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            count_components_dfs(graph, used, i);
            ++res;
        }
    }
    return res;
}

void print(int handles, int holes) {
    cout << "The ribbon graph is homeomorphous to a sphere with " << handles << " handles and " << holes << " holes\n";
}

int main() {
    int n, m;
    vector <vector <edge> > expension = read_graph(n, m);
    vector <int> circle = make_circle(expension, n, m);
    vector <vector <int> > graph = make_graph(circle);
    int k = count_components(graph);
    print((m + 2 - n - k) / 2, k);
    return 0;
}
