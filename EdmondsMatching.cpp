#include "EdmondsMatching.hpp"

#include "assert.h"
#include <algorithm>
#include <limits>


EdmondsMatching::EdmondsMatching(int numberOfVertices)
{
    ASSERT(numberOfVertices % 2 == 0, "numOfVertices should be even");

    this->numberOfVertices = numberOfVertices;
    this->matchingSize = 0;

    vertices.reserve(numberOfVertices);

    for (int i = 0; i < numberOfVertices; i++)
    {
        vertices.push_back(std::unique_ptr<Vertex>(new Vertex(i)));
        roots.push_back(vertices.back().get());
    }
}

void EdmondsMatching::addEdge(int v1id, int v2id, int weight)
{
    Vertex* v1 = vertices[v1id].get();
    Vertex* v2 = vertices[v2id].get();

    ASSERT(v1 != v2, "Graph can't contain loops.");

    edges.push_back(std::unique_ptr<Edge>(new Edge(this, v1, v2, weight)));
}

bool EdmondsMatching::findMinimumWeightMatching()
{
    while(true)
    {
        // Find the minimum across all edges of the maximum charge
        // that edge can tolerate before some invariant gets violated
        double min_reserve = std::numeric_limits<double>::infinity();
        for (auto const &edge : edges)
        {
            double edge_reserve = edge->getMaxCharge();
            min_reserve = std::min(min_reserve, edge_reserve);
        }

        // Add this charge to all trees
        for (auto const &root: roots)
            root->updateCharge(min_reserve);

        // Keep changing until nothing happens
        bool change = false;
        while (true)
        {
            bool round = false;
            for (auto const &root: roots)
            {
                if (root->updateTree())
                {
                    round = change = true;
                    break;
                }
            }
            if (round)
                continue;
            for (auto const &edge: edges)
            {
                if (edge->update())
                {
                    round = change = true;
                    break;
                }
            }
            if (round)
                continue;
            // Nothing has changed this round so we are done
            break;
        }

        // If nothing has changed and no charge can be added we are stuck
        if (!change && min_reserve == 0)
            return false;

        // If we have found a matching it must be optimal
        if (matchingSize*2 == numberOfVertices)
            return true;

        // If we can add any charge there is no perfect matching
        if (min_reserve == std::numeric_limits<double>::infinity())
            return false;
    }
}

void EdmondsMatching::removeRootFromForest(Blossom *root)
{
    roots.remove(root);
}

void EdmondsMatching::addRootToForest(Blossom *root)
{
    roots.push_back(root);
}

int EdmondsMatching::getMatchingWeight()
{
    int weight = 0;
    for (auto const& edge: edges)
    {
        if (edge->isMatched())
            weight += edge->getWeight();
    }

    return weight;
}

std::vector<std::pair<int, int>> EdmondsMatching::getMatchedEdges()
{
    std::vector<std::pair<int, int>> result;
    result.reserve(matchingSize);

    for (auto const& edge: edges)
    {
        if (edge->isMatched())
        {
            result.emplace_back(edge->getV1()->id, edge->getV2()->id);
        }
    }

    return result;
}
