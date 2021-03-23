#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* m_map;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    m_map = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = 0;
    newCrowDistance = 0;

    if (deliveries.empty())
        return;

    oldCrowDistance += distanceEarthMiles(depot, deliveries[0].location);

    for (int i = 0; i < deliveries.size() - 1; i++)
    {
        oldCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
    }

    oldCrowDistance += distanceEarthMiles(depot, deliveries[deliveries.size() - 1].location);


    //let's start with two vertices,
    //the starting vertex and the vertex of the delivery that's initially last
    //and have a group that each vertex belongs to

    GeoCoord v1 = depot;                                         vector<DeliveryRequest> v1d;
    GeoCoord v2 = deliveries[deliveries.size() - 1].location;    vector<DeliveryRequest> v2d;

    //we go through all the other vertices and place it in the group which it is closer to
    for (int i = 0; i < deliveries.size(); i++)
    {
        double dist1 = distanceEarthMiles(v1, deliveries[i].location);
        double dist2 = distanceEarthMiles(v2, deliveries[i].location);

        dist1 < dist2 ? v1d.push_back(deliveries[i]) : v2d.push_back(deliveries[i]);

    }
    deliveries.clear();
    for (int i = 0; i < v1d.size() + v2d.size(); ++i)
    {
        if (i < v1d.size())
        {
            deliveries.push_back(v1d[i]);
        }
        else
        {
            deliveries.push_back(v2d[i - v1d.size()]);
        }
    }

    newCrowDistance += distanceEarthMiles(depot, deliveries[0].location);

    for (int i = 0; i < deliveries.size() - 1; i++)
    {
       newCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
    }

    newCrowDistance += distanceEarthMiles(depot, deliveries[deliveries.size() - 1].location);

}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
