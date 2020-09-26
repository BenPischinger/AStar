#pragma once
#include <vector>

using namespace std;
class NavigationPoint;
class NodeLink;
class PrimitivePainter;

// Contains the super presentation of all graphs.
class NavigationGraph
{
public:
	~NavigationGraph(void);

	// Generates a navigation graph, does so at the beginning.
	void GenerateNavGraph();
	// Paints the graph with the indicated positions.
	void PaintGraph(PrimitivePainter* painter);

	// Pick start node
	void PickStartNode(float position[2]);
	// Picks the destination node if possible.
	void PickDestination(float position[2]);



private:
	// The navigation points we have in our list.
	vector<NavigationPoint*> m_navigationalPoint;
	// The links we have in our list.
	vector<NodeLink*> m_linkList;
	// The start node we have.
	NavigationPoint* m_startNode;
	// The target node we have.
	NavigationPoint* m_targetNode;

	// Tries to find a node close to the position.
	NavigationPoint* PickNode(float position[2]);
	// Cleas all the information on the graph.
	void ClearGraph();

	// Tries to solve for the A* solution.
	void SolveAStarProblem();

	
};

