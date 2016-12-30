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
};

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
			}
		}


	}

}
void LGraph_Destroy(LGraph* graph);
void LGraph_Clear(LGraph* graph);
int LGraph_AddEdge(LGraph* graph, int v1, int v2, int w);
int LGraph_RemoveEdge(LGraph* graph, int v1, int v2);
int LGraph_GetEdge(LGraph* graph, int v1, int v2);
int LGraph_TD(LGraph* graph, int v);
int LGraph_VertexCount(LGraph* graph);
int LGraph_EdgeCount(LGraph* graph);
void LGraph_DFS(LGraph* graph, int v, LGraph_Printf* pFunc);
void LGraph_BFS(LGraph* graph, int v, LGraph_Printf* pFunc);
void LGraph_Display(LGraph* graph, LGraph_Printf* pFunc);