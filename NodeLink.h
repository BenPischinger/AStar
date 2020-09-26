#pragma once


class NavigationPoint;

// This is simply a helper class that is used to establish the network.
// It is means as a validator to see if a certain link can be established.
class NodeLink
{
public:

	// Constructor takes the two candidates.
	NodeLink(NavigationPoint* firstCandidate, NavigationPoint* secondCandidate);
	
	// Indicates if the line segment is not a duplicate and does not intersect with the other routine.
	bool IsValdidAgainst(NodeLink* otherLink);
	// Checks if the link is valid itself.
	bool IsValidItself();
	// Establishes the link in the between the nodes.
	void EstablishLink();
	// Extracts the coordinates from the line segment.
	void ObtainLineCoordinates(float start[2], float end[2]);

	// Checks if we are a link between two solution nodes.
	bool IsSolutionLink();

private:

	// Checks if two line segments intersect with each other.
	bool IntersectLineSegments(const float* startA, const float* endA, const float* startB, const float* endB);

	// The first candidate of the line we use.
	NavigationPoint* m_firstCandidate;
	// The second candidate of the line we use.
	NavigationPoint* m_secondCandidate;
};

