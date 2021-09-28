#include "Edge.hpp"

#include "EdmondsMatching.hpp"

#include "assert.h"
#include <algorithm>
#include <limits>


Edge::Edge(EdmondsMatching *edmondsMatching, Vertex *v1, Vertex *v2, int weight)
{
    this->edmondsMatching = edmondsMatching;
    this->v1 = v1;
    this->v2 = v2;
    this->weight = weight;

    this->matched = false;
}

double Edge::getMaxCharge()
{
    auto outer1 = v1->getOutermostBlossom();
    auto outer2 = v2->getOutermostBlossom();

    // Edges contained inside the same outermost blossom
    // will not have any charge added or removed
    if (outer1 == outer2)
        return std::numeric_limits<double>::infinity();

    // Reserve on this edge - since endpoints are in different
    // outermost blossoms, each blossom containing either endpoint
    // is cutting this edge
    double reserve = weight - v1->getTotalCharge()
                            - v2->getTotalCharge();

    // Edge between dumbbells will have no charge change
    if (isBetweenDumbbells(outer1, outer2))
        return std::numeric_limits<double>::infinity();

    // Edge between a dumbbell and even level vertex
    if (isBetweenEvenAndDumbbell(outer1, outer2))
        return reserve;

    // Edge between two even level blossoms - can only add half
    // of the reserve since it will be added to both endpoints
    if (isBetweenEvenBlossoms(outer1, outer2))
        return reserve / 2;

    // For the remaining case, at least one of the outermost
    // blossoms is on an odd level therefore we need check for
    // negative charge only
    reserve = std::numeric_limits<double>::infinity();
    if (outer1->getTreeLevel() == TreeLevel::ODD)
        reserve = std::min(reserve, outer1->getMaxNegativeCharge());
    if (outer2->getTreeLevel() == TreeLevel::ODD)
        reserve = std::min(reserve, outer2->getMaxNegativeCharge());
    return reserve;
}

bool Edge::update()
{
    // We don't have to do anything if this edge is not full
    if (getMaxCharge() > 0)
        return false;

    auto outer1 = v1->getOutermostBlossom();
    auto outer2 = v2->getOutermostBlossom();
    ASSERT(outer1 != outer2, "Edge within the same blossom got full.");

    // Case (P2) - edge between a dumbbell and even level blossom
    if (isBetweenEvenAndDumbbell(outer1, outer2))
    {
        if (outer1->getTreeLevel() == TreeLevel::DUMBBELL)
            outer1->connectToTree(outer2, this);
        else
            outer2->connectToTree(outer1, this);
        return true;
    }

    // Cases (P3) and (P4) - edge between two even level blossoms
    if (isBetweenEvenBlossoms(outer1, outer2))
    {
        auto root1 = outer1->getRootBlossom();
        auto root2 = outer2->getRootBlossom();

        // Case (P3) - within the same tree, create new blossom
        if (root1 == root2)
        {
            Blossom *newRoot = outer1->makeBlossomWith(outer2, this);
            // If the root of the tree was replaced with a new blossom we need
            // to update this in the EdmondsMatching
            if (newRoot != nullptr)
            {
                edmondsMatching->removeRootFromForest(root1);
                edmondsMatching->addRootToForest(newRoot);
            }
        }
        // Case (P4) - between different trees, flip path to increase matching
        else
        {
            // Disassemble tree into dumbbells, keeping the two
            // outer blossoms separate. Flip the edges on the path
            // to root.
            outer1->flipPathAndDisassemble(v1);
            outer2->flipPathAndDisassemble(v2);

            // Remove the roots from the root list
            edmondsMatching->removeRootFromForest(root1);
            edmondsMatching->removeRootFromForest(root2);

            // Join the two blossoms into a dumbbell, flip the edge between them
            outer1->makeDumbbellWith(outer2, this);
            this->flip();

            // Increase matching size
            edmondsMatching->increaseMatchingSize();
        }

        return true;
    }
}

void Edge::flip()
{
    matched = !matched;
}
