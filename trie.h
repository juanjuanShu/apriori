#include <unordered_map>

using namespace std;
using ItemSet = vector<string>;

struct TrieNode {
    //Hashmap �ļ����ַ���ֵ�����Ӧ���ӽڵ�
    //��ʹ��������΢��һЩ ����ֻ�洢��Ҫ�Ľڵ� ��˸���� Ҳ��ʡ�ռ�
    unordered_map<string, TrieNode*> next;
};

class Trie {
public:
   //���캯��
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

    /*��������*/
    ~Trie() {
        delete root;
    }
private:
    TrieNode* root;
    unordered_map<string, TrieNode*> next;
};

