#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

bool LList_Remove(LINK_NODE * Head, LINK_NODE * node)
{

	bool success = true;
	if (node != NULL)
	{
		if (node->prev && node->next)
		{
			// Middle of list
			((LINK_NODE *)node->prev)->next = node->next;
			((LINK_NODE *)node->next)->prev = node->prev;
		}
		else if (node->prev)
		{
			// End of list
			((LINK_NODE *)node->prev)->next = NULL;
		}
		else if (node->next)
		{
			// Start of list
			Head->next = node->next;
			((LINK_NODE *)node->next)->prev = NULL;
		}
		else
		{
			success = false;
		}
	}
	else
	{
		success = false;
	}
	
	return true;
	
}

uint8_t LList_ItemCount(LINK_NODE * listhead)
{

	uint8_t count = 0;
	LINK_NODE * node = listhead;
	if (node != NULL)
	{
		while (node->next != NULL)
		{
			node = node->next;
			count++;
		}
	}

	return count;
}

LINK_NODE * LList_Traverse(LINK_NODE * head, llink_cb callback)
{
	bool stop = false;
	
	LINK_NODE * node = head->next;
	
	while (node && !stop)
	{
		stop = callback(node);
		if (!stop)
		{
			node = node->next;
		}
	}

	return node;
}
