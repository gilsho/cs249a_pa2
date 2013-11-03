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

  Location::Ptr loc1 = Customer::CustomerIs("sf");
  Location::Ptr loc2 = Customer::CustomerIs("paris");
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

  ASSERT_TRUE(net->fleet(TruckMode) == t);
  ASSERT_TRUE(net->fleet(BoatMode) == b);
  ASSERT_TRUE(net->fleet(PlaneMode) == p);

  ASSERT_TRUE(net->fleet(TruckMode) != b);
  ASSERT_TRUE(net->fleet(BoatMode) != p);
  ASSERT_TRUE(net->fleet(PlaneMode) != t);
}

TEST(Engine, InstanceOf) 
{
  Location::Ptr loc = Customer::CustomerIs("sf");

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
  
  Customer::Ptr loc1 = Customer::CustomerIs("cloc1");
  net->locationIs(loc1);
  ASSERT_TRUE(stats->customerLocations() == 1);
  ASSERT_TRUE(stats->ports() == 0);
  ASSERT_TRUE(stats->truckSegments() == 0);
  ASSERT_TRUE(stats->planeSegments() == 0);
  ASSERT_TRUE(stats->boatSegments() == 0);

  Customer::Ptr loc2 = Customer::CustomerIs("cloc2");
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
  
  Customer::Ptr loc1 = Customer::CustomerIs("cloc1");
  net->locationIs(loc1);

  Customer::Ptr loc2 = Customer::CustomerIs("cloc2");
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

TEST(Path, segmentNew)
{

  FleetDesc::Ptr fleet = FleetDesc::FleetDescIs();
  fleet->speedIs(5);
  fleet->costPerMileIs(10);

  Location::Ptr loc1 = Customer::CustomerIs("pa");
  Location::Ptr loc2 = Port::PortIs("redwood");
  Location::Ptr loc3 = BoatTerminal::BoatTerminalIs("sf");

  Segment::Ptr seg1 = TruckSegment::TruckSegmentIs("280");
  seg1->sourceIs(loc1);
  seg1->destinationIs(loc2);
  seg1->difficultyIs(2.0);
  seg1->lengthIs(15);

  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("bay");
  seg2->sourceIs(loc2);
  seg2->destinationIs(loc3);
  seg2->difficultyIs(3.0);
  seg2->lengthIs(25);

  Segment::Ptr seg3 = PlaneSegment::PlaneSegmentIs("skyline");
  seg3->sourceIs(loc2);
  seg3->destinationIs(loc1);

  Path::Ptr p = Path::PathIs();
  p->segmentNew(seg1, fleet);
  p->segmentNew(seg2, fleet);
  p->segmentNew(seg3, fleet);

  ASSERT_TRUE(p->segments() == 2);

  Path::SegmentIterator it = p->segmentIter();
  ASSERT_TRUE(*it == seg1); 
  it++;
  ASSERT_TRUE(*it == seg2);

  ASSERT_TRUE(p->cost() == 1050);
  ASSERT_TRUE(p->length() == 40);
  ASSERT_TRUE(p->time() == 8);
}

TEST(Path, ExpediteSupported) 
{
}













