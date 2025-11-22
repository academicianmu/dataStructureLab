#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int CreateGraph(MGraph *G, int vexnum, int arcnum)
{
    if (!G || vexnum <= 0 || vexnum > MAX_VERTEX_NUM) return ERROR;
    
    G->vexnum = vexnum;
    G->arcnum = arcnum;
    
    for (int i = 0; i < vexnum; i++)
    {
        G->vexs[i] = i; 
    }
    
    for (int i = 0; i < vexnum; i++)
    {
        for (int j = 0; j < vexnum; j++)
        {
            if (i == j)
                G->arcs[i][j] = 0;
            else
                G->arcs[i][j] = INFINITY; 
        }
    }
    
    return OK;
}

// 添加顶点
int AddVertex(MGraph *G, int index, VertexType data)
{
    if (!G || index < 0 || index >= G->vexnum) return ERROR;
    G->vexs[index] = data;
    return OK;
}

// 添加边
int AddEdge(MGraph *G, int v1, int v2, WeightType weight)
{
    if (!G || v1 < 0 || v1 >= G->vexnum || v2 < 0 || v2 >= G->vexnum) 
        return ERROR;
    
    G->arcs[v1][v2] = weight;
    G->arcs[v2][v1] = weight; 
    return OK;
}

// 获取顶点索引
int GetVertexIndex(MGraph *G, VertexType v)
{
    if (!G) return -1;
    for (int i = 0; i < G->vexnum; i++)
    {
        if (G->vexs[i] == v)
            return i;
    }
    return -1;
}

// 打印图
void PrintGraph(MGraph *G)
{
    if (!G) return;
    
    printf("\n图的邻接矩阵:\n");
    printf("    ");
    for (int i = 0; i < G->vexnum; i++)
    {
        printf("%4d", i);
    }
    printf("\n");
    
    for (int i = 0; i < G->vexnum; i++)
    {
        printf("%4d", i);
        for (int j = 0; j < G->vexnum; j++)
        {
            if (G->arcs[i][j] == INFINITY)
                printf("  ∞");
            else
                printf("%4d", G->arcs[i][j]);
        }
        printf("\n");
    }
}

// 销毁图
void DestroyGraph(MGraph *G)
{
    if (G)
    {
        G->vexnum = 0;
        G->arcnum = 0;
    }
}

// 初始化并查集
int InitUnionFind(UnionFind *uf, int n)
{
    if (!uf || n <= 0 || n > MAX_VERTEX_NUM) return ERROR;
    
    for (int i = 0; i < n; i++)
    {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
    return OK;
}

// 查找根节点
int Find(UnionFind *uf, int x)
{
    if (!uf) return -1;
    
    if (uf->parent[x] != x)
    {
        uf->parent[x] = Find(uf, uf->parent[x]); 
    }
    return uf->parent[x];
}

// 合并两个集合
int Union(UnionFind *uf, int x, int y)
{
    if (!uf) return ERROR;
    
    int rootX = Find(uf, x);
    int rootY = Find(uf, y);
    
    if (rootX == rootY) return ERROR; 
    
    // 按秩合并
    if (uf->rank[rootX] < uf->rank[rootY])
    {
        uf->parent[rootX] = rootY;
    }
    else if (uf->rank[rootX] > uf->rank[rootY])
    {
        uf->parent[rootY] = rootX;
    }
    else
    {
        uf->parent[rootY] = rootX;
        uf->rank[rootX]++;
    }
    
    return OK;
}

// 边排序（冒泡排序）
void SortEdges(Edge edges[], int n)
{
    if (!edges || n <= 0) return;
    
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (edges[j].weight > edges[j + 1].weight)
            {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

// Prim算法
int Prim(MGraph *G, int start, MSTEdge mst[], int *mstCount)
{
    if (!G || !mst || !mstCount || start < 0 || start >= G->vexnum)
        return ERROR;
    
    // 标记顶点是否已加入MST
    int visited[MAX_VERTEX_NUM];
    // 记录到MST的最小距离
    WeightType lowcost[MAX_VERTEX_NUM];
    // 记录最小距离对应的顶点
    int closest[MAX_VERTEX_NUM];
    
    for (int i = 0; i < G->vexnum; i++)
    {
        visited[i] = FALSE;
        lowcost[i] = G->arcs[start][i];
        closest[i] = start;
    }
    
    visited[start] = TRUE;
    *mstCount = 0;
    WeightType totalWeight = 0;
    
    for (int i = 1; i < G->vexnum; i++)
    {
        WeightType min = INFINITY;
        int k = -1;
        
        for (int j = 0; j < G->vexnum; j++)
        {
            if (!visited[j] && lowcost[j] < min)
            {
                min = lowcost[j];
                k = j;
            }
        }
        
        if (k == -1) return ERROR; 
        
        mst[*mstCount].start = closest[k];
        mst[*mstCount].end = k;
        mst[*mstCount].weight = min;
        (*mstCount)++;
        totalWeight += min;
        
        visited[k] = TRUE;
        
        for (int j = 0; j < G->vexnum; j++)
        {
            if (!visited[j] && G->arcs[k][j] < lowcost[j])
            {
                lowcost[j] = G->arcs[k][j];
                closest[j] = k;
            }
        }
    }
    
    return OK;
}

// Kruskal算法
int Kruskal(MGraph *G, MSTEdge mst[], int *mstCount)
{
    if (!G || !mst || !mstCount) return ERROR;
    
    // 收集所有边
    Edge edges[MAX_VERTEX_NUM * MAX_VERTEX_NUM];
    int edgeCount = 0;
    
    for (int i = 0; i < G->vexnum; i++)
    {
        for (int j = i + 1; j < G->vexnum; j++)
        {
            if (G->arcs[i][j] != INFINITY && G->arcs[i][j] != 0)
            {
                edges[edgeCount].start = i;
                edges[edgeCount].end = j;
                edges[edgeCount].weight = G->arcs[i][j];
                edgeCount++;
            }
        }
    }
    
    // 对边按权值排序
    SortEdges(edges, edgeCount);

    UnionFind uf;
    if (InitUnionFind(&uf, G->vexnum) != OK) return ERROR;
    
    *mstCount = 0;
    WeightType totalWeight = 0;

    for (int i = 0; i < edgeCount && *mstCount < G->vexnum - 1; i++)
    {
        int root1 = Find(&uf, edges[i].start);
        int root2 = Find(&uf, edges[i].end);

        if (root1 != root2)
        {
            mst[*mstCount].start = edges[i].start;
            mst[*mstCount].end = edges[i].end;
            mst[*mstCount].weight = edges[i].weight;
            (*mstCount)++;
            totalWeight += edges[i].weight;
            
            Union(&uf, root1, root2);
        }
    }

    if (*mstCount < G->vexnum - 1)
    {
        return ERROR;
    }
    
    return OK;
}

