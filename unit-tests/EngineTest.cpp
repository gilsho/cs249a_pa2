#include "gtest/gtest.h"
#include <fstream>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include "Engine.h"

using namespace Shipping;

using std::cout;
using std::endl;

TEST(Network, NotifieeTest) 
{
  class Reactor : public Network::Notifiee {
  public:
    Reactor() { segments = 0; locations = 0;}
    virtual void onSegmentNew(Segment::Ptr seg) {
      segments++;
    }

    virtual void onLocationNew(Location::Ptr loc) {
      locations++;
    } 

    virtual void onLocationDel(Location::Ptr loc) {
      locations--;
    }

    virtual void onSegmentDel(Segment::Ptr seg) {
      segments--;
    }

    int segments;
    int locations;
  };

  Network::Ptr net = Network::NetworkIs("network");
  Reactor * rec = new Reactor();
  rec->notifierIs(net);

  Location::Ptr loc1 = Location::LocationIs("sf");
  Location::Ptr loc2 = Location::LocationIs("paris");
  Segment::Ptr seg1 = TruckSegment::TruckSegmentIs("280");
  Segment::Ptr seg2 = TruckSegment::TruckSegmentIs("101");

  ASSERT_TRUE(rec->locations == 0);
  ASSERT_TRUE(rec->segments == 0);

  net->locationIs(loc1);
  ASSERT_TRUE(rec->locations == 1);
  ASSERT_TRUE(rec->segments == 0);

  net->locationIs(loc2);
  ASSERT_TRUE(rec->locations == 2);
  ASSERT_TRUE(rec->segments == 0);

  net->segmentIs(seg1);
  ASSERT_TRUE(rec->locations == 2);
  ASSERT_TRUE(rec->segments == 1);

  //check idempotency
  net->locationIs(loc2);
  net->segmentIs(seg1);
  ASSERT_TRUE(rec->locations == 2);
  ASSERT_TRUE(rec->segments == 1);

  net->segmentIs(seg2);
  ASSERT_TRUE(rec->locations == 2);
  ASSERT_TRUE(rec->segments == 2);


  //check deletion
  net->locationDel(loc1->name());
  ASSERT_TRUE(rec->locations == 1);
  ASSERT_TRUE(rec->segments == 2);

  net->segmentDel(seg1->name());
  ASSERT_TRUE(rec->locations == 1);
  ASSERT_TRUE(rec->segments == 1);

  //idempotency
  net->segmentDel(seg1->name());
  ASSERT_TRUE(rec->locations == 1);
  ASSERT_TRUE(rec->segments == 1);

  net->locationDel(loc2->name());
  ASSERT_TRUE(rec->locations == 0);
  ASSERT_TRUE(rec->segments == 1);

  net->segmentDel(seg2->name());
  ASSERT_TRUE(rec->locations == 0);
  ASSERT_TRUE(rec->segments == 0);
}

TEST(Network, BoatFleet)
{
  Network::Ptr net = Network::NetworkIs("network");
  BoatFleetDesc::Ptr b  = BoatFleetDesc::BoatFleetDescIs();
  net->boatFleetIs(b);
  ASSERT_TRUE(net->boatFleet() == b);
}

TEST(Network, TruckFleet)
{
  Network::Ptr net = Network::NetworkIs("network");
  TruckFleetDesc::Ptr t  = TruckFleetDesc::TruckFleetDescIs();
  net->truckFleetIs(t);
  ASSERT_TRUE(net->truckFleet() == t);
}

TEST(Network, PlaneFleet)
{
  Network::Ptr net = Network::NetworkIs("network");
  PlaneFleetDesc::Ptr p  = PlaneFleetDesc::PlaneFleetDescIs();
  net->planeFleetIs(p);
  ASSERT_TRUE(net->planeFleet() == p);
}

TEST(Network, Fleet)
{
  Network::Ptr net = Network::NetworkIs("network");
  BoatFleetDesc::Ptr b  = BoatFleetDesc::BoatFleetDescIs();
  PlaneFleetDesc::Ptr p = PlaneFleetDesc::PlaneFleetDescIs();
  TruckFleetDesc::Ptr t = TruckFleetDesc::TruckFleetDescIs();

  net->boatFleetIs(b);
  net->planeFleetIs(p);
  net->truckFleetIs(t);

  ASSERT_TRUE(net->fleet(Truck) == t);
  ASSERT_TRUE(net->fleet(Boat) == b);
  ASSERT_TRUE(net->fleet(Plane) == p);

  ASSERT_TRUE(net->fleet(Truck) != b);
  ASSERT_TRUE(net->fleet(Boat) != p);
  ASSERT_TRUE(net->fleet(Plane) != t);
}

TEST(Engine, InstanceOf) 
{
  Location::Ptr loc = Location::LocationIs("sf");

  ASSERT_TRUE(INSTANCE_OF(loc.ptr(), Location*));
  ASSERT_TRUE(!INSTANCE_OF(loc.ptr(), Segment*));
}

TEST(Stats, onLocationNew)
{
  Network::Ptr net = Network::NetworkIs("net"); 
  Stats::Ptr stats = StatsConfig::StatsConfigIs(net);

  ASSERT_TRUE(stats->customerLocations() == 0);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);
  
  CustomerLocation::Ptr loc1 = CustomerLocation::CustomerLocationIs("cloc1");
  net->locationIs(loc1);
  ASSERT_TRUE(stats->customerLocations() == 1);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);

  CustomerLocation::Ptr loc2 = CustomerLocation::CustomerLocationIs("cloc2");
  net->locationIs(loc2);
  ASSERT_TRUE(stats->customerLocations() == 2);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);

  Port::Ptr loc3 = Port::PortIs("ploc1");
  net->locationIs(loc3);
  ASSERT_TRUE(stats->customerLocations() == 2);
  ASSERT_TRUE(stats->ports() == 1);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);
}

TEST(Stats, onSegmentNew) 
{
  Network::Ptr net = Network::NetworkIs("net"); 
  Stats::Ptr stats = StatsConfig::StatsConfigIs(net);

  TruckSegment::Ptr seg1 = TruckSegment::TruckSegmentIs("tseg1");
  net->segmentIs(seg1);
  ASSERT_TRUE(stats->customerLocations() == 0);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 1);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);

  BoatSegment::Ptr seg2 = BoatSegment::BoatSegmentIs("bseg1");
  net->segmentIs(seg2);
  ASSERT_TRUE(stats->customerLocations() == 0);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 1);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 1);
}

TEST(Stats, onExpedite) 
{
  Network::Ptr net = Network::NetworkIs("net"); 
  Stats::Ptr stats = StatsConfig::StatsConfigIs(net);

  ExpediteOptions val1 = ExpediteNotSupported;
  ExpediteOptions val2 = ExpediteSupported;

  TruckSegment::Ptr seg1 = TruckSegment::TruckSegmentIs("tseg1");
  TruckSegment::Ptr seg2 = TruckSegment::TruckSegmentIs("tseg2");


  seg1->expediteIs(val1);
  ASSERT_TRUE(stats->expedited() == 0);

  net->segmentIs(seg1);
  ASSERT_TRUE(stats->truckSegments() == 1);
  ASSERT_TRUE(stats->expedited() == 0);

  seg1->expediteIs(val2);
  ASSERT_TRUE(stats->truckSegments() == 1);
  ASSERT_TRUE(stats->expedited() == 1);  

  net->segmentIs(seg2);
  seg2->expediteIs(val2);

  ASSERT_TRUE(stats->truckSegments() == 2);
  ASSERT_TRUE(stats->expedited() == 2); 

  seg1->expediteIs(val1);
  ASSERT_TRUE(stats->truckSegments() == 2);
  ASSERT_TRUE(stats->expedited() == 1); 

  seg2->expediteIs(val1);
  ASSERT_TRUE(stats->truckSegments() == 2);
  ASSERT_TRUE(stats->expedited() == 0);
}

TEST(Stats, onLocationDel)
{
  Network::Ptr net = Network::NetworkIs("net"); 
  Stats::Ptr stats = StatsConfig::StatsConfigIs(net);
  
  CustomerLocation::Ptr loc1 = CustomerLocation::CustomerLocationIs("cloc1");
  net->locationIs(loc1);

  CustomerLocation::Ptr loc2 = CustomerLocation::CustomerLocationIs("cloc2");
  net->locationIs(loc2);

  Port::Ptr loc3 = Port::PortIs("ploc1");
  net->locationIs(loc3);
  ASSERT_TRUE(stats->customerLocations() == 2);
  ASSERT_TRUE(stats->ports() == 1);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);

  net->locationDel(loc1->name());
  ASSERT_TRUE(stats->customerLocations() == 1);
  ASSERT_TRUE(stats->ports() == 1);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);

  net->locationDel(loc2->name());
  ASSERT_TRUE(stats->customerLocations() == 0);
  ASSERT_TRUE(stats->ports() == 1);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);

  net->locationDel(loc3->name());
  ASSERT_TRUE(stats->customerLocations() == 0);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);
}

TEST(Stats, onSegmentDel) 
{
  Network::Ptr net = Network::NetworkIs("net"); 
  Stats::Ptr stats = StatsConfig::StatsConfigIs(net);

  TruckSegment::Ptr seg1 = TruckSegment::TruckSegmentIs("tseg1");
  net->segmentIs(seg1);

  BoatSegment::Ptr seg2 = BoatSegment::BoatSegmentIs("bseg1");
  net->segmentIs(seg2);


  net->segmentDel(seg1->name());
  ASSERT_TRUE(stats->customerLocations() == 0);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 1);

  net->segmentDel(seg2->name());
  ASSERT_TRUE(stats->customerLocations() == 0);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);
}

TEST(FleetDesc, speed) 
{
  FleetDesc::Ptr fleet = BoatFleetDesc::BoatFleetDescIs();
  fleet->speedIs(5);
  ASSERT_TRUE(fleet->speed() == 5);
}

TEST(FleetDesc, capacity) 
{
  FleetDesc::Ptr fleet = BoatFleetDesc::BoatFleetDescIs();
  fleet->capacityIs(25);
  ASSERT_TRUE(fleet->capacity() == 25);
}

TEST(FleetDesc, costPerMile) 
{
  FleetDesc::Ptr fleet = BoatFleetDesc::BoatFleetDescIs();
  fleet->costPerMileIs(210);
  ASSERT_TRUE(fleet->costPerMile() == 210);
}


//test stats delete!!!









