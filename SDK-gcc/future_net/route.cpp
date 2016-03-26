#include <stdio.h>
#include <string.h>
#include <time.h>
#include <queue>
#include <stack>
#include <algorithm>
#include "route.h"
#include "lib_record.h"
#define N 602
#define M 4802
typedef pair<int, int> pii;

int startTime;
bool demand[N];
vi demandVec;

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
                if (d[k] + a[i][2] - demand[j] < d[j])
                {
                    d[j] = d[k] + a[i][2] - demand[j];
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
    int a[M][4], b[N], bb[N];
    int d[N]; // shortest path distance
    int p[N][2]; // spfa previous node
    bool v[N]; // visited
    int s; // edge count
    int t; // end point
    void init()
    {
        ans = 252645135;
        s = 0;
        memset(b, 0, sizeof(b));
        memset(v, 0, sizeof(v));
    }
    void add(int i, int j, int k, int l)
    {
        a[++s][0] = b[i];
        b[i] = s;
        a[s][1] = j;
        a[s][2] = k;
        a[s][3] = l;
    }
    void astar(int k, int left, int cost)
    {
        stack<pii> st;
        st.push(pii(k, cost));
        v[k] = 1;
        memcpy(bb, b, sizeof(b));
        while (!st.empty())
        {
            if (time(NULL) - startTime > 19)
                return;
            pii temp = st.top();
            k = temp.first;
            cost = temp.second;
            bool flag = 1;
            if (left == 0)
            {
                int dis = cost + spfa(k);
                if (dis < ans)
                {
                    ans = dis;
                    stack<int> pathStack;
                    for (int i = t; i != k; i = p[i][0])
                        pathStack.push(p[i][1]);
                    pathBest = path;
                    while (!pathStack.empty())
                    {
                        pathBest.push_back(pathStack.top());
                        pathStack.pop();
                    }
                }
            }
            else
            {
                for (int i = bb[k]; i; i = a[i][0])
                {
                    int j = a[i][1];
                    if (v[j])
                        continue;
                    if (cost + a[i][2] + revGraph.d[j] + left - demand[j] >= ans)
                        continue;
                    if (j == t)
                    {
                        if (left == 0 && cost + a[i][2] < ans)
                        {
                            ans = cost + a[i][2];
                            pathBest = path;
                            pathBest.push_back(a[i][3]);
                            if (ans == 143)
                                return;
                        }
                        continue;
                    }
                    path.push_back(a[i][3]);
                    cost += a[i][2];
                    if (demand[j])
                        --left;
                    v[j] = 1;
                    st.push(pii(j, cost));
                    bb[k] = a[i][0];
                    flag = 0;
                    break;
                }
            }
            if (flag)
            {
                v[k] = 0;
                bb[k] = b[k];
                if (demand[k])
                    ++left;
                st.pop();
                path.pop_back();
            }
        }
    }
    int spfa(int x)
    {
        queue<int> q;
        memset(d, 15, sizeof(d));
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
                    p[j][0] = k;
                    p[j][1] = a[i][3];
                    if (!v[j])
                    {
                        q.push(j);
                        v[j] = 1;
                    }
                }
            }
            v[k] = 0;
        }
        return d[t];
    }
    void greedyGetPath(int x, int y, int left)
    {
        stack<int> pathStack;
        v[x] = 1;
        for (int i = y; i != x; i = p[i][0])
        {
            v[i] = 1;
            pathStack.push(p[i][1]);
            left -= demand[i];
        }
        while (!pathStack.empty())
        {
            path.push_back(pathStack.top());
            pathStack.pop();
        }
    }
    void greedy(int k, int tot)
    {
        while (1)
        {
            if (time(NULL) - startTime > 19)
                return;
            random_shuffle(demandVec.begin(), demandVec.end());
            memset(v, 0, sizeof(v));
            spfa(k);
            int left = tot - 1;
            int next = demandVec[0];
            int cost = d[next];
            if (cost + revGraph.d[next] >= ans)
                continue;
            path.clear();
            greedyGetPath(k, next, left);
            if (left > 0 && v[t])
                continue;
            if (cost + revGraph.d[next] + left >= ans)
                continue;
            for (int i = 1; i < tot; ++i)
                if (!v[demandVec[i]])
                {
                    spfa(next);
                    cost += d[demandVec[i]];
                    if (cost + revGraph.d[demandVec[i]] >= ans)
                    {
                        cost = ans;
                        break;
                    }
                    greedyGetPath(next, demandVec[i], left);
                    if (left > 0 && v[t])
                    {
                        cost = ans;
                        break;
                    }
                    if (cost + revGraph.d[next] + left >= ans)
                    {
                        cost = ans;
                        break;
                    }
                    next = demandVec[i];
                }
            if (cost >= ans)
                continue;
            spfa(next);
            cost += d[t];
            if (cost < ans)
            {
                ans = cost;
                greedyGetPath(next, t, left);
                pathBest = path;
            }
        }
    }
    void output()
    {
        for (int i = 0; i < (int)pathBest.size(); ++i)
            record_result(pathBest[i]);
        // printf("%d\n", ans);
    }
} g;

bool cmp(const vector<int> &a, const vector<int> &b) {
    return a[3] > b[3];
}

void search_route(vector<vi> topo, vi demandVector)
{
    // topo[i]: LinkID, SourceID, DestinationID, Cost
    startTime = time(NULL);
    int edgenum = (int)topo.size();
    int s = demandVector[0];
    int t = demandVector[1];
    int tot = demandVector.size();
    sort(topo.begin(), topo.end(), cmp);
    g.init();
    for (int i = 0; i < edgenum; ++i)
    {
        g.add(topo[i][1], topo[i][2], topo[i][3], topo[i][0]);
        revGraph.add(topo[i][2], topo[i][1], topo[i][3]);
    }
    g.t = t;
    memset(demand, 0, sizeof(demand));
    for (int i = 2; i < tot; ++i)
    {
        demand[demandVector[i]] = 1;
        demandVec.push_back(demandVector[i]);
    }
    revGraph.spfa(g.t);
    if (edgenum > 400)
        g.greedy(s, tot - 2);
    else
    {
        g.ans = 144;
        g.astar(s, tot - 2, 0);
    }
    g.output();
}
