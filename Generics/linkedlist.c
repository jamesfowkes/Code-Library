#include <stdbool.h>
#include <stddef.h>

#include "linkedlist.h"

bool LList_Init(LINK_NODE * listhead)
{
	
	listhead->prev = NULL;
	listhead->next = NULL;
	
	return true;
}

LINK_NODE * LList_Tail(LINK_NODE * node)
{
	
	if (node != NULL)
	{
		while (node->next != NULL)
		{
			node = node->next;
		}
	}
	
	return node;
}

bool LList_Add(LINK_NODE * node, LINK_NODE * newNode)
{
	
	LINK_NODE * oldTail = LList_Tail(node);
	
	if (node != NULL)
	{
		newNode->prev = oldTail;
		oldTail->next = newNode;
		newNode->next = NULL;
	}
	
	return true;
}

bool LList_Traverse(LINK_NODE * node, llink_cb callback)
{
	bool stop = false;
	
	while (node && !stop)
	{
		stop = callback(node);
		node = node->next;
	}

	return true;
}
