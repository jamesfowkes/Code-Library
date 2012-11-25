#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

/*
 * Public Datatypes
 */
struct link_node
{
	void * prev;
	void * next;
};
typedef struct link_node LINK_NODE;

typedef bool(*llink_cb)(LINK_NODE*);

/*
 * Public Function Definitions
 */
bool LList_Init(LINK_NODE * listhead);
uint8_t LList_ItemCount(LINK_NODE * listhead);
LINK_NODE * LList_Tail(LINK_NODE * node);
bool LList_Add(LINK_NODE * node, LINK_NODE * newNode);
LINK_NODE * LList_Traverse(LINK_NODE * node, llink_cb callback);

#endif
