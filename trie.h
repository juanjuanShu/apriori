#include <unordered_map>

using namespace std;
using ItemSet = vector<string>;

struct TrieNode {
    //Hashmap 的键是字符，值是相对应的子节点
    //比使用数组稍微慢一些 但是只存储需要的节点 因此更灵活 也节省空间
    unordered_map<string, TrieNode*> next;
};

class Trie {
public:
   //构造函数
    Trie() {
        root = new TrieNode();
    }

    /** Inserts a word into the trie. */
    void insert(const ItemSet& item) {
        TrieNode* p = root;
        for (int i = 0; i < item.size(); ++i) {
            if ((p->next).count(item[i]) <= 0) {
                // insert a new node if the path does not exist
                (p->next).insert(make_pair(item[i], new TrieNode()));
            }
            p = (p->next)[item[i]];
        }
    }

    
    /** Returns if the word is in the trie. */
    bool search(const ItemSet& item) {
        TrieNode* p = root;
        for (int i = 0; i < item.size(); ++i) {
            if ((p->next).count(item[i]) <= 0) {
                return false;
            }
            p = (p->next)[item[i]];
        }
        return true;
    }

    /** Returns if the key is in the trie. */
    bool search(const string key) {
        return next.count(key);
    }

    TrieNode*  nextPtr(const string key)
    {
        if (!next.count(key))
            return nullptr;
        return next[key];
    }

    /*析构函数*/
    ~Trie() {
        delete root;
    }
private:
    TrieNode* root;
    unordered_map<string, TrieNode*> next;
};

