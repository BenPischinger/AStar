#pragma once
#include<vector>
#include<math.h>


using namespace std;

// Represents a point in the navigation hierarchie.
class NavigationPoint
{
public:
	
	// Constructs the navigation point with random coordinates [0,1]^2
	NavigationPoint();

	// Sets the destination node.
	static void SetDestinationNode(NavigationPoint* destination);
	// Establishes a link with another node.
	void EstablishLinkWith(NavigationPoint* otherNode);

	
	// Returns the position of the navigation point needed for rendering.
	const float* ObtainPosition();
	// Resets the node to make the calculation of a new round possible.
	void ResetNode(bool isStartNode);
	// Checks if we are in the vicinty of a coordinate handed over.
	bool IsCloseTo(const float position[2]);
	// Locks in the current node and continues processing of the neighbors.
	void LockInNode(vector<NavigationPoint*>&  processingList);
	// Calls in the backprogagation of the final path.
	void ProcessBackwardsPath();
	// Obtains a painting color (RGBA) for the node.
	float* GetPaintingColor();
	// Checks if we are a solution node.
	bool IsSolutionNode() { return m_internalState == SOLUTION; }
	// Get the estimated weight.
	float GetEstimatedWeight() { return m_nodeEvaluation; }

	// Gets the distance to a partner node.
	float ComputeDistanceTo(NavigationPoint* partnerNode)
	{
		float dx = partnerNode->m_coordinates[0] - m_coordinates[0];
		float dy = partnerNode->m_coordinates[1] - m_coordinates[1];
		return sqrtf(dx * dx + dy * dy);
	}
	
private:
	// The position of the navigation point.
	float m_coordinates[2];
	// Describes a link to another node in the graph.
	struct InternalLink
	{
		// The link to the partner node.
		NavigationPoint* m_partnerNode;
		// The distance we have to the partner node.
		float m_distance;
	};
	// The list with the partner nodes we have.
	vector<InternalLink> m_partnerNodes;


	// The destination node we use.
	static NavigationPoint* m_destinationNode;

	// ----------------------------------------------------------
	// Visualization color applied.
	// ----------------------------------------------------------

	static float m_initialColor[4];
	static float m_visitedColor[4];
	static float m_closedColor[4];
	static float m_solutionColor[4];

	// ----------------------------------------------------------
	// The follwing information gets dynamically updated with the A star algorithm.
	// -----------------------------------------------------------

	enum NodeState
	{
		// The node has not been processed by the algorithm yet.
		INITIAL,
		// The node has already been visited once.
		VISITED,
		// The node is already closed.
		CLOSED,
		// The node has been identified as beeing part of the solution.
		SOLUTION
	};

	// The state of the node we are in.
	NodeState m_internalState;

	// The distance that already got accumulated from the start.
	float m_accumulatedDistance;
	// The complete evaluation of the node, wich is the accumulated distance and the heuristic estimation.
	float m_nodeEvaluation;
	// The base node to track back the solution.
	NavigationPoint* m_rootNode;


	




};

