#include "provided.h"
//#include "support.h"
#include <queue>
#include <list>
#include <cmath>
using namespace std;

//Declring a class called Vertex, which represents the Vertex of a graph, carrying all the info required to implement the A* algorithm
class Vertex    //it is declared as a class and not a struct purely because 'this' doesn't work with classes
{
public:
	GeoCoord P;		//Position of the vertex
	double L;		//Label = shortest distance to that point
	double H;		//Heuristic measure

	double D;		//Euclidian distance to target
	GeoCoord prev;  //the previous vertex it came from
	StreetSegment cameThrough;

	bool isStart;   //if isStart == true, don't check prev, cameThrough

	Vertex(GeoCoord pos, GeoCoord ending)
	{
		isStart = true;
		P = pos;
		L = 0;
		D = distanceEarthMiles(ending, P);
		H = D + L;
	}

	Vertex(GeoCoord pos, Vertex pr, StreetSegment cT, GeoCoord ending)
	{
		isStart = false;

		P = pos;
		prev = pr.P;
		cameThrough = cT;

		//since I only create a vertex if it's next to me, I don't have to have the infinity initialization
		L = distanceEarthMiles(pr.P, pos) + pr.L;

		D = distanceEarthMiles(ending, P);

		H = D + L;
	}

	bool operator==(const Vertex& other) const   //check if two vertices are the same
	{
		if (P.latitude == other.P.latitude && P.longitude == other.P.longitude)
			return true;
		return false;
	}

	bool operator==(const GeoCoord& other) const  //check if two vertices are the same by passing in a coordinate
	{
		if (P.latitude == other.latitude && P.longitude == other.longitude)
			return true;
		return false;
	}

	bool operator<(const Vertex& other) const
	{
		return H > other.H;         //we're going to use this in a priority queue.
	}							    //so v1 > v2 (more priority) if v1.H < v2.H
									//so reverse them
	bool operator!=(const GeoCoord& other) const
	{
		return !(*this == other);

	}

};
//end of Vertex class

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;

private:
    const StreetMap* m_map;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_map = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
	vector<StreetSegment> s;
	if (m_map->getSegmentsThatStartWith(start, s) == false || m_map->getSegmentsThatStartWith(end, s) == false)
		return BAD_COORD;

	Vertex B(start, end); // define starting vertex

	vector<Vertex> haveBeenCircled;
	priority_queue<Vertex> q;          //all the vertices yet to be explored
	priority_queue<Vertex> temp;       //utility

	q.push(B); //push starting vertex onto priority queue

	int returnval = 0;
	while (q.top() != end)
	{
		Vertex curr(q.top());
		q.pop();
		haveBeenCircled.push_back(curr);

		vector<StreetSegment> streets;     //all the street segments that start at curr

		m_map->getSegmentsThatStartWith(curr.P, streets);

		for (int i = 0; i < streets.size(); i++)    // for each possible street
		{
			StreetSegment st = streets[i];

			Vertex v(st.end, curr, st, end);    //find the corresponding vertex

			bool circled = false;   //checking if the given vertex has already been explored
			for (int j = 0; j < haveBeenCircled.size(); j++)
			{
				if (haveBeenCircled[j] == v)
					circled = true;

			}
			if (circled == true)
				continue;

			// if it's not circled, add it to the priority queue/ update its label in the priority queue
			bool alreadyPresent = false;

			if (!q.empty())
				q.swap(temp);

			while (!temp.empty())
			{
				if (temp.top() == v)  //does it have the same position?
				{
					alreadyPresent = true;
					if (v < temp.top())   //and is it more efficient??
					{
						q.push(v);       //replace temp.top() with v
						temp.pop();
						continue;
					}
				}

				q.push(temp.top());
				temp.pop();
			}
			// any vertices that were already in the priority queue are now updated if necessary

			if (alreadyPresent == false)
				q.push(v);

			//the priority queue is now completely updated

		}
		//we're done for the vertex curr
		streets.clear();

		if (q.empty())
		{
			returnval = 1;
			break;
		}
	}
	//end of while

	if (returnval == 1)
		return NO_ROUTE;

	else
	{
		route.clear();
		totalDistanceTravelled = 0;
		Vertex p = q.top();

		//cerr<<"Route in reverse order:"<<endl;
		while (!p.isStart)
		{
			//cerr << p.P.latitudeText << "  " << p.P.longitudeText << endl;

			route.push_front(p.cameThrough);

			totalDistanceTravelled += distanceEarthMiles(p.cameThrough.end, p.cameThrough.start);

			GeoCoord prev = p.cameThrough.start;			     //the coord of the prev vertex
			for (int i = 0; i < haveBeenCircled.size(); i++)     //search for the vertex in haveBeenCircled that has that coordinate
			{
				if (haveBeenCircled[i].P == prev)
					p = haveBeenCircled[i];
			}
		}
		//cerr << p.P.latitudeText << "  " << p.P.longitudeText << endl;
		return DELIVERY_SUCCESS;
	}


}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
