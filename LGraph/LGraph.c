#include "LGraph.h"
#include "LinkList.h"
#include "LinkQueue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _tag_LGraph
{
	int count;
	LVertex **v;
	LinkList **la;
}TLGraph;
typedef struct _tag_ListNode
{
	LinkListNode header;
	int v;
	int w;
}TListNode;

LGraph* LGraph_Create(LVertex** v, int n)
{
	TLGraph *ret = NULL;
	int ok = -1;
	if (v != NULL&&n > 0)
	{
		ret = (TLGraph *)malloc(sizeof(TLGraph));
		if (ret != NULL)
		{
			ret->count = n;
			/*calloc*/
			ret->v = (LVertex **)calloc(n, sizeof(LVertex*));
			ret->la = (LinkList**)calloc(n, sizeof(LinkList*));
			ok = (ret->v != NULL) && (ret->la != NULL);
			if (ok)
			{
				int i = 0;
				for (i = 0; i < n; i++)
				{
					ret->v[i] = v[i];
				}
				for (i = 0; i < n && ok; i++)
				{
					ok = ok&&((ret->la[i] = LinkList_Create()) != NULL);
				}
			}
			if (!ok)
			{
				if (ret->la != NULL)
				{
					int i = 0;
					for (i = 0; i < n; i++)
					{
						LinkList_Destroy(ret->la[i]);
					}
				}
				free(ret->v);
				free(ret->la);
				free(ret);
				ret = NULL;
			}
		}
	}
	return ret;
}
void LGraph_Destroy(LGraph* graph)
{
	TLGraph * tGraph = (TLGraph *)graph;
	
	if (tGraph != NULL)
	{
		int i = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			LinkList_Destroy(tGraph->la[i]);
		}
		free(tGraph->la);
		free(tGraph->v);
		free(tGraph);
		
	}
}
void LGraph_Clear(LGraph* graph)
{
	TLGraph *tGraph = (TLGraph *)graph;
	if (tGraph != NULL)
	{
		int i = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			while (LinkList_Length(tGraph->la[i]))
			{
				free(LinkList_Delete(tGraph->la[i], 0));
			}
		}
	}
}
int LGraph_AddEdge(LGraph* graph, int v1, int v2, int w)
{
	TLGraph * tGraph = (TLGraph *)graph;
	TListNode *node = NULL;
	int ret = (graph != NULL) && (0 <= v1&&v1 < tGraph->count) && (0 <= v2&&v2 < tGraph->count);
	ret = ret &&((node = (TListNode *)malloc(sizeof(TListNode))) != NULL);
	if (ret)
	{
		node->v = v2;
		node->w = w;
		LinkList_Insert(tGraph->la[v1], node, 0);
	}
	return ret;
}
int LGraph_RemoveEdge(LGraph* graph, int v1, int v2)
{
	TLGraph *tGraph = (TLGraph *)graph;
	int ok = (graph != NULL) && (0 <= v1&&v1 < tGraph->count) && (0 <= v2&&v2 < tGraph->count);
	int ret = 0;
	if (ok)
	{
		TListNode *node = NULL;
		int i = 0;
		for (i = 0; i < LinkList_Length(tGraph->la[v1]); i++)
		{
			node = (TListNode *)LinkList_Get(tGraph->la[v1], i);
			if (node->v == v2)
			{
				ret = node->w;
				LinkList_Delete(tGraph->la[v1], i);
				free(node);
				break;
			}
		}
	}
	return ret;
}
int LGraph_GetEdge(LGraph* graph, int v1, int v2)
{
	TLGraph * tGraph = (TLGraph *)graph;
	int ok = (graph != NULL) && (0 <= v1&&v1 < tGraph->count) && (0 <= v2&&v2 < tGraph->count);
	int ret = 0;
	if (ok)
	{
		int i = 0;
		TListNode * node = NULL;
		for (i = 0; i < LinkList_Length(tGraph->la[v1]); i++)
		{
			node = (TListNode *)LinkList_Get(tGraph->la[v1],i);
			if (node->v == v2)
			{
				ret = node->w;
				break;
			}
		}
	}
	return ret;
}
int LGraph_TD(LGraph* graph, int v)
{
	TLGraph *tGraph = (TLGraph *)graph;
	int ok = (graph != NULL) && (0 <= v&&v < tGraph->count);
	int ret = 0;
	if (ok)
	{
		int i = 0;
		int j = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			for (j = 0; j < LinkList_Length(tGraph->la[i]); j++)
			{
				TListNode * node = LinkList_Get(tGraph->la[i], j);
				if (node->v == v)
				{
					ret++;
				}
			}
		}
		ret += LinkList_Length(tGraph->la[v]);
	}
	return ret;
}
int LGraph_VertexCount(LGraph* graph)
{
	TLGraph *tGraph = (TLGraph *)graph;
	int ret = 0;
	if (tGraph != NULL)
	{
		ret = tGraph->count;
	}
	return ret;
}
int LGraph_EdgeCount(LGraph* graph)
{
	TLGraph *tGraph = (TLGraph *)graph;
	int ret = 0;
	if (tGraph != NULL)
	{
		int i = 0;
		for (i = 0; i < tGraph->count; i++)
		{
			ret += LinkList_Length(tGraph->la[i]);
		}
	}
	return ret;
}
static void recursive_dfs(TLGraph *tGraph, int v, int visited[], LGraph_Printf *pFunc)
{
	int i = 0;
	pFunc(tGraph->v[v]);
	visited[v] = 1;
	printf(",");
	for (i = 0; i < LinkList_Length(tGraph->la[v]); i++)
	{
		TListNode *node = (TListNode *)LinkList_Get(tGraph->la[v], i);
		if (!visited[node->v])
		{
			recursive_dfs(tGraph, v, visited, pFunc);
		}
	}
}
static void bfs(TLGraph *tGraph, int v, int visited[], LGraph_Printf *pFunc)
{
	LinkQueue *queue = LinkQueue_Create();
	if (queue != NULL)
	{
		LinkQueue_Append(queue, tGraph->v[v]);
		visited[v] = 1;
		while (LinkQueue_Length(queue))
		{
			int i = 0;
			v = (LVertex *)LinkQueue_Retrieve(queue);
			pFunc(tGraph->v[v]);
			printf(",");
			for (i = 0; i < LinkList_Length(tGraph->la[v]); i++)
			{
				TListNode *node = (TListNode *)LinkList_Get(tGraph->la[v], i);
				if (!visited[node->v])
				{
					LinkQueue_Append(queue, tGraph->v[node->v]);
					visited[node->v] = 1;
				}
			}

		}
	}
}
void LGraph_DFS(LGraph* graph, int v, LGraph_Printf* pFunc);
void LGraph_BFS(LGraph* graph, int v, LGraph_Printf* pFunc);
void LGraph_Display(LGraph* graph, LGraph_Printf* pFunc);