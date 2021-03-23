#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include "ExpandableHashMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;

private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_map;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	ifstream infile(mapFile);  
	if (!infile)		        // Did opening the file fail?
	{
		return false;
	}

	string line;
	while (getline(infile, line))
	{
		string sname = line;
		//cerr << sname << endl;

		int nPieces;
		infile >> nPieces;
		infile.ignore(10000, '\n');    //go to the next line

		//cerr << nPieces << endl;

		for (int i = 1; i <= nPieces; ++i)
		{
			string x1, y1, x2, y2;
			infile >> x1 >> y1 >> x2 >> y2;

			infile.ignore(10000, '\n');   //go to the next line

			//cerr << x1 << ", " << y1 << "  " << x2 << ", " << y2 << endl;

			vector<StreetSegment>* vecPtr;

			GeoCoord B(x1, y1);
			GeoCoord E(x2, y2);

			StreetSegment S(B, E, sname);        //create the street 
			StreetSegment R(E, B, sname);        //create the reverse street

			vecPtr = m_map.find(B);                //get a pointer to the vector mapped to B
			if (vecPtr == nullptr)                 //if the coord is not yet in the map
			{
				vector<StreetSegment> emptyVal;
				m_map.associate(B, emptyVal);     // create an empty vector for it
			}
			vecPtr = m_map.find(B);               
			(*vecPtr).push_back(S);               //add the Street Segment to that vector


			vecPtr = m_map.find(E);
			if (vecPtr == nullptr)
			{
				vector<StreetSegment> emptyVal;
				m_map.associate(E, emptyVal);
			}
			vecPtr = m_map.find(E);
			(*vecPtr).push_back(R);

		}

	}
    
	return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{

	const vector<StreetSegment>* vecPtr;
	vecPtr = m_map.find(gc);

	if (vecPtr == nullptr)      //if that coordinate isn't in the map
	{
		return false;
	}

	segs = *vecPtr;
	return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
