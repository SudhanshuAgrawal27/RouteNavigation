#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif

//
//#include "provided.h"
//#include "ExpandableHashMap.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
////#include "support.h"
//#include <queue>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//	GeoCoord s("0.0", "0.0");
//
//	GeoCoord g1("4.0", "3.0");
//
//	GeoCoord g2("5.0", "-5.0");
//
//	vector<DeliveryRequest> dr;
//	DeliveryRequest d1("Item1", g1);
//	DeliveryRequest d2("Item2", g2);
//	DeliveryRequest d3("Item3", g2);
//	dr.push_back(d3);
//	dr.push_back(d1);
//	dr.push_back(d2);
//
//
//	StreetMap sm;
//	sm.load("datat2.txt");
//	DeliveryPlanner dp(&sm);	
//	vector<DeliveryCommand> dcs;
//	double td;
//
//
//	DeliveryResult res = dp.generateDeliveryPlan(s, dr, dcs, td);
//	if (res == DELIVERY_SUCCESS)
//	{
//		for (int i = 0; i < dcs.size(); ++i)
//		{
//			cerr << dcs[i].description() << endl;
//		}
//	}
//	else if (res == NO_ROUTE)
//		cerr << "no route" << endl;
//	else
//		cerr << "Bad coord" << endl;
//
//
//
//	
//
//}

//int main()
//{
//StreetMap smap;


//	smap.load("datat2.txt");
//	DeliveryOptimizer opt(&smap);
//
//	GeoCoord s("0.0", "0.0");
//	GeoCoord d1("4.0", "3.0");
//	GeoCoord d2("8.0", "0.0");
//	GeoCoord d3("5.0", "-5.0");
//	DeliveryRequest D1("I1", d1);
//	DeliveryRequest D2("I2", d2);
//	DeliveryRequest D3("I3", d3);
//	vector<DeliveryRequest> dr;
//
//	dr.push_back(D1);
//	dr.push_back(D3);
//	dr.push_back(D2);
//
//	cerr << "Before: " << endl;
//	for (int i = 0; i < dr.size(); i++)
//		cerr << dr[i].item << endl;
//
//	double od, nd;
//
//	opt.optimizeDeliveryOrder(s, dr, od, nd);
//
//	cerr << "After: " << endl;
//	for (int i = 0; i < dr.size(); i++)
//		cerr << dr[i].item << endl;
//}


//int main()
//{
//	StreetMap smap;

	//datat3
	/*smap.load("datat3.txt");

	PointToPointRouter p(&smap);

	GeoCoord s("0.0", "0.0");

	GeoCoord e("4.0", "2.5");*/



	//datat1
	/*smap.load("datat1.txt");

	PointToPointRouter p(&smap);

	GeoCoord s("0.0", "0.0");
	GeoCoord e("-2.0", "2.0");*/

	

	//datat2
	/*smap.load("datat2.txt");

	PointToPointRouter p(&smap);

	GeoCoord s("0.0", "0.0");
	GeoCoord e("5.0", "-5.0");*/


	//double tD;
	//list<StreetSegment> sts;

	//DeliveryResult res = p.generatePointToPointRoute(s, e, sts, tD);
	//if (res == DELIVERY_SUCCESS)
	//{
	//	cerr << "Path found" << endl;
	//	for (list<StreetSegment>::iterator it = sts.begin(); it != sts.end(); ++it)
	//	{
	//		cerr << it->name << endl;
	//	}
	//	cerr << "Distance = " << tD << endl;
	//}
	//else if (res == NO_ROUTE)
	//	cerr << "No Route found" << endl;
	//else if(res == BAD_COORD)
	//	cerr << "Bad coord" << endl;



	/*
	StreetMap smap;
	smap.load("data2.txt");
 

	GeoCoord a("34.0555267", "-118.4796954");    //possible move 1
	GeoCoord b("34.0555356", "-118.4798135");    //start
	GeoCoord c("34.0554131", "-118.4804510");    //possible move 2
	//GeoCoord e("34.0591176", "-118.4822040");      //arbitrary end 
	GeoCoord e("34.0554131", "-118.4804510");    //arbitrary end that has a path to it

	*/



	/*Vertex v1(a, c);
	Vertex v2(b, v1, st, c);

	cout << v1.D << endl;
	cout << v1.L << endl;
	cout << v1.H << endl;
	cout << v1.P.latitude << "  " << v1.P.longitude << endl;
	cout << endl;
	cout << v2.D << endl;
	cout << v2.L << endl;
	cout << v2.H << endl;
	cout << v2.P.latitude << "  " << v2.P.longitude << endl;

	cout << endl;
	priority_queue<Vertex> q;
	q.push(v1);
	q.push(v2);
	cout << q.top().D;*/

	//StreetMap s;
	/*s.load("data.txt");
	GeoCoord g("34.0555356", "-118.4798135");
	vector<StreetSegment> segs;
	s.getSegmentsThatStartWith(g, segs);
	cout << "Segs size = " << segs.size() << endl;*/

	
//}





#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);

int main(int argc, char* argv[])
{/*
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
		return 1;
	}*/

	StreetMap sm;

	if (!sm.load("mapdata.txt"))
	{
		cout << "Unable to load map data file "<< endl;
		return 1;
	}

	GeoCoord depot;
	vector<DeliveryRequest> deliveries;
	if (!loadDeliveryRequests("deliveries.txt", depot, deliveries))
	{
		cout << "Unable to load delivery request file " << endl;
		return 1;
	}

	cout << "Generating route...\n\n";

	DeliveryPlanner dp(&sm);
	vector<DeliveryCommand> dcs;
	double totalMiles;
	DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
	if (result == BAD_COORD)
	{
		cout << "One or more depot or delivery coordinates are invalid." << endl;
		return 1;
	}
	if (result == NO_ROUTE)
	{
		cout << "No route can be found to deliver all items." << endl;
		return 1;
	}
	cout << "Starting at the depot...\n";
	for (const auto& dc : dcs)
		cout << dc.description() << endl;
	cout << "You are back at the depot and your deliveries are done!\n";
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
	ifstream inf(deliveriesFile);
	if (!inf)
		return false;
	string lat;
	string lon;
	inf >> lat >> lon;
	inf.ignore(10000, '\n');
	depot = GeoCoord(lat, lon);
	string line;
	while (getline(inf, line))
	{
		string item;
		if (parseDelivery(line, lat, lon, item))
			v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
	}
	return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
	const size_t colon = line.find(':');
	if (colon == string::npos)
	{
		cout << "Missing colon in deliveries file line: " << line << endl;
		return false;
	}
	istringstream iss(line.substr(0, colon));
	if (!(iss >> lat >> lon))
	{
		cout << "Bad format in deliveries file line: " << line << endl;
		return false;
	}
	item = line.substr(colon + 1);
	if (item.empty())
	{
		cout << "Missing item in deliveries file line: " << line << endl;
		return false;
	}
	return true;
}

