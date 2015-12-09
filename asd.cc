/**
@file asd.cc

Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <algorithm>

#include <iostream>
#include <sstream>

#ifdef _SUNOS
#include "/materialy/AISDI/tree/TreeMap.h"
#else
#include "TreeMap.h"
#endif

/// A helper class.
class TreeMapDetail {
protected:
        friend class TreeMap;
        typedef TreeNode node;
        static TreeNode* end_guard;

        unsigned int tree_size = 0;

        unsigned size() const {
                return this->tree_size;
        }
        unsigned size(int diff) {
                this->tree_size = diff;
                return this->tree_size;
                return this->tree_size += diff;
        }
        #define bf b
        #define l left
        #define r right
        #define p parent
        static void ll_rotate(node* origin, node** root) {
                node* n = origin;
                node* ln = n->l;
                node* lrn = ln->r;

                if(*root == n)
                        *root = ln;
                else if(origin->p->l == origin)
                        origin->p->l = ln;
                else
                        origin->p->r = ln;

                n->l = lrn;
                ln->r = n;

                ln->p = n->p;
                n->p = ln;
                if(lrn)
                        lrn->p = n;

                if(ln->bf == 1)
                        ln->bf = n->bf = 0;
                else
                        ln->bf = -1, n->bf =1;
        }

        static void rr_rotate(node* origin, node** root) {
                node* n = origin;
                node* rn = n->r;
                node* rln = rn->l;

                if(*root == n)
                        *root = rn;
                else if(origin->p->l == origin)
                        origin->p->l = rn;
                else
                        origin->p->r = rn;

                n->r = rln;
                rn->l = n;

                rn->p = n->p;
                n->p = rn;
                if(rln)
                        rln->p = n;

                if(rn->bf == -1)
                        n->bf = rn->bf = 0;
                else
                        rn->bf = 1, n->bf = -1;
        }

        static void rl_rotate(node* origin, node** root) {
                node* n    = origin;
                node* rn   = n->r;
                node* rln  = rn->l;
                node* rlln = rln->l;
                node* rlrn = rln->r;

                if(*root == n)
                        *root = rln;
                else if(origin->p->l == origin)
                        origin->p->l = rln;
                else
                        origin->p->r = rln;

                rln->l = n;
                rln->r = rn;
                n->r = rlln;
                rn->l = rlrn;

                rln->p = n->p;
                n->p = rln;
                rn->p = rln;
                if(rlln)
                        rlln->p = n;
                if(rlrn)
                        rlrn->p = rn;

                if(rln->bf == -1)
                        n->bf = 1;
                else
                        n->bf = 0;

                if(rln->bf == 1)
                        rn->bf = -1;
                else
                        rn->bf = 0;
                rln->bf = 0;
        }

        static void lr_rotate(node* origin, node** root) {
                node* n    = origin;
                node* ln   = n->l;
                node* lrn  = ln->r;
                node* lrln = ln->r;
                node* lrrn = lrn->r;

                if(*root == n)
                        *root = lrn;
                else if(origin->p->l == origin)
                        origin->p->l = lrn;
                else
                        origin->p->r = lrn;

                lrn->l = ln;
                lrn->r = n;
                ln->r = lrln;
                n->l = lrrn;

                lrn->p = n->p;
                ln->p = lrn;
                n->p = lrn;
                if(lrln)
                        lrln->p = ln;
                if(lrrn)
                        lrrn->p = n;

                if(lrn->bf == 1)
                        n->bf = -1;
                else
                        n->bf = 0;
                if(lrn->bf == -1)
                        ln->bf = 1;
                else
                        ln->bf = 0;
                lrn->bf = 0;
        }

        static node* delete_node(node* n, node** root)  {
                node* np = nullptr;
                bool nested;

                if(n->l && n->r) {
                        np = delete_node(prev(n), root);
                        nested = false;
                } else {
                        if(n->l) {
                                np = n->l;
                                n->l = nullptr;
                        } else {
                                np = n->r;
                                n->r = nullptr;
                        }
                        n->bf = 0;
                        nested = true;
                }

                if(np) {
                        np->r = n->r;
                        np->l = n->l;
                        if(np->l)
                                np->l->p = np;
                        np->p = n->p;
                        if(np->r)
                                np->r->p = np;
                        np->bf = n->bf;
                }

                if(n->p) {
                        if(n->p->l == n)
                                n->p->l = np;
                        else
                                n->p->r = np;
                } else {
                        *root = np;
                }

                if(nested) {
                        node* ns;
                        node* nr = np;

                        np = n->p;

                        while(np) {
                                if(!np->bf) {
                                        if(np->l == nr)
                                                np->bf = -1;
                                        else
                                                np->bf = 1;
                                        break;
                                } else {
                                        if(((np->bf == 1) && (np->l == nr)) ||
                                                ((np->bf == -1) && (np->r == nr))) {
                                                np->bf = 0;
                                                nr = np;
                                                np = np->p;
                                        } else {
                                                ns = np->l == nr ? np->r : np->l;
                                                if(!ns->bf) {
                                                        if(np->bf == 1)
                                                                ll_rotate(np, root);
                                                        else
                                                                rr_rotate(np, root);
                                                        break;
                                                } else if(ns->bf == np->bf) {
                                                        if(np->bf == 1)
                                                                ll_rotate(np, root);
                                                        else
                                                                rr_rotate(np, root);
                                                        nr = ns;
                                                        np = ns->p;
                                                } else {
                                                        if(np->bf == 1)
                                                                lr_rotate(np, root);
                                                        else
                                                                rl_rotate(np, root);
                                                        nr = np->p;
                                                        np = nr->p;
                                                }
                                        }
                                }
                        }
                }
                return n;
        }

        static node* next(node* n) {
                if(n->r) {
                        n = n->r;
                        while(n->l)
                                n = n->l;
                } else {
                        while(!(n->p) || n->p->l != n) {
                                if(!(n = n->p))
                                        return nullptr;
                        }
                        n = n->p;
                }
                return n;
        }

        static node* prev(node* n) {
                if(n->l) {
                        n = n->l;
                        while(n->r)
                                n = n->r;
                } else {
                        while(!(n->l) || n->l->p != n) {
                                if(!(n = n->l))
                                        return nullptr;
                        }
                        n = n->p;
                }
                return n;
        }

        static void insert(node* n, node* at, node** root) {
                if(n->data.first < at->data.first) {
                        //left
                        if(at->l)
                                insert(n, at->l, root);
                        else {
                                at->l = n;
                                n->p = at;
                                if(at->bf)
                                        at->bf = 0;
                                else {
                                        at->bf = 1;
                                        rebalance_rest(at, root);
                                }
                        }
                } else {
                        //right
                        if(at->r)
                                insert(n, at->r, root);
                        else {
                                at->r = n;
                                n->p = at;
                                if(at->bf)
                                        at->bf = 0;
                                else {
                                        at->bf = -1;
                                        rebalance_rest(at, root);
                                }
                        }
                }
        }

        static void rebalance_rest(node* n, node** root) {
                if(n->p && n->p->bf) {
                        if(n->p->bf == 1) {
                                if(n->p->r == n) {
                                        n->p->bf = 0;
                                } else {
                                        if(n->bf == -1)
                                                lr_rotate(n->p, root);
                                        else
                                                ll_rotate(n->p, root);
                                }
                        } else {
                                if(n->p->l == n) {
                                        n->p->bf = 0;
                                } else {
                                        if(n->bf == 1)
                                                rl_rotate(n->p, root);
                                        else
                                                rr_rotate(n->p, root);
                                }
                        }
                } else if(n->p) {
                        n->p->bf = n->p->l == n ? 1 : -1;
                        rebalance_rest(n->p, root);
                }
        }

        static bool subtree_eq(node* lhs, node* rhs) {
                if(lhs == nullptr && rhs == nullptr)
                        return true;
                if(lhs == nullptr || rhs == nullptr)
                        return false;

                if(lhs->data != rhs->data)
                        return false;
                if(lhs->bf != rhs->bf)
                        return false;

                if(     !subtree_eq(lhs->l, rhs->l) ||
                        !subtree_eq(lhs->r, rhs->r))
                        return false;

                return true;
        }

        static void print_subtree(std::ostringstream& out, node* n, unsigned depth) {
                if(n->l)
                        print_subtree(out, n->l, depth+1);
                out << std::string(depth, '\t') << n->data.first << " : " << n->data.second << "\n";
                if(n->r)
                        print_subtree(out, n->r, depth+1);
        }

        static bool sanity_check(TreeMap& t) {
                auto current = t.begin();
                for(auto prev = current++; current != t.end() ;prev++, current++)
                        if(prev->first >= current->first)
                                return false;
                return true;
        }
public:
        static std::string print_tree(TreeMap& t) {
                std::ostringstream oss;
                TreeMapDetail::print_subtree(oss, t.root, 0);
                return oss.str();
        }


        #undef bf
        #undef l
        #undef r
        #undef p
};

std::string print_tree(TreeMap& t) {
        return TreeMapDetail::print_tree(t);
}

TreeNode* TreeMapDetail::end_guard = new TreeNode(TreeNode::T{});

//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

TreeMap::TreeMap() {
        this->root = nullptr;
        this->detail = new TreeMapDetail;
};

/// Content of existing TreeMap object is copied into the new object.
TreeMap::TreeMap( const TreeMap& m ) {
        //TODO: just do it
};

TreeMap::~TreeMap() {
        clear();
        delete this->detail;
};

// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//          made and false if the map already contained an element
//          associated with that key, and whose iterator component coresponds to
//          the address where a new element was inserted or where the element
//          was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry) {
        iterator it = this->find(entry.first);
        bool inserted = false;
        if(it == this->end()) {
                it = this->unsafe_insert(entry);
                inserted = true;
        }
        return std::make_pair(it, inserted);
}

// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.

TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry) {
        detail->size(+1);
        Node* n = new Node(entry);
        if(this->empty())
                this->root = n;
        else
                detail->insert(n, this->root, &this->root);
        return iterator(n);
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k) {
        // TODO: shortcircuting if k between current and child
        Node* current = this->root;
        while(true) {
                if(current == nullptr)
                        return this->end();
                else if(current->data.first == k)
                        return iterator(current);
                else if(current->data.first > k)
                        current = current->left;
                else
                        current = current->right;
        }
}

TreeMap::const_iterator TreeMap::find(const Key& k) const {
        return const_cast<TreeMap*>(this)->find(k);
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k) {
        iterator it = this->find(k);
        if(it == this->end())
                it = this->unsafe_insert(std::make_pair(k, std::string{}));

        return it->second;
}

// Tests if a map is empty.
bool TreeMap::empty( ) const {
        return root == nullptr;
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size() const {
        return this->detail->size();
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const {
        return this->find(_Key) == this->end() ? 0 : 1;
}

// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i) {
        Node* next = this->detail->next(i.node);
        this->detail->size(-1);
        this->detail->delete_node(i.node, &this->root);
        std::cout << i.node->data.first << std::endl;
        delete i.node;
        return iterator(next);
}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l) {
        while(f != l) {
                std::cout << "e" << f.node->data.first << std::endl;
                this->erase(f++);
        }
        return l;
}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//          Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key) {
        iterator it = this->find(key);
        if(it == this->end())
                return 0;
        this->erase(it);
        return 0;
}

// Erases all the elements of a map.
void TreeMap::clear() {
        //TODO: make it not suck
        if(!this->empty())
                this->erase(this->begin(), this->end());
}

bool TreeMap::struct_eq(const TreeMap& another) const {
        return this->detail->subtree_eq(this->root, another.root);
}

bool TreeMap::info_eq(const TreeMap& another) const {
        //TODO: maybe it's just struct equality?
   assert(0);
   return false;
}

// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator++() {
        std::cout << "next" << std::endl;
        if(this->node == TreeMapDetail::end_guard)
                throw std::out_of_range("already at end");

        Node* next = TreeMapDetail::next(this->node);
        if(next == nullptr)
                next = TreeMapDetail::end_guard;
        if(this->node == next)
                std::cout << "wtf";

        std::cout << this->node->data.first << ", " << next->data.first << std::endl;

        this->node = next;
        return *this;
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int) {
        const_iterator old(*this);
        operator++();
        return old;
}

// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--() {
        Node* prev = TreeMapDetail::prev(this->node);
        if(prev == nullptr)
                throw std::out_of_range("already at begin");
        this->node = prev;
        return *this;
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int) {
        const_iterator old(*this);
        operator--();
        return old;
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& other) {
        //TODO: well, implementing
   return *this;
}

/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin() {
        //TODO: some caching maybe?
        Node* n = this->root;
        while(n->left)
                n = n->left;
        return const_iterator(n);
}

TreeMap::const_iterator TreeMap::begin() const {
        //TODO: some caching maybe?
        //TODO: less <C-p><C-v>
        Node* n = this->root;
        while(n->left)
                n = n->left;
        return const_iterator(n);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end() {
        return iterator(this->detail->end_guard);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const {
        return const_iterator(this->detail->end_guard);
}

//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

/// A helper function that outputs a key-value pair.
void print(const std::pair<int, std::string>& p) {
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// The big mean test function ;)
void test() {
   // A typedef used by the test.
   typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   //typedef TreeMap TEST_MAP;

   std::cout << "Testy uzytkownika" << std::endl;

        TEST_MAP m;

   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[4] = "Magdalena";

   for_each(m.begin(), m.end(), print );
   //system("PAUSE");

}
