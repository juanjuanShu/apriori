#include <unordered_map>

using namespace std;
using ItemSet = vector<string>;

struct TrieNode {
    //Hashmap 的键是字符，值是相对应的子节点
    //比使用数组稍微慢一些 但是只存储需要的节点 因此更灵活 也节省空间
    unordered_map<char, TrieNode*> next;
};

//insert, search, 和 startsWith

class Trie {
public:
    /** Initialize your data structure here. */
    Trie() {
        root = new TrieNode();
    }

    /** Inserts a word into the trie. */
    void insert(const ItemSet &item) {
        TrieNode* p = root;
       /* if ((p->next).count(item[0]) <= 0) {

        }*/


        //TrieNode* p = root;
        //

        //for (int i = 0; i < word.length(); ++i) {
        //    if ((p->next).count(word[i]) <= 0) {
        //        // insert a new node if the path does not exist
        //        (p->next).insert(make_pair(word[i], new TrieNode()));
        //    }
        //    p = (p->next)[word[i]];
        //}
       
    }

    /** Returns if the word is in the trie. */
    bool search(string word) {

    }

    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {

    }

private:
    TrieNode* root;
    /*unordered_map<> _hashTable;*/
};
