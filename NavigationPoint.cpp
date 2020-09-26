#include "NavigationPoint.h"
#include <random>
#include <algorithm>

// The destination node we use.
NavigationPoint* NavigationPoint::m_destinationNode = NULL;


// Color definition.
float NavigationPoint::m_initialColor[4] = {0.0, 1.0, 0.0, 1.0};
float NavigationPoint::m_visitedColor[4] = {1.0, 1.0, 0.0, 1.0};
float NavigationPoint::m_closedColor[4] = {1.0, 0.0, 0.0, 1.0};
float NavigationPoint::m_solutionColor[4] = {0.0, 0.0, 1.0, 1.0};


NavigationPoint::NavigationPoint(void)
{
	m_coordinates[0] = (float)rand() / RAND_MAX;
	m_coordinates[1] = (float)rand() / RAND_MAX;
	ResetNode(false);
}


// Sets the destination node.
void NavigationPoint::SetDestinationNode(NavigationPoint* destination)
{
	m_destinationNode = destination;
}


// Establishes a link with another node.
void NavigationPoint::EstablishLinkWith(NavigationPoint* otherNode)
{
	// We assume that we do not have the link already.
	float distance = ComputeDistanceTo(otherNode);
	InternalLink newPartner;
	newPartner.m_partnerNode = otherNode;
	newPartner.m_distance = distance;
	m_partnerNodes.push_back(newPartner);

	// Do the reciprocal also.
	newPartner.m_partnerNode = this;
	otherNode->m_partnerNodes.push_back(newPartner);
}

// Returns the position of the navigation point needed for rendering.
const float* NavigationPoint::ObtainPosition()
{
	return (&m_coordinates[0]);
}

// Resets the node to make the calculation of a new round possible.
void NavigationPoint::ResetNode(bool isStartNode)
{
	m_accumulatedDistance = m_nodeEvaluation = 0.0f;
	m_rootNode = NULL;

	if (isStartNode)
		m_internalState = NavigationPoint::VISITED;
	else
		m_internalState = NavigationPoint::INITIAL;
}

// Checks if we are in the vicinty of a coordinate handed over.
bool NavigationPoint::IsCloseTo(const float position[2])
{
	float dx = position[0] - m_coordinates[0];
	float dy = position[1] - m_coordinates[1];

	return (dx * dx + dy * dy < 0.01f * 0.01f);
}

// Locks in the current node and continues processing of the neighbors.
// processingList: This is the list of open nodes that requires processing.
void NavigationPoint::LockInNode(vector<NavigationPoint*>&  processingList)
{
	m_internalState = CLOSED;

	int cap = m_partnerNodes.size();

	for (int i = 0; i < cap; ++i)
	{
		NavigationPoint* candidate = m_partnerNodes[i].m_partnerNode;
		
		switch (candidate->m_internalState)
		{
		case CLOSED:
		case SOLUTION:
			break;
		case INITIAL:
			candidate->m_internalState = VISITED;
			candidate->m_accumulatedDistance = m_accumulatedDistance + m_partnerNodes[i].m_distance;
			candidate->m_nodeEvaluation = candidate->m_accumulatedDistance + candidate->ComputeDistanceTo(m_destinationNode);
			candidate->m_rootNode = this;
			processingList.push_back(candidate);
			break;
		case VISITED:
			float newDistance = m_accumulatedDistance + m_partnerNodes[i].m_distance;
			
			if (newDistance > candidate->m_accumulatedDistance)
			{
				break;
			}

			candidate->m_accumulatedDistance = newDistance;
			candidate->m_nodeEvaluation = candidate->m_accumulatedDistance + candidate->ComputeDistanceTo(m_destinationNode);
			candidate->m_rootNode = this;
			break;
		}
	}
}

// Calls in the backprogagation of the final path.
void NavigationPoint::ProcessBackwardsPath()
{
	NavigationPoint* candidate = this;
	while(candidate)
	{
		candidate->m_internalState = NavigationPoint::SOLUTION;
		candidate = candidate->m_rootNode;
	}
}

// Obtains a painting color (RGBA) for the node.
float* NavigationPoint::GetPaintingColor()
{
	switch(m_internalState)
	{
	case NavigationPoint::INITIAL:
		return (&m_initialColor[0]);
	case NavigationPoint::VISITED:
		return (&m_visitedColor[0]);
	case NavigationPoint::CLOSED:
		return (&m_closedColor[0]);
	case NavigationPoint::SOLUTION:
		return (&m_solutionColor[0]);
	default:
		return NULL;
	}
}





