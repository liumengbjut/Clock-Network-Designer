#ifndef __VERTEX__H__
#define __VERTEX__H__

#include "Blossom.hpp"
class Edge;

class Vertex : public Blossom
{
public:
    Vertex(int _id) : Blossom(), id(_id), totalCharge(0) {};

    double getTotalCharge() { return totalCharge; }

    // Overriding behaviour for vertex blossoms
    virtual double getMaxNegativeCharge() override;
    virtual void updateVertexTotal(double delta) override;
    virtual Vertex *getStem() override;

    const int id;

private:
    // Total charge of all blossoms that contain this vertex
    double totalCharge;
};

#endif