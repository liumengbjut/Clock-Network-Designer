#include "Vertex.hpp"

#include "assert.h"
#include "Edge.hpp"
#include <limits>

double Vertex::getMaxNegativeCharge()
{
    // No negative charge restriction for vertex level blossoms
    return std::numeric_limits<double>::infinity();
}

void Vertex::updateVertexTotal(double delta)
{
    totalCharge += delta;
}

Vertex *Vertex::getStem()
{
    return this;
}
