#include "AVLTree.h"

AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
//TODO

    //delete all nodes to prevent memory leaks
    if (!node) {
        return;
    }

    clear(node->left);
    clear(node->right);
    delete node;
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root
    //return x;
    
    //standard AVL left rotation
    ArtifactNode *r = x->right;
    ArtifactNode *l = r->left;

    // Perform the rotation
    r->left = x;
    x->right = l;

    //update heights
    x->height = 1 + std::max(height(x->left), height(x->right));
    r->height = 1 + std::max(height(r->left), height(r->right));

    return r;
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).

    //standard AVL right rotation
    ArtifactNode *l = y->left;
    ArtifactNode *r = l->right;

    // Perform the rotation
    l->right = y;
    y->left = r;

    //update heights
    y->height = 1 + std::max(height(y->left), height(y->right));
    l->height = 1 + std::max(height(l->left), height(l->right));

    return l;
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.

    //find the node with the smallest ID in a subtree
    while(node && node->left) {
        node = node->left;
    }
    return node;
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.
    if (!node) {
        inserted = true;
        return new ArtifactNode(artifact);
    }

    if (artifact.artifactID < node->data.artifactID) {
        node->left = insert(node->left, artifact, inserted);
    }
    else if (artifact.artifactID > node->data.artifactID) {
        node->right = insert(node->right, artifact, inserted);
    }
    else {
        inserted = false; //duplicate id found
        return node;
    }

    //update height of the current ancestor node
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    //LL
    if (balance > 1 && artifact.artifactID < node->left->data.artifactID) {
        return rotateRight(node);
    }

    //RR
    if (balance < -1 && artifact.artifactID > node->right->data.artifactID) {
        return rotateLeft(node);
    }

    //LR
    if (balance > 1 && artifact.artifactID > node->left->data.artifactID) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    //RL
    if (balance < -1 && artifact.artifactID < node->right->data.artifactID) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.
    if (!node) {
        return node;
    }

    if (artifactID < node->data.artifactID) {
        node->left = remove(node->left, artifactID, removed);
    }
    else if (artifactID > node->data.artifactID) {
        node->right = remove(node->right, artifactID, removed);
    }
    else {
        removed = true;

        //node found -> handle 0, 1, or 2 children
        if (!node->left || !node->right) {
            ArtifactNode *child = node->left ? node->left : node->right;
            delete node;
            return child;
        }
        else {
            //node with 2 children
            ArtifactNode *t = findMinNode(node->right);
            node->data = t->data;
            node->right = remove(node->right, t->data.artifactID, removed);
        }
    }

    if (!node) {
        return node;
    }

    //node with 2 children
    node->height = 1 + std::max(height(node->left), height(node->right));

    //rebalance the tree after deletion
    int balance = getBalance(node);

    //LL
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }

    //RR
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }

    //LR
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    //RL
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;//tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;//tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO

    //recursive search for an artifact by id
    if (!node) {
        return nullptr;
    }

    if (artifactID == node->data.artifactID) {
        return node;
    }

    if (artifactID < node->data.artifactID) {
        return find(node->left, artifactID);
    }
    
    return find(node->right, artifactID);
}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{

    //assign artifact to researcher
    ArtifactNode *a = findArtifact(artifactID);
    if (a) {
        a->data.assignedToName = researcherName;
    }
}


void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO
    ArtifactNode *node = findArtifact(artifactID);
    if (node) {
        node->data.assignedToName = "";
    }
}

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>
    if (!node) {
        return;
    }

    printUnassigned(node->left);

    if (node->data.assignedToName == "") {
        std::cout << node->data.artifactID << " " << node->data.name << " " << node->data.rarityLevel << " " << node->data.researchValue << std::endl;
    }

    printUnassigned(node->right);
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    // TODO: return size of subtree.
    if (!node) {
        return 0;
    }

    return 1 + getArtifactCount(node->right) + getArtifactCount(node->left);
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    // TODO: sum of rarityLevel over subtree.
    if (!node) {
        return 0;
    }

    return node->data.rarityLevel + getTotalRarity(node->right) + getTotalRarity(node->left);
}

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.
    if (!node) {
        return;
    }

    traversePostOrderForStats(node->left);
    traversePostOrderForStats(node->right);

    if (node->data.assignedToName.empty()) {
        std::cout << node->data.artifactID << " " << node->data.name << " " << node->data.rarityLevel << " " << node->data.researchValue << " UNASSIGNED" <<std::endl;
    }
    else {
        std::cout << node->data.artifactID << " " << node->data.name << " " << node->data.rarityLevel << " " << node->data.researchValue << " " << node->data.assignedToName << std::endl;
    }
}
