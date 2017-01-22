typedef struct trie_node
{
	struct trie_node *Child;
	struct trie_node *Sibling;
	char Value;
	int WordMarker;
}trie_node;

typedef struct trie_root_node
{
	//52 Letters to allow for capitals too
	//Array of pointers
	trie_node *FirstLetter;
}trie_root_node;

trie_root_node *
TrieRootNodeConstructor();

trie_node *
TrieNodeConstructor(char Value);

trie_node *
MoveToChild(trie_node *CurrentNode);

trie_node *
FindNextNode(trie_root_node *Root, trie_node *CurrentNode, char *TextFile, int Position);

trie_root_node *
CreateTrie(char *TextFile);

trie_node *
FindPrefixNode(trie_root_node *Root, char *Prefixbuffer);

int
NumberOfSiblings(trie_node *CurrentNode);