P3092 [USACO13NOV]No Change G 状态压缩+二分

这是一道和 [P3694 邦邦的大合唱站队] 非常相似的蓝色题目，前面一题刚开始不太会做，看了题解开导思路，这道题自己独立做出来了。
dp[i]的下标i代表已经用了的硬币，值代表买到的最后一样东西。转移方程就是枚举比i中少一个1的状态转移到i状态，在那个状态的值往后二分搜出当前硬币能往后买到的最后面的物品编号，然后取最大值。当当前的状态可以买到所有物品后，更新答案ans。ans就是没有取的硬币的和。

```c++
#include <bits/stdc++.h>
#define endl "\n"
#define MAXN 5000005
using namespace std;
int n,m;
int val[MAXN];
int dp[1<<17];
int ans=-1;
int coin[17],sum;
int get(int pos,int v)
{
    int L=pos,R=m;
    while(L<R)
    {
        int mid=(L+R+1)>>1;
        if(val[mid]-val[pos]<=v)
            L=mid;
        else
            R=mid-1;
    }
    return L;
}
void update(int x)
{
    int ret=sum;
    for(int i=0;i<17;i++)
        if(x&(1<<i))
    {
        ret-=coin[i+1];
        int f=x&~(1<<i);
        dp[x]=max(get(dp[f],coin[i+1]),dp[x]);
    }
    if(dp[x]==m)
        ans=max(ans,ret);
}
signed main()
{
    cin>>n>>m;
    for(int i=1;i<=n;i++)
    {
        cin>>coin[i];
        sum+=coin[i];
    }
    for(int i=1;i<=m;i++)
    {
        cin>>val[i];
        val[i]+=val[i-1];
    }
    for(int i=1;i<(1<<n);i++)
        update(i);
    cout<<ans;
}

```

## P2656 采蘑菇

![img](https://img-blog.csdnimg.cn/795d6489c22e48a48a9c20e741783e8a.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAZ2xvcmlvdXNfZHJlYW0=,size_20,color_FFFFFF,t_70,g_se,x_16)

题意描述：
N 个草丛，M 条单向的小道，经过每条道时可以采走这条道上的所有蘑菇，蘑菇有一个恢复系数，即在采过这一片蘑菇后又会新长出蘑菇，求从给定的点出发能采到的蘑菇最大值。

算法分析：
很容易想到最后要求一个最大值，可以用树形 DP，不过本人比较懒，推荐 spfa 求最长路，既好理解又好写。

首先明确一点，如果走到一个环中，里面的所有蘑菇包括每一次恢复后新长出来的蘑菇都能采到。

于是想到了什么？ Tarjan 缩点，成为一个有向无环图，把每一个强连通分量里面的蘑菇总数求出来，最后跑一边最长路即可。

然后要注意细节：这道题不需要用 double 存，会有精度问题。可以在存图的时候先把 k 乘 10，在后面的处理中把 k 除以 10 就行了。

代码+注释：

蓝题，思路不算很难，题意可知强连通块上每条边的值都可一压榨干净，其他边最多走一次。因此先缩点，连通块内所有边的值先算出来，然后再重新构图，这时候是个无环图，跑一边动态规划输出最大值即可。

```cpp
#include <bits/stdc++.h>
#define int long long
#define endl "\n"
#define MAXN 1000005
using namespace std;
struct EDGE
{
    int to,w,next;
    double co;
} edge[MAXN];
int head[MAXN],ptr;
void add_edge(int u,int v,int w,double co)
{
    edge[++ptr].to=v;
    edge[ptr].w=w;
    edge[ptr].co=co;
    edge[ptr].next=head[u];
    head[u]=ptr;
}
int n,m,s;
int belong[MAXN],scc;
int val[MAXN];
int dfn[MAXN],low[MAXN];
int idx;
bool in_stk[MAXN];
stack<int> stk;
void tarjan(int now)
{
    dfn[now]=low[now]=++idx;
    stk.push(now);
    in_stk[now]=1;
    for(int p=head[now]; p; p=edge[p].next)
    {
        int to=edge[p].to;
        if(!dfn[to])
        {
            tarjan(to);
            low[now]=min(low[now],low[to]);
        }
        else if(in_stk[to])
            low[now]=min(low[now],dfn[to]);
    }
    if(dfn[now]==low[now])
    {
        int temp;
        scc++;
        do
        {
            temp=stk.top();
            belong[temp]=scc;
            in_stk[temp]=0;
            stk.pop();
        }
        while(temp!=now);
    }
}
struct E
{
    int u,v,w;double co;
}e[MAXN];
int get(int p)
{
    int ret=0;
    int res=e[p].w;
    double co=e[p].co;
    while(res)
        ret+=res,res*=co;
    return ret;
}
void dfs(int now)
{
    for(int p=head[now];p;p=edge[p].next)
    {
        int to=edge[p].to,w=edge[p].w;
        val[to]=max(val[to],val[now]+w);
        dfs(to);
    }
}
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0),cout.tie(0);
    cin>>n>>m;
    for(int i=1;i<=m;i++)
    {
        int u,v,w;double co;
        cin>>u>>v>>w>>co;
        add_edge(u,v,w,co);
        e[i].u=u,e[i].v=v,e[i].w=w,e[i].co=co;
    }
    cin>>s;
    tarjan(s);
    ptr=0;
    memset(head,0,sizeof(head));
    memset(edge,0,sizeof(edge));
    for(int i=1;i<=m;i++)
    {
        int u=belong[e[i].u],v=belong[e[i].v],w=e[i].w;
        if(u==v)
        {
            val[u]+=get(i);
            continue;
        }
        add_edge(u,v,w,0);
    }
    dfs(belong[s]);
    int ans=0;
    for(int i=1;i<=scc;i++)
        ans=max(ans,val[i]);
    cout<<ans;
}

```

