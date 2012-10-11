#include <stdbool.h>

#include "linkedlist.h"

bool LList_Init(LINK_NODE * listhead)
{
	
	bool success = False;
	
	listhead->prev = Null;
	listhead->next = Null;
}

LINK_NODE * LList_Tail(LINK_NODE * node)
{
	
	if (node != Null)
	{
		while (node->next != Null)
		{
			node = node->next;
		}
	}
	
	return node;
}

bool LList_Add(LINK_NODE * node, LINK_NODE * newNode)
{
	
	LINK_NODE * oldTail = LL_Tail(node);
	
	if (node != NULL)
	{
		newNode->prev = oldTail;
		oldTail->Next = newNode;
		newNode->Next = Null;
	}
	
	return True;
}

bool LList_Traverse(LINK_NODE * node, llink_cb callback)
{
	bool stop = False;
	
	while (node && !stop)
	{
		stop = callback(node);
		node = node->next;
	}
}
