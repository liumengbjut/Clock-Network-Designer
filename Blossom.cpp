#include "Blossom.hpp"
#include "Edge.hpp"

#include "assert.h"
#include <algorithm>


Blossom::Blossom()
{
    charge = 0;

    treeLevel = TreeLevel::EVEN;
    treeParentBlossom = nullptr;
    treeParentEdge = nullptr;

    outwardBlossom = nullptr;
}

void Blossom::addCharge(double delta)
{
    charge += delta;

    // Update total charge for all vertices contained
    // in this blossom and all sub-blossoms recursively
    updateVertexTotal(delta);
}

double Blossom::getMaxNegativeCharge()
{
    // For non-vertex blossoms the charge must remain >= 0
    return getCharge();
}

void Blossom::updateVertexTotal(double delta)
{
    for (auto const& inner: innerChildren)
        inner->updateVertexTotal(delta);
}

void Blossom::updateCharge(double reserve)
{
    ASSERT(getTreeLevel() != TreeLevel::DUMBBELL, "No dumbbell can be in the forest.");
    ASSERT(this->outwardBlossom == nullptr, "This blossom is contained in another");

    // If we are on even level we get positive charge,
    // negative charge on odd level
    if (getTreeLevel() == TreeLevel::EVEN)
        addCharge(reserve);
    else
        addCharge(-reserve);

    // Recursively update all tree children
    for (auto const& child : treeChildren)
        child->updateCharge(reserve);
}

bool Blossom::updateTree()
{
    ASSERT(this->outwardBlossom.get() == nullptr, "This blossom is contained in another");

    // Handle children first, since disassembling blossom will mess things up
    for (auto const &child : treeChildren)
    {
        // Stop after first change
        if (child->updateTree())
            return true;
    }

    // Case (P1) - non-vertex blossom on odd level has reached zero charge,
    // disassemble it
    if (getMaxNegativeCharge() == 0 && getTreeLevel() == TreeLevel::ODD)
    {
        disassembleBlossom();
        return true;
    }

    return false;
}

Blossom* Blossom::getOutermostBlossom()
{
    Blossom* outermost = this;
    while (outermost->outwardBlossom.get() != nullptr)
        outermost = outermost->outwardBlossom.get();
    return outermost;
}

Blossom *Blossom::getInnerBlossomContaining(Vertex *v)
{
    Blossom *inner = v;
    while (inner != nullptr && inner->outwardBlossom.get() != this)
        inner = inner->outwardBlossom.get();
    ASSERT(inner != nullptr, "Vertex v is not part of any inner blossoms");
    return inner;
}

Blossom* Blossom::getRootBlossom()
{
    Blossom* root = this;
    while (root->treeParentBlossom != nullptr)
        root = root->treeParentBlossom;
    return root;
}

Vertex* Blossom::getStem()
{
    ASSERT(innerChildren.size() > 0, "This non-vertex blossom has no inner blossoms.");
    return innerChildren[0]->getStem();
}

void Blossom::flipPathAndDisassemble(Vertex *v, Blossom *pair, Edge *pairEdge)
{
    ASSERT(this->outwardBlossom == nullptr, "This blossom is contained in another");
    ASSERT(pair == nullptr || getTreeLevel() == TreeLevel::EVEN,
           "Can only form dumbbell from even level blossom with odd level child.");

    // Flip path between v and the vertex connecting us to parent
    // For even level this is the stem, for odd level we must find it
    Vertex *v2 = getStem();
    if (getTreeLevel() == TreeLevel::ODD)
    {
        v2 = treeParentEdge->getV1();
        if (v2->getOutermostBlossom() != this)
            v2 = treeParentEdge->getV2();
        ASSERT(v2->getOutermostBlossom() == this, "Neither edge endpoint in this blossom");
    }
    flipPathBetween(v, v2);

    // Disconnect all children and disassemble into dumbbells
    // Only do this in even level blossom since odd level ones only have a single
    // child with which we will be forming dumbbell anyway.
    if (getTreeLevel() == TreeLevel::EVEN)
    {
        for (auto const &child: treeChildren)
        {
            // Skip the child blossom with which we will form a dumbbell
            if (child != pair)
                child->disassembleTreeIntoDumbbells();
        }
    }

    // Remove all children nodes
    treeChildren.clear();

    // Store parent information, since we will delete it
    Blossom *parent = treeParentBlossom;
    Edge *parentEdge = treeParentEdge;

    treeParentBlossom = nullptr;
    treeParentEdge = nullptr;

    // Store tree level, since making a dumbbell will delete it
    TreeLevel oldTreeLevel = getTreeLevel();

    // For a dumbbell with one of the children if needed
    if (pair != nullptr)
        makeDumbbellWith(pair, pairEdge);

    // Recursively handle the parent blossom all the way to the root
    if (parent == nullptr)
        return;

    // We need to find the vertex to which we connect in the parent blossom
    Vertex *pv = parentEdge->getV1();
    if (pv->getOutermostBlossom() == this)
        pv = parentEdge->getV2();
    ASSERT(pv->getOutermostBlossom() == parent, "Neither edge endpoint in parent");

    // Flip the parent edge
    parentEdge->flip();

    // For even level we are making dumbbell with a provided child blossom,
    // for odd level we will be making dumbbell with the parent and need to
    // provide pointer to us.
    if (oldTreeLevel == TreeLevel::EVEN)
        parent->flipPathAndDisassemble(pv);
    else
        parent->flipPathAndDisassemble(pv, this, parentEdge);
}

void Blossom::disassembleTreeIntoDumbbells()
{
    ASSERT(outwardBlossom == nullptr, "This blossom is contained in another");

    // For even level blossoms recursively disassemble all odd level children
    if (getTreeLevel() == TreeLevel::EVEN)
    {
        for (auto const& child: treeChildren)
            child->disassembleTreeIntoDumbbells();

        // Clear children list and parent edge
        treeChildren.clear();
        treeParentBlossom = nullptr;
        treeParentEdge = nullptr;
    }
    else
    {
        // Odd level blossoms have exactly one even level child, form a dumbbell
        ASSERT(treeChildren.size() == 1, "Odd level blossom doesn't have one child");

        // Store pointers to pair and connecting edge for later
        Blossom *pair = treeChildren.front();
        Edge *pairEdge = treeChildren.front()->treeParentEdge;

        // First disassemble the pair recursively
        pair->disassembleTreeIntoDumbbells();

        // Clear children list and parent edge for this blossom, pair
        // will be cleared in the recursive call
        treeChildren.clear();
        treeParentBlossom = nullptr;
        treeParentEdge = nullptr;

        // Form a dumbbell
        makeDumbbellWith(pair, pairEdge);
    }
}

void Blossom::flipPathBetween(Vertex *v1, Vertex *v2)
{
    // Make sure v1 is the stem of current blossom
    if (getStem() != v1)
        std::swap(v1, v2);
    ASSERT(getStem() == v1, "One of v1, v2 must be the stem");

    // If we have no inner blossoms this must be the vertex level blossom
    // and thus v1 must equal v2 and nothing has to be done
    if (innerChildren.size() == 0)
    {
        ASSERT(v1 == v2, "v1 and v2 are not equal but in vertex together");
        return;
    }

    // Find the child blossom to which v2 belongs
    Blossom *b2 = getInnerBlossomContaining(v2);
    // We want a path of even length between the inner blossoms, we have odd
    // number of them, pick one of the directions.
    // We will go from first to last using the next blossom pointers,
    // potentially wrapping around at the end of the cycle
    Blossom *first = innerChildren[0];
    Blossom *last = b2;

    // Find the index of this blossom in the inner cycle to determine the direction
    auto b2_pos = std::find(innerChildren.begin(), innerChildren.end(), b2);
    long pos = std::distance(innerChildren.begin(), b2_pos);
    if (pos % 2 == 1)
    {
        std::swap(v1, v2);
        std::swap(first, last);
    }

    // Now go from first to last, recursively handling each inner blossom
    // and flipping the edge connecting them.
    // v1 and v2 will always be the first and last vertex on this path,
    // which we need to update from the connecting edge.
    Blossom *current = first;
    while (current != last)
    {
        // v1 is the in the current inner blossom
        // One of the following will be the v2 for current blossom, make sure
        // it's b_v1 by swapping them
        Vertex *b_v1 = current->nextBlossomEdge->getV1();
        Vertex *b_v2 = current->nextBlossomEdge->getV2();
        if (getInnerBlossomContaining(b_v1) != current)
            std::swap(b_v1, b_v2);

        // Recursively flip path in the current inner blossom
        current->flipPathBetween(v1, b_v1);
        // Flip the connecting edge
        current->nextBlossomEdge->flip();

        // Advance to the next inner blossom, update v1
        current = getInnerBlossomContaining(b_v2);
        v1 = b_v2;
    }
    // Handle the last inner blossom - we just have to do a recursive call,
    // v1 and v2 are already set correctly
    last->flipPathBetween(v1, v2);

    // Rotate the cycle of inner blossoms so that b2 is now the stem
    std::rotate(innerChildren.begin(), b2_pos, innerChildren.end());
}

void Blossom::connectToTree(Blossom *parent, Edge *parentEdge)
{
    ASSERT(getTreeLevel() == TreeLevel::DUMBBELL, "Not a dumbbell");
    ASSERT(parent->getTreeLevel() == TreeLevel::EVEN, "Parent not on even level");

    // Change from dumbbell to a odd-even tree pair, add other dumbbell pair
    // as a tree child
    treeLevel = TreeLevel::ODD;
    treeChildren.push_back(treeParentBlossom);
    treeParentBlossom->treeLevel = TreeLevel::EVEN;

    // Add self to parent's children, change parent edge
    treeParentBlossom = parent;
    treeParentEdge = parentEdge;
    treeParentBlossom->treeChildren.push_back(this);
}

void Blossom::makeDumbbellWith(Blossom *pair, Edge *edge)
{
    ASSERT(this->outwardBlossom == nullptr, "This blossom is contained in another");
    ASSERT(pair->outwardBlossom == nullptr, "Other blossom is contained in another");
    ASSERT(this->treeChildren.size() == 0, "This blossom still has tree children");
    ASSERT(pair->treeChildren.size() == 0, "Other blossom still has tree children");
    ASSERT(this->treeParentBlossom == nullptr, "This blossom still has tree parent");
    ASSERT(pair->treeParentBlossom == nullptr, "Other blossom still has tree parent");

    ASSERT((edge->getV1() == getStem() && edge->getV2() == pair->getStem()) ||
           (edge->getV2() == getStem() && edge->getV1() == pair->getStem()),
           "Edge must be between stems");

    // Set tree level, link together with the parent edge
    this->treeLevel = TreeLevel::DUMBBELL;
    pair->treeLevel = TreeLevel::DUMBBELL;

    this->treeParentBlossom = pair;
    pair->treeParentBlossom = this;
    this->treeParentEdge = edge;
    pair->treeParentEdge = edge;
}

Blossom* Blossom::makeBlossomWith(Blossom *pair, Edge *pairEdge)
{
    ASSERT(this->outwardBlossom == nullptr, "This blossom is contained in another");
    ASSERT(pair->outwardBlossom == nullptr, "Other blossom is contained in another");

    // First find lowest common ancestor and paths to it
    std::list<Blossom *> ourPath;
    std::list<Blossom *> pairPath;
    ourPath.push_back(this);
    pairPath.push_back(pair);
    while (ourPath.back()->treeParentBlossom != nullptr)
        ourPath.push_back(ourPath.back()->treeParentBlossom);
    while (pairPath.back()->treeParentBlossom != nullptr)
        pairPath.push_back(pairPath.back()->treeParentBlossom);

    // LCA will be the one before first difference
    Blossom *LCA = nullptr;
    ASSERT(ourPath.back() == pairPath.back(), "Blossoms not in the same tree");
    while (ourPath.size() > 0 && pairPath.size() > 0 &&
           ourPath.back() == pairPath.back())
    {
        LCA = ourPath.back();
        ourPath.erase(std::prev(ourPath.end()));
        pairPath.erase(std::prev(pairPath.end()));
    }

    // From LCA and two path of even length now construct new blossom.
    ASSERT(ourPath.size() % 2 == 0, "Path to LCA has odd length.");
    ASSERT(pairPath.size() % 2 == 0, "Path to LCA has odd length.");
    ASSERT(LCA->getTreeLevel() == TreeLevel::EVEN, "LCA not on even level.");

    // Create new blossom,, by default on even level with zero charge
    std::shared_ptr<Blossom> newBlossom = std::make_shared<Blossom>();

    // Replace LCA as the child of LCA's parent in the tree
    newBlossom->treeParentBlossom = LCA->treeParentBlossom;
    newBlossom->treeParentEdge = LCA->treeParentEdge;
    if (newBlossom->treeParentBlossom != nullptr)
    {
        newBlossom->treeParentBlossom->treeChildren.remove(LCA);
        newBlossom->treeParentBlossom->treeChildren.push_back(newBlossom.get());
    }

    // Add all blossoms on the LCA path and LCA itself as inner blossoms, the
    // LCA will be the stem. To form a cycle one path must be reversed.
    //
    // Also store edges connecting each blossom to the next one in the cycle,
    // for the last one this is the edge to the LCA.
    newBlossom->innerChildren.push_back(LCA);
    // First half of the cycle
    Blossom *previous = LCA;
    for (auto it = ourPath.rbegin(); it != ourPath.rend(); ++it)
    {
        newBlossom->innerChildren.push_back(*it);
        // Going from LCA down, but parent edges are stored the other way
        previous->nextBlossomEdge = (*it)->treeParentEdge;
        previous = *it;
    }
    // Set the edge for the last blossom (which will be the current one)
    previous->nextBlossomEdge = pairEdge;
    // Second half of the cycle
    for (auto it = pairPath.begin(); it != pairPath.end(); ++it)
    {
        newBlossom->innerChildren.push_back(*it);
        // Parent edges point the same way as the cycle direction
        (*it)->nextBlossomEdge = (*it)->treeParentEdge;
    }

    // Store shared pointer to the new outer blossom
    for (auto const &inner : newBlossom->innerChildren)
        inner->outwardBlossom = newBlossom;

    // Change all parent edges to point to the new blossom, add all children
    // which are outermost (that is, not part of this new blossom).
    for (auto const &inner : newBlossom->innerChildren)
    {
        for (auto const &child : inner->treeChildren)
        {
            if (child->outwardBlossom == nullptr)
            {
                newBlossom->treeChildren.push_back(child);
                child->treeParentBlossom = newBlossom.get();
            }
        }
    }

    newBlossom->outwardBlossom = nullptr;

    // If the LCA was the root of this tree we need to exchange it
    if (LCA->treeParentEdge == nullptr)
        return newBlossom.get();
    return nullptr;
}

void Blossom::disassembleBlossom()
{
    // Disassemble current blossom into tree and dumbbells
    ASSERT(outwardBlossom == nullptr, "This blossom is contained in another");
    ASSERT(getTreeLevel() == TreeLevel::ODD, "This blossom is not on odd level");
    ASSERT(treeChildren.size() == 1, "Odd level blossom doesn't have one child");

    // Child blossom is connected to the stem inner blossom
    Edge *childEdge = treeChildren.front()->treeParentEdge;
    ASSERT(childEdge->getV1() == getStem() || childEdge->getV2() == getStem(),
           "Child blossom must be connected to the stem");

    // Parent blossom is connected to one of the inner blossoms
    Blossom *parentInner;
    if (treeParentEdge->getV1()->getOutermostBlossom() == this)
        parentInner = getInnerBlossomContaining(treeParentEdge->getV1());
    else
        parentInner = getInnerBlossomContaining(treeParentEdge->getV2());

    // Find the index of this blossom in the inner cycle
    auto parentInner_pos = std::find(innerChildren.begin(), innerChildren.end(), parentInner);
    int pos = std::distance(innerChildren.begin(), parentInner_pos);

    // The tree part will be formed from the even length path, the odd length path
    // will be split into dumbbells
    // We need to set the appropriate tree level and child/parent pointers
    int N = innerChildren.size();
    // We will cycle from start to end, modulo N, setting pointers to parent
    // from i+diff, edge pointers from i+ediff, with stem being the index of
    // the stem in the cycle
    int start = 0;
    int end = pos;
    int diff = 1;
    int ediff = 0;
    int stem = 0;
    // For odd length we go the other way around, wrapping around at the end
    if (pos % 2 == 1)
    {
        start = pos;
        end = stem = N;
        diff = ediff = -1;
    }

    for (int i = start; i <= end; i++)
    {
        // Tree level will be odd for stem and then alternates
        innerChildren[i%N]->treeLevel = (i % 2 == stem % 2) ? TreeLevel::ODD : TreeLevel::EVEN;
        // Set parent to next inner blossom, except for the last one which
        // will be the child of the current parent.
        innerChildren[i%N]->treeParentBlossom =
                (i == pos) ? treeParentBlossom : innerChildren[i+diff];
        innerChildren[i%N]->treeParentEdge =
                (i == pos) ? treeParentEdge : innerChildren[i+ediff]->nextBlossomEdge;
        // Add previous inner blossom to children, except for the stem which
        // will be the parent of current child.
        innerChildren[i%N]->treeChildren.clear();
        innerChildren[i%N]->treeChildren.push_back(
                (i == stem) ? treeChildren.front() : innerChildren[(i-diff)%N]
        );
    }

    // Exchange child pointer for parent from this blossom to inner one
    treeParentBlossom->treeChildren.remove(this);
    treeParentBlossom->treeChildren.push_back(innerChildren[pos]);
    // Set parent blossom to stem for the child, parent edge already set
    treeChildren.front()->treeParentBlossom = innerChildren[0];

    // To delete this blossom we reset all outward pointers to it
    std::shared_ptr<Blossom> thiscopy = innerChildren.front()->outwardBlossom;
    for (auto const& inner: innerChildren)
        inner->outwardBlossom.reset();

    // Take the remaining odd length cycle range and disassemble it
    // into dumbbells. Go in the direction of next pointers
    for (int i = (pos % 2 == 0) ? pos + 1 : 1;
         (pos % 2 == 0) ? i < N : i < pos;
         i+=2)
    {
        innerChildren[i]->treeChildren.clear();
        innerChildren[i+1]->treeChildren.clear();
        innerChildren[i]->treeParentBlossom = nullptr;
        innerChildren[i+1]->treeParentBlossom = nullptr;

        innerChildren[i]->makeDumbbellWith(innerChildren[i+1],
                                           innerChildren[i]->nextBlossomEdge);
    }
}
