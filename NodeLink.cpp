#include "NodeLink.h"
#include "NavigationPoint.h"


// Constructor takes the two candidates.
NodeLink::NodeLink(NavigationPoint* firstCandidate, NavigationPoint* secondCandidate)
{
	m_firstCandidate = firstCandidate;
	m_secondCandidate = secondCandidate;
}
	
// Indicates if the line segment is not a duplicate and does not intersect with the other routine.
bool NodeLink::IsValdidAgainst(NodeLink* otherLink)
{
	// First we check if we describe the same link.
	if ((m_firstCandidate == otherLink->m_firstCandidate) && (m_secondCandidate == otherLink->m_secondCandidate))
		return false;
	// Also check the otherway around.
	if ((m_firstCandidate == otherLink->m_secondCandidate) && (m_secondCandidate == otherLink->m_firstCandidate))
		return false;




	// Check if we have an intersection situation.
	if (IntersectLineSegments(m_firstCandidate->ObtainPosition(), m_secondCandidate->ObtainPosition(),
		otherLink->m_firstCandidate->ObtainPosition(), otherLink->m_secondCandidate->ObtainPosition()))
		return false;

	return true;
}

// Checks if the link is valid itself.
bool  NodeLink::IsValidItself()
{
	// We reject links if they cover more than 0.2 of distance.
	return (m_firstCandidate->ComputeDistanceTo(m_secondCandidate) < 0.2f);
}


// Extracts the coordinates from the line segment.
void NodeLink::ObtainLineCoordinates(float start[2], float end[2])
{
	const float* position = m_firstCandidate->ObtainPosition();
	start[0] = position[0];
	start[1] = position[1];

	position = m_secondCandidate->ObtainPosition();
	end[0] = position[0];
	end[1] = position[1];

}

// Establishes the link in the between the nodes.
void NodeLink::EstablishLink()
{
	m_firstCandidate->EstablishLinkWith(m_secondCandidate);
}


// Checks if we are a link between two solution nodes.
bool NodeLink::IsSolutionLink()
{
	return (m_firstCandidate->IsSolutionNode() && m_secondCandidate->IsSolutionNode());
}

// Checks if two line segments intersect with each other.
// The start and the end postion are handed over as parameters.
bool NodeLink::IntersectLineSegments(const float* startA, const float* endA, const float* startB, const float* endB)
{
	// l * (ea-sa) + m * (sb - eb) = sb - sa
	float resultDelta[2];
	resultDelta[0] = startB[0] - startA[0];
	resultDelta[1] = startB[1] - startA[1];

	float lcoeff[2];
	lcoeff[0] = endA[0] - startA[0];
	lcoeff[1] = endA[1] - startA[1];

	float mcoeff[2];
	mcoeff[0] = startB[0] - endB[0];
	mcoeff[1] = startB[1] - endB[1];

	float baseDet = 1.0f / (lcoeff[0] * mcoeff[1] - lcoeff[1] * mcoeff[0]);

	float l = baseDet * (resultDelta[0] * mcoeff[1] - resultDelta[1] * mcoeff[0]);
	if ((l < 0.00001f) || (l > 0.9999f))
		return false;

	float m = baseDet *  (lcoeff[0] * resultDelta[1] - lcoeff[1] * resultDelta[0]);
	if ((m < 0.00001f) || (m > 0.9999f))
		return false;

	return true;
}