#include "NavigationGraph.h"
#include "NavigationPoint.h"
#include "NodeLink.h"
#include "PrimitivePainter.h"


// Remove the stuff we have generated.
NavigationGraph::~NavigationGraph(void)
{
	ClearGraph();

}


// Generates a navigation graph, does so at the beginning.
void NavigationGraph::GenerateNavGraph()
{
	ClearGraph();
	// First we generates the nodes.
	for(int i = 0; i < 200; ++i)
	{
		NavigationPoint* candidate = NULL;
		while(!candidate)
		{
			bool suceed = true;
			candidate = new NavigationPoint();
			for(int j = 0; j < i; ++j)
			{
				if (candidate->IsCloseTo(m_navigationalPoint[j]->ObtainPosition()))
					suceed = false;
			}
			if (!suceed)
			{
				delete candidate;
				candidate = NULL;
			}
		}
		m_navigationalPoint.push_back(candidate);
	}

	// Now we establish links.
	int linkFillingSize = 0;

	// Try to establish the links.
	for(int i = 0; (i < 100000) && (linkFillingSize < 550); ++i)
	{
		int firstCandidate = (rand() % 200);
		int secondCandidate = (rand() % 200);

		// Trivial reject.
		if (firstCandidate == secondCandidate)
			continue;
		// We check against all other elements.
		NodeLink* candidate = new NodeLink(m_navigationalPoint[firstCandidate], m_navigationalPoint[secondCandidate]);
		bool isValid = candidate->IsValidItself();
		if (isValid)
		{
			for(int j = 0; j < linkFillingSize; ++j)
			{
				if (!candidate->IsValdidAgainst(m_linkList[j]))
				{
					isValid = false;
					break;
				}
			}
		}

		// Depending on whether we are valid or not, we establish the link.
		if (isValid)
		{
			candidate->EstablishLink();
			m_linkList.push_back(candidate);
			++linkFillingSize;
		}
		else
		{
			delete candidate;
		}
	}

	m_startNode = m_navigationalPoint[0];
}


// Paints the graph with the indicated positions.
void NavigationGraph::PaintGraph(PrimitivePainter* painter)
{
	// We start with the lines.
	float start[2];
	float end[2];

	int target = m_linkList.size();
	for(int i = 0; i < target; ++i)
	{
		m_linkList[i]->ObtainLineCoordinates(start, end);
		painter->PaintLine(start, end, m_linkList[i]->IsSolutionLink());
	}

	// Now we draw the nodes
	const float* nodePosition;
	const float* paintColor;

	target = m_navigationalPoint.size();
	for(int i = 0; i < target; ++i)
	{
		paintColor = m_navigationalPoint[i]->GetPaintingColor();
		nodePosition = m_navigationalPoint[i]->ObtainPosition();
		painter->PaintCircle(nodePosition, paintColor);
	}
}


// Tries to pick a  start node
void NavigationGraph::PickStartNode(float position[2])
{
	NavigationPoint* candidate = PickNode(position);
	if (candidate)
	{
		m_startNode = candidate;
	}
}

// Picks the destination node if possible.
void NavigationGraph::PickDestination(float position[2])
{
	NavigationPoint* candidate = PickNode(position);
	if (!candidate)
		return;

	m_targetNode = candidate;
	SolveAStarProblem();	
}



// Tries to find a node close to the position.
NavigationPoint* NavigationGraph::PickNode(float position[2])
{
	int target = m_navigationalPoint.size();
	for(int i = 0; i < target; ++i)
	{
		if (m_navigationalPoint[i]->IsCloseTo(position))
			return m_navigationalPoint[i];
	}
	return NULL;
}


// Cleas all the information on the graph.
void NavigationGraph::ClearGraph()
{
	int targetNodes = m_navigationalPoint.size();
	for(int i = 0; i < targetNodes; ++i)
		delete m_navigationalPoint[i];
	m_navigationalPoint.clear();

	int targetLinks = m_linkList.size();
	for(int i = 0; i < targetLinks; ++i)
		delete m_linkList[i];
	m_linkList.clear();
}

// Tries to solve for the A* solution.
void NavigationGraph::SolveAStarProblem()
{
	// First we reset everything.
	int target = m_navigationalPoint.size();
	for(int i = 0; i < target; ++i)
		m_navigationalPoint[i]->ResetNode(false);
	// Set the start node.
	m_startNode->ResetNode(true);
	// Prep the destination.
	NavigationPoint::SetDestinationNode(m_targetNode);

	// Our processing list.
	vector<NavigationPoint*> processingList;
	processingList.push_back(m_startNode);



	while ((!m_targetNode->IsSolutionNode()) && (!processingList.empty()))
	{
		int target = processingList.size();

		NavigationPoint* candidate = processingList[0];

		float bestValue = candidate->GetEstimatedWeight();

		int bestIndex = 0;

		for (int i = 1; i < target; ++i)
		{
			candidate = processingList[i];

			float candidateValue = candidate->GetEstimatedWeight();

			if (candidateValue < bestValue)
			{
				bestValue = candidateValue;
				bestIndex = i;
			}
		}

		candidate = processingList[bestIndex];

		processingList[bestIndex] = processingList[target - 1];
		processingList.pop_back();

		candidate->LockInNode(processingList);

		if (candidate == m_targetNode) 
		{
			candidate->ProcessBackwardsPath();
		}
	}
	
		
}
