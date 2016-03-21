#include <stdio.h>
#include <string.h>
#include <time.h>
#include <queue>
#include "route.h"
#include "lib_record.h"
#define N 602
#define M 4802

int startTime;

struct graph
{
    int a[M][3], b[N];
    int d[N]; // shortest path distance
    bool v[N]; // visited
    int s; // node count
    void init()
    {
        s = 0;
        memset(b, 0, sizeof(b));
    }
    void add(int i, int j, int k)
    {
        a[++s][0] = b[i];
        b[i] = s;
        a[s][1] = j;
        a[s][2] = k;
    }
    void spfa(int x)
    {
        queue<int> q;
        memset(d, 15, sizeof(d));
        memset(v, 0, sizeof(v));
        v[x] = 1;
        d[x] = 0;
        q.push(x);
        while (!q.empty())
        {
            int k = q.front();
            q.pop();
            for (int i = b[k]; i; i = a[i][0])
            {
                int j = a[i][1];
                if (d[k] + a[i][2] < d[j])
                {
                    d[j] = d[k] + a[i][2];
                    if (!v[j])
                    {
                        q.push(j);
                        v[j] = 1;
                    }
                }
            }
            v[k] = 0;
        }
    }
} revGraph;

struct Solver
{
    vi path, pathBest;
    int ans; // best cost
    int a[M][4], b[N];
    bool demand[N];
    bool v[N]; // visited
    int s; // node count
    int t; // end point
    void init()
    {
        ans = 1 << 30;
        s = 0;
        memset(b, 0, sizeof(b));
        memset(demand, 0, sizeof(demand));
    }
    void add(int i, int j, int k, int l)
    {
        a[++s][0] = b[i];
        b[i] = s;
        a[s][1] = j;
        a[s][2] = k;
        a[s][3] = l;
    }
    void dfs(int k, int left, int cost)
    {
        if (time(NULL) - startTime > 9)
            return;
        if (cost + revGraph.d[k] + left - demand[k] >= ans)
            return;
        if (k == t)
        {
            if (left == 0 && cost < ans)
            {
                ans = cost;
                pathBest = path;
            }
            return;
        }
        v[k] = 1;
        for (int i = b[k]; i; i = a[i][0])
        {
            int j = a[i][1];
            if (v[j])
                continue;
            int k = a[i][2];
            path.push_back(a[i][3]);
            if (demand[j])
                dfs(j, left - 1, cost + k);
            else
                dfs(j, left, cost + k);
            path.pop_back();
        }
        v[k] = 0;
    }
    void output()
    {
        for (int i = 0; i < (int)pathBest.size(); ++i)
            record_result(pathBest[i]);
    }
} g;

void search_route(vector<vi> topo, vi demand)
{
    // topo[i]: LinkID, SourceID, DestinationID, Cost
    startTime = time(NULL);
    g.init();
    for (int i = 0; i < (int)topo.size(); ++i)
    {
        g.add(topo[i][1], topo[i][2], topo[i][3], topo[i][0]);
        revGraph.add(topo[i][2], topo[i][1], topo[i][3]);
    }
    int s = demand[0];
    g.t = demand[1];
    int tot = demand.size();
    for (int i = 2; i < tot; ++i)
        g.demand[demand[i]] = 1;
    revGraph.spfa(g.t);
    g.dfs(s, tot - 2, 0);
    g.output();
}
