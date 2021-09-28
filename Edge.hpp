#ifndef __EDGE__H__
#define __EDGE__H__

#include "Vertex.hpp"

class EdmondsMatching;

class Edge
{
public:
    Edge(EdmondsMatching* edmondsMatching, Vertex* v1, Vertex* v2, int weight);

    Vertex* getV1() { return v1; }
    Vertex* getV2() { return v2; }

    double getMaxCharge();
    bool update();

    void flip();

    bool isMatched() { return matched; }
    int getWeight() { return weight; }

private:
    EdmondsMatching* edmondsMatching;

    Vertex* v1;
    Vertex* v2;
    int weight;

    bool matched;

    bool isBetweenDumbbells(Blossom* outer1, Blossom* outer2)
    {
        return (outer1->getTreeLevel() == TreeLevel::DUMBBELL &&
                outer2->getTreeLevel() == TreeLevel::DUMBBELL);
    }
    bool isBetweenEvenBlossoms(Blossom* outer1, Blossom* outer2)
    {
        return (outer1->getTreeLevel() == TreeLevel::EVEN &&
                outer2->getTreeLevel() == TreeLevel::EVEN);
    }
    bool isBetweenEvenAndDumbbell(Blossom* outer1, Blossom* outer2)
    {
        return ((outer1->getTreeLevel() == TreeLevel::DUMBBELL &&
                 outer2->getTreeLevel() == TreeLevel::EVEN) ||
                (outer1->getTreeLevel() == TreeLevel::EVEN &&
                 outer2->getTreeLevel() == TreeLevel::DUMBBELL));
    }
};

#endif
