#ifndef __EDMONDSMATCHING__H__
#define __EDMONDSMATCHING__H__

#include <vector>
#include <list>


#include "Blossom.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"

class EdmondsMatching
{
public:
    EdmondsMatching(int numberOfVertices);
    void addEdge(int v1id, int v2id, int weight);
    bool findMinimumWeightMatching();

    void removeRootFromForest(Blossom *root);
    void addRootToForest(Blossom *root);

    void increaseMatchingSize() { matchingSize += 1; }

    int getMatchingWeight();
    std::vector<std::pair<int, int>> getMatchedEdges();

private:
    int matchingSize;
    int numberOfVertices;

    std::vector<std::unique_ptr<Vertex>> vertices;
    std::vector<std::unique_ptr<Edge>> edges;
    std::list<Blossom*> roots;
};



#endif
