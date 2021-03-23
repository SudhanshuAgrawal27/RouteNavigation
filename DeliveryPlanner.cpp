#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m_map;
    string whichDir(const double angle) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    m_map = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}


DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    totalDistanceTravelled = 0;
    commands.clear();

    vector<DeliveryRequest> dr;      //deliveries is const so I need another vector to reorder? 
    for (int i = 0; i < deliveries.size(); i++)
        dr.push_back(deliveries[i]);

    DeliveryOptimizer opt(m_map);
    double ocd, ncd;
    opt.optimizeDeliveryOrder(depot, dr, ocd, ncd);

    //deliveries are now optimized
    //cerr << "Deliveries are now optimized" << endl;

    PointToPointRouter ppr(m_map);              //create a point to point router for the map
    vector<list<StreetSegment>> routes;         //vector of all the point to point routes for each of the steps
    
    list<StreetSegment> route;                  //list of point to point routes for a given stretch of the journey
    double tD;

    //for depot to first spot
    DeliveryResult res = ppr.generatePointToPointRoute(depot, dr[0].location, route, tD);
    if (res == DELIVERY_SUCCESS)
        routes.push_back(route);
    else                            //if there was a bad start coordinate or no route existed
        return res;

    //for in between deliveries
    for (int i = 0; i < dr.size() - 1; i++)
    {
        res = ppr.generatePointToPointRoute(dr[i].location, dr[i + 1].location, route, tD);
        if (res == DELIVERY_SUCCESS)
            routes.push_back(route);
        else                        //if there was a bad start coordinate or no route existed
            return res;
    }

    //for last spot to depot
    res = ppr.generatePointToPointRoute(dr[dr.size() - 1].location, depot, route, tD);
    if (res == DELIVERY_SUCCESS)
        routes.push_back(route);
    else                            //if there was a bad start coordinate or no route existed
        return res;

    //routes now contains point to point routes for each stretch of the journey
    //cerr << "Routes now contain point to point routes" << endl;

    for (int i = 0; i < routes.size(); i++)
    {
        list<StreetSegment> rt = routes[i];   //for the given route from Depot/Delivery location to Delivery location/Depot
        for (list<StreetSegment>::iterator it = rt.begin(); it != rt.end(); ++it)
        {
            double angle = angleOfLine(*it); // find the angle of the first line segment, since it will always be used
            string dir = whichDir(angle);    // find the direction it points in
            string nm = it->name;            //find its name
            double travelDist = distanceEarthMiles(it->end, it->start);   //find its length

            //start with the next vertex 
            it++;
            while (it != rt.end() && (it->name == nm)) //keep iterating as long as the name is the same
            {
                travelDist += distanceEarthMiles(it->end, it->start);
                ++it;
            }

            DeliveryCommand pc;
            pc.initAsProceedCommand(dir, nm, travelDist); 
            commands.push_back(pc);

            totalDistanceTravelled += travelDist;

            if (it == rt.end())          //so we don't execute the other statements
                break;


            //at this point we know that the given street is done, and we're on a new street
            //so we need to turn if necessary

            list<StreetSegment>::iterator ittemp = --it;                   //pointer to previous line segment
            ++it;                                                          //bring it back to current line segment

            double angleBetween = angleBetween2Lines(*(ittemp), *(it));   //angle between the previous one and the current one

            if (angleBetween < 1 || angleBetween >359)
                continue;                                   //go to the next move which will involve the new street
            if (angleBetween >= 1 && angleBetween < 180)
            {
                DeliveryCommand tc;
                tc.initAsTurnCommand("left", it->name);
                commands.push_back(tc);
            }
            if(angleBetween >=180 && angleBetween <=359)
            {
                DeliveryCommand tc;
                tc.initAsTurnCommand("right", it->name);
                commands.push_back(tc);
            }
            //at this point it is on the next street that we need to convert
            //but ++it will increase it again
            //so let's decrease it
            --it;

        }

        //at this point we're done with a given route so we need to deliver

        if (i != (routes.size() - 1))    //the last route is the route back to the depot so there's nothing to deliver
        {
            DeliveryCommand dc;
            dc.initAsDeliverCommand(dr[i].item);
            commands.push_back(dc);
        }

    }

    return DELIVERY_SUCCESS;
}

string DeliveryPlannerImpl::whichDir(const double angle) const
{
    if (0 <= angle && angle < 22.5)
        return "east";
    else if (22.5 <= angle && angle < 67.5)
        return "northeast";
    else if (67.5 <= angle && angle < 112.5)
        return "north";
    else if (112.5 <= angle && angle < 157.5)
        return "northwest";
    else if (157.5 <= angle && angle < 202.5)
        return "west";
    else if (202.5 <= angle && angle < 247.5)
        return "southwest";
    else if (247.5 <= angle && angle < 292.5)
        return "south";
    else if (292.5 <= angle && angle < 337.5)
        return "southeast";
    else if (angle >= 337.5)
        return "east";

    return "I should never get here";
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
