//#ifndef SUPPORT_H
//#define SUPPORT_H
//
//#include "provided.h"
//#include <cmath>
//class Vertex
//{
//public:
//	GeoCoord P;		//Position of the vertex
//	double L;		//Label = shortest distance to that point
//	double H;		//Heuristic measure
//
//	double D;		//Euclidian distance to target
//	GeoCoord prev;  //the previous vertex it came from
//	StreetSegment cameThrough;
//
//	bool isStart;   //if isStart == true, don't check prev, cameThrough
//
//	Vertex(GeoCoord pos, GeoCoord ending)
//	{
//		isStart = true;
//		P = pos;
//		L = 0;
//		D = distanceEarthMiles(ending, P);
//		H = D + L;
//	}
//
//	Vertex(GeoCoord pos, Vertex pr, StreetSegment cT, GeoCoord ending)
//	{
//		isStart = false;
//
//		P = pos;
//		prev = pr.P;
//		cameThrough = cT;
//			
//		//since I only create a vertex if it's next to me, I don't have to have the infinity initialization
//		L = distanceEarthMiles(pr.P, pos) + pr.L;
//
//		D = distanceEarthMiles(ending, P);
//
//		H = D + L;
//	}
//
//	bool operator==(const Vertex& other) const   //check if two vertices are the same
//	{
//		if (P.latitude == other.P.latitude && P.longitude == other.P.longitude)
//			return true;
//		return false;
//	}
//
//	bool operator==(const GeoCoord& other) const  //check if two vertices are the same by passing in a coordinate
//	{
//		if (P.latitude == other.latitude && P.longitude == other.longitude)
//			return true;
//		return false;
//	}
//
//	bool operator<(const Vertex& other) const
//	{
//		return H > other.H;         //we're going to use this in a priority queue.
//	}							    //so v1 > v2 (more priority) if v1.H < v2.H
//							        //so reverse them
//	bool operator!=(const GeoCoord& other) const
//	{
//		return !(*this == other);
//
//	}
//
//};
//
//#endif
