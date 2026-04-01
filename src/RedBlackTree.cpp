#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:
    if(!node) {
        return;
    }

    clear(node->left);
    clear(node->right);
    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name

    //node does not exist
    if (!node) {
        return nullptr;
    }

    //found the node
    if (fullName == node->data.fullName) {
        return node;
    }
    //if smaller
    else if (fullName < node->data.fullName) {
        return find(node->left, fullName);
    }
    //if greater
    else {
        return find(node->right, fullName);
    }
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise

    // Check if researcher already exists
    if (findResearcher(researcher.fullName)) {
        return false;
    }

    ResearcherNode* newNode = new ResearcherNode(researcher);

    ResearcherNode* current = root;
    ResearcherNode* parent = nullptr;

    // Standard BST insertion
    while (current) {
        parent = current;
        if (newNode->data.fullName < current->data.fullName) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    newNode->parent = parent;
    
    if (!parent) {
        root = newNode;
    } else if (newNode->data.fullName < parent->data.fullName) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    
    // Fix RBT properties
    insertFixup(newNode);
    return true;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).
    if (!current) {
        inserted = true;
        return node;
    }

    if (node->data.fullName == current->data.fullName) {
        inserted = false;
        return current;
    }
    else if (node->data.fullName < current->data.fullName) {
        current->left = bstInsert(current->left, node, inserted);
        if (inserted) {
            current->left->parent =current;
        }
    }
    else {
        current->right = bstInsert(current->right, node, inserted);
        if (inserted) {
            current->right->parent = current;
        }
    }

    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.
    while (node->parent && node->parent->color == RED) {

        ResearcherNode *parent = node->parent;
        ResearcherNode *grand = parent->parent;

        if (!grand) break;  // Safety check

        if (parent == grand->left) {
            ResearcherNode *aunt = grand->right;

            // Case 1: aunt is RED
            if (aunt && aunt->color == RED) {
                parent->color = BLACK;
                aunt->color = BLACK;
                grand->color = RED;
                node =grand;
            }
            else {
                // Case 2: Node is right child (Left-Right case)
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    if (!parent) break;
                    grand = parent->parent;
                    if (!grand) break;
                }
                // Case 3: Node is left child (Left-Left case)
                parent->color = BLACK;
                grand->color = RED;
                rotateRight(grand);
            }
        }
        else {
            ResearcherNode *aunt = grand->left;

            // Case 1: aunt is RED
            if (aunt && aunt->color == RED) {
                parent->color = BLACK;
                aunt->color = BLACK;
                grand->color = RED;
                node =grand;
            }
            else {
                // Case 2: Node is left child (Right-Left case)
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    if (!parent) break;
                    grand = parent->parent;
                    if (!grand) break;
                }
                // Case 3: Node is right child (Right-Right case)
                parent->color = BLACK;
                grand->color = RED;
                rotateLeft(grand);
            }
        }
    }
    root->color = BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.

    ResearcherNode *z=findResearcher(fullName);
    if (z==nullptr){
        return false;
    }
    ResearcherNode *y=z;
    ResearcherNode *x=nullptr;
    Color originalColor=y->color;
    
    //BST delete logic: handle cases with 0, 1, or 2 children
    if(!z->left){
        x=z->right;
        // Replacement: replace z with its right child
        if (!z->parent){
            root=x;
        }else if(z==z->parent->left){
            z->parent->left=x;
        }else{
            z->parent->right=x;
        }

        if (x!=nullptr){
            x->parent=z->parent;
        }
   }else if (z->right==nullptr){
        x=z->left;
        if (z->parent==nullptr){
            root=x;
        }else if (z==z->parent->left){
            z->parent->left=x;
        }else{
            z->parent->right=x;
        }
        
        if (x!=nullptr){
            x->parent=z->parent;
        }
   }else{
    //node has 2 children: find the successor -> minimum of right subtree
        y=minimum(z->right);
        originalColor=y->color;
        x=y->right;
        if (y->parent==nullptr){
            root=y->right;
        }else if (y==y->parent->left){
            y->parent->left=y->right;
        }else{
            y->parent->right=y->right;
        }

        if (y->right !=nullptr){
            y->right->parent=y->parent;
        }
        y->right=z->right;
        if (y->right !=nullptr){
            y->right->parent=y;
        }
        
        if (z->parent==nullptr){
            root=y;
        }else if (z==z->parent->left){
            z->parent->left=y;
        }else{
            z->parent->right=y;
        }

        if(y!=nullptr){
            y->parent=z->parent;
        }
        y->left=z->left;
        if(y->left!=nullptr){
            y->left->parent=y;
        }
        y->color=z->color;
   }
   //if the removed node was black we might have double black
   if (originalColor==BLACK && x!=nullptr){
    //handle the case where x might be nullptr
        ResearcherNode *xParent= (x!=nullptr) ? x->parent : (z->parent!=nullptr) ? z->parent : nullptr;
        deleteFixup(x,xParent);
   }

   //safely delete the memory
   delete z;
   return true;
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    // TODO: Standard Red-Black deletion fixup.
    if (!parent) return; //burayı ekledim

    //fix the tree balance while we have a double-black node
    while (node != root && (!node || node->color == BLACK)) {
        if (!parent) break;

        if (node == parent->left) {
            ResearcherNode *x = parent->right;
            // Case 1: Sibling is RED
            if (x && x->color == RED) {
                x->color = BLACK;
                parent->color = RED;
                rotateLeft(parent);
                x = parent->right;
            }

            // Case 2: Sibling is BLACK with two BLACK children
            if (x && (!x->left || x->left->color == BLACK) && (!x->right || x->right->color == BLACK)) {
                if (x) {
                    x->color = RED;
                }
                node =parent;
                parent = node->parent;
            }
            else if (x) {
                // Case 3: Sibling is BLACK, sibling's right child is BLACK
                if (!x->right || x->right->color == BLACK) {
                    if (x->left) {
                        x->left->color = BLACK;
                    }
                    x->color = RED;
                    rotateRight(x);
                    x = parent->right;
                }

                // Case 4: Sibling is BLACK, sibling's right child is RED
                if(x) {
                    x->color = parent->color;
                    parent->color = BLACK;
                    if (x->right) {
                        x->right->color = BLACK;
                    }
                    rotateLeft(parent);
                }

                node = root; 
            }
            else {
                break;
            }
        }
        else {
            ResearcherNode *x = parent->left;
            // Case 1: Sibling is RED
            if (x && x->color == RED) {
                x->color = BLACK;
                parent->color = RED;
                rotateRight(parent);
                x = parent->left;
            }

            // Case 2: Sibling is BLACK with two BLACK children
            if (x && (!x->left || x->left->color == BLACK) && (!x->right || x->right->color == BLACK)) {
                x->color = RED;
                node =parent;
                parent = node->parent;
            }
            else if (x) {
                // Case 3: Sibling is BLACK, sibling's left child is BLACK
                if (x && (!x->left || x->left->color == BLACK)) {
                    if (x->right) {
                        x->right->color = BLACK;
                    }
                    x->color = RED;
                    rotateLeft(x);
                    x = parent->left;
                }

                // Case 4: Sibling is BLACK, sibling's left child is RED
                if (x) {
                    x->color = parent->color;
                    parent->color = BLACK;
                    if (x->left) {
                        x->left->color =BLACK;
                    }
                    rotateRight(parent);
                }

                node= root;

            }
            else {
                break;
            }
        }
    }

    if (node) {
        node->color = BLACK;
    }
    
}

ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.

    //keep going left to find the smallest value in the subtree
    while (node->left) {
        node = node->left;
    }
    return node;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.

    //move x's right child up to be the new root of this subtree
    ResearcherNode *k = x->right;
    x->right = k->left;

    if (k->left) {
        k->left->parent = x;
    }

    k->parent = x->parent;

    if (!x->parent) {
        root = k;
    }
    else if (x == x->parent->left) {
        x->parent->left = k;
    }
    else {
        x->parent->right = k;
    }

    k->left = x;
    x->parent = k;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.

    //move y's left child up to be the new root of this subtree
    ResearcherNode *m = y->left;
    y->left = m->right;

    if (m->right) {
        m->right->parent = y;
    }

    m->parent = y->parent;

    if (!y->parent) {
        root = m;
    }
    else if (y == y->parent->left) {
        y->parent->left = m;
    }
    else {
        y->parent->right = m;
    }

    m->left = y;
    y->parent = m;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    if (!node) {
        return 0;
    }

    return 1 + getResearcherCount(node->left) +getResearcherCount(node->right);
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    if (!node) {
        return 0;
    }

    return node->data.numAssigned + getTotalLoad(node->right) + getTotalLoad(node->left);
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.
    if (!node) {
        return;
    }

    std::cout << node->data.fullName << " " << node->data.capacity << " " << node->data.numAssigned << std::endl;

    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);
}
