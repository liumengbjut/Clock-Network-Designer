#ifndef __BLOSSOM__H__
#define __BLOSSOM__H__

#include <vector>
#include <list>
#include <memory>

// Level of this blossom in some tree of the blossom forest
enum class TreeLevel
{
    // Indexing from zero, roots are even and get charge added
    EVEN,
    // Odd level blossoms get charge removed
    ODD,
    // Dumbbells are not part of the forest and their charge is not changed
    DUMBBELL
};

// Forward-declare Edge and Vertex to avoid cyclic include
class Edge;
class Vertex;

class Blossom
{
public:
    Blossom();

    double getCharge() { return charge; }
    TreeLevel getTreeLevel() { return treeLevel; }

    void addCharge(double delta);

    void updateCharge(double reserve);

    Blossom*        getOutermostBlossom();
    Blossom*        getInnerBlossomContaining(Vertex *v);
    Blossom*        getRootBlossom();
    virtual Vertex* getStem();

    void flipPathAndDisassemble(Vertex* v, Blossom* pair = nullptr, Edge* pairEdge = nullptr);
    void disassembleTreeIntoDumbbells();
    void flipPathBetween(Vertex *v1, Vertex *v2);

    void makeDumbbellWith(Blossom* pair, Edge *edge);
    void connectToTree(Blossom *parent, Edge *parentEdge);

    Blossom* makeBlossomWith(Blossom* pair, Edge *pairEdge);
    void disassembleBlossom();

    // These behave differently in vertices (lowest level blossoms)
    virtual double getMaxNegativeCharge();
    bool updateTree();
    virtual void updateVertexTotal(double delta);


    void debugDump(int indent);
private:
    double charge;

    // Tree organization
    //
    // For in-tree blossoms (odd or even tree level) this points
    // to parent blossom and the edge going to it. One endpoint is
    // current stem, other is in the parent blossom.
    //
    // For dumbbells (out of tree) this points to the other blossom
    // in this pair. The children list is empty.
    TreeLevel               treeLevel;
    Blossom*                treeParentBlossom;
    Edge*                   treeParentEdge;
    std::list<Blossom*>     treeChildren;

    // Recursive blossom organization
    //
    // Vertex level blossoms are kept in memory by the EdmondsMatching for
    // the entire duration of the algorithm. Other blossoms are owned by
    // the inner ones in the outward shared pointer.
    //
    // The odd number of inner blossoms is kept in-order. The first element
    // contains the stem. Each has pointer to the edge connecting it to
    // next blossom (for the last one the next is the stem).
    std::shared_ptr<Blossom>    outwardBlossom;
    std::vector<Blossom*>       innerChildren;
    Edge*                       nextBlossomEdge;
};

#endif
