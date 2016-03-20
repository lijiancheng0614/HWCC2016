#include <stdio.h>
#include <string.h>
#include <time.h>
#include "route.h"
#include "lib_record.h"
#define N 602
#define M 4802

int startTime;

struct graph
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
        ans = -1;
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
        if (k == t && left == 0)
        {
            if (cost < ans || ans < 0)
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
    }
    int s = demand[0];
    g.t = demand[1];
    int tot = demand.size();
    for (int i = 2; i < tot; ++i)
        g.demand[demand[i]] = 1;
    g.dfs(s, tot - 2, 0);
    g.output();
}
