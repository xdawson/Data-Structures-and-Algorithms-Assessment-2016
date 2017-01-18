#include "Trie.h"
#include <malloc.h>


trie_node *
TrieNodeConstructor(char Value)
{
	trie_node *Node = malloc(sizeof(trie_node));

	Node->Child = NULL;
	Node->Sibling = NULL;

	Node->Value = Value;
	Node->WordMarker = 0;

	return Node;
}

//Seems like I should some sort of non-implementation dependant name for these
//to go with the whole data hiding ideology
trie_root_node *
TrieRootNodeConstructor()
{
	//get memory for Trie root node
	trie_root_node *Root = malloc(sizeof(trie_root_node));

	Root->FirstLetter = TrieNodeConstructor(0);

	return Root;
}

//TODO destructors
//TODO insert(maybe) and search functions