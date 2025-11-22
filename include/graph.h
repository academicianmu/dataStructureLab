#ifndef GRAPH_H
#define GRAPH_H

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

#define MAX_VERTEX_NUM 50
#ifndef INFINITY
#define INFINITY 999999  // 表示无穷大（不连通）
#endif

// 顶点类型
typedef int VertexType;

// 边的权值类型
typedef int WeightType;

// 图结构（邻接矩阵）
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];           // 顶点数组
    WeightType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];  // 邻接矩阵（权值）
    int vexnum;                                 // 顶点数
    int arcnum;                                 // 边数
} MGraph;

// 边结构（用于Kruskal算法）
typedef struct {
    int start;      // 起点
    int end;        // 终点
    WeightType weight;  // 权值
} Edge;

// 并查集结构（用于Kruskal算法）
typedef struct {
    int parent[MAX_VERTEX_NUM];
    int rank[MAX_VERTEX_NUM];
} UnionFind;

// 最小生成树边结构
typedef struct {
    int start;
    int end;
    WeightType weight;
} MSTEdge;

// 图的基本操作
int CreateGraph(MGraph *G, int vexnum, int arcnum);
int AddVertex(MGraph *G, int index, VertexType data);
int AddEdge(MGraph *G, int v1, int v2, WeightType weight);
int GetVertexIndex(MGraph *G, VertexType v);
void PrintGraph(MGraph *G);
void DestroyGraph(MGraph *G);

// Prim算法
int Prim(MGraph *G, int start, MSTEdge mst[], int *mstCount);

// Kruskal算法
int Kruskal(MGraph *G, MSTEdge mst[], int *mstCount);

// 并查集操作（用于Kruskal算法）
int InitUnionFind(UnionFind *uf, int n);
int Find(UnionFind *uf, int x);
int Union(UnionFind *uf, int x, int y);

// 边排序（用于Kruskal算法，使用简单的冒泡排序）
void SortEdges(Edge edges[], int n);

#endif

