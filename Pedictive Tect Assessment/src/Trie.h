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