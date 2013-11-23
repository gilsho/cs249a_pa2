#include "gtest/gtest.h"
#include <stdlib.h>
#include <queue>
#include <iostream>
#include "Engine.h"
#include "Activity.h"
#include "ActivityImpl.h"


using namespace Shipping;

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;


Network::Ptr prepareNetwork1() 
{
  Network::Ptr net = Network::NetworkIs("net1");

  TruckFleetDesc::Ptr f1 = TruckFleetDesc::TruckFleetDescIs();
  BoatFleetDesc::Ptr f2 = BoatFleetDesc::BoatFleetDescIs();
  PlaneFleetDesc::Ptr f3 = PlaneFleetDesc::PlaneFleetDescIs();

  net->truckFleetIs(f1);
  net->boatFleetIs(f2);
  net->planeFleetIs(f3);

  f1->speedIs(5);
  f2->speedIs(6);
  f3->speedIs(7);

  f1->costPerMileIs(10);
  f2->costPerMileIs(20);
  f3->costPerMileIs(30);

  Location::Ptr loc1 = Customer::CustomerIs("cent");
  Location::Ptr loc2 = Port::PortIs("r1");
  Location::Ptr loc3 = Customer::CustomerIs("c1");
  Location::Ptr loc4 = TruckTerminal::TruckTerminalIs("t2");
  Location::Ptr loc5 = Port::PortIs("r2");
  Location::Ptr loc6 = BoatTerminal::BoatTerminalIs("b3");
  Location::Ptr loc7 = BoatTerminal::BoatTerminalIs("bb3");

  net->locationIs(loc1);
  net->locationIs(loc2);
  net->locationIs(loc3);
  net->locationIs(loc4);
  net->locationIs(loc5);
  net->locationIs(loc6);

  Segment::Ptr seg1 = PlaneSegment::PlaneSegmentIs("pseg1");
  Segment::Ptr seg11 = PlaneSegment::PlaneSegmentIs("pseg1-ret");
  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("pseg2");
  Segment::Ptr seg22 = BoatSegment::BoatSegmentIs("pseg2-ret");
  Segment::Ptr seg3 = BoatSegment::BoatSegmentIs("bseg1");
  Segment::Ptr seg33 = BoatSegment::BoatSegmentIs("bseg1-ret");
  Segment::Ptr seg4 = BoatSegment::BoatSegmentIs("bseg2");
  Segment::Ptr seg44 = BoatSegment::BoatSegmentIs("bseg2-ret");
  Segment::Ptr seg5 = TruckSegment::TruckSegmentIs("tseg1");
  Segment::Ptr seg55 = TruckSegment::TruckSegmentIs("tseg1-ret");
  Segment::Ptr seg6 = TruckSegment::TruckSegmentIs("tseg2");
  Segment::Ptr seg66 = TruckSegment::TruckSegmentIs("tseg2-ret");

  seg1->sourceIs(loc1); 
  seg11->sourceIs(loc2);
  seg2->sourceIs(loc2); 
  seg22->sourceIs(loc3);
  seg3->sourceIs(loc1); 
  seg33->sourceIs(loc6);
  seg4->sourceIs(loc6); 
  seg44->sourceIs(loc7);
  seg5->sourceIs(loc1); 
  seg55->sourceIs(loc4);
  seg6->sourceIs(loc4); 
  seg66->sourceIs(loc5);

  seg1->returnSegmentIs(seg11);
  seg22->returnSegmentIs(seg2);
  seg3->returnSegmentIs(seg33);
  seg4->returnSegmentIs(seg44);
  seg55->returnSegmentIs(seg5);
  seg6->returnSegmentIs(seg66);


  net->segmentIs(seg1);
  net->segmentIs(seg2);
  net->segmentIs(seg3);
  net->segmentIs(seg4);
  net->segmentIs(seg5);
  net->segmentIs(seg6);

  net->segmentIs(seg11);
  net->segmentIs(seg22);
  net->segmentIs(seg33);
  net->segmentIs(seg44);
  net->segmentIs(seg55);
  net->segmentIs(seg66);  

  seg1->lengthIs(100);
  seg2->lengthIs(200);
  seg3->lengthIs(300);
  seg4->lengthIs(400);
  seg5->lengthIs(500);
  seg6->lengthIs(600);

  seg11->lengthIs(100);
  seg22->lengthIs(200);
  seg33->lengthIs(300);
  seg44->lengthIs(400);
  seg55->lengthIs(500);
  seg66->lengthIs(600);

  seg1->difficultyIs(1.0);
  seg2->difficultyIs(2.0);
  seg3->difficultyIs(1.0);
  seg4->difficultyIs(3.0);
  seg5->difficultyIs(1.0);
  seg6->difficultyIs(4.0);

  seg11->difficultyIs(1.0);
  seg22->difficultyIs(2.0);
  seg33->difficultyIs(1.0);
  seg44->difficultyIs(3.0);
  seg55->difficultyIs(1.0);
  seg66->difficultyIs(4.0);

  seg1->expediteIs(ExpediteSupported);
  seg2->expediteIs(ExpediteSupported);
  seg3->expediteIs(ExpediteNotSupported);
  seg4->expediteIs(ExpediteSupported);
  seg5->expediteIs(ExpediteNotSupported);
  seg6->expediteIs(ExpediteNotSupported);

  seg11->expediteIs(ExpediteSupported);
  seg22->expediteIs(ExpediteSupported);
  seg33->expediteIs(ExpediteNotSupported);
  seg44->expediteIs(ExpediteSupported);
  seg55->expediteIs(ExpediteNotSupported);
  seg66->expediteIs(ExpediteNotSupported);

  return net;
}

TEST(Location, name)
{
  Location::Ptr loc1 = Customer::CustomerIs("loc1");
  Location::Ptr loc2 = Customer::CustomerIs("loc2");
  ASSERT_TRUE(loc1->name() == "loc1");
  ASSERT_TRUE(loc2->name() == "loc2");
}

TEST(Location, segmentIs)
{
  Location::Ptr loc1 = Customer::CustomerIs("loc1");
  Segment::Ptr seg = BoatSegment::BoatSegmentIs("seg");
  seg->sourceIs(loc1);
  ASSERT_TRUE(loc1->segment(0) == seg);
  ASSERT_TRUE(loc1->segments() == 1);
}

TEST(Location, segmentDel)
{
  Location::Ptr loc = Customer::CustomerIs("loc");
  Segment::Ptr seg0 = BoatSegment::BoatSegmentIs("seg0");
  Segment::Ptr seg1 = PlaneSegment::PlaneSegmentIs("seg1");
  Segment::Ptr seg2 = PlaneSegment::PlaneSegmentIs("seg2");
  Segment::Ptr seg3 = PlaneSegment::PlaneSegmentIs("seg3");
  Segment::Ptr seg4 = PlaneSegment::PlaneSegmentIs("seg4");
  Segment::Ptr seg5 = PlaneSegment::PlaneSegmentIs("seg5");

  seg0->sourceIs(loc);
  seg1->sourceIs(loc);
  seg2->sourceIs(loc);
  seg3->sourceIs(loc);
  seg4->sourceIs(loc);
  seg5->sourceIs(loc);

  ASSERT_TRUE(loc->segment(0) == seg0);
  ASSERT_TRUE(loc->segment(1) == seg1);
  ASSERT_TRUE(loc->segment(2) == seg2);
  ASSERT_TRUE(loc->segment(3) == seg3);
  ASSERT_TRUE(loc->segment(4) == seg4);

  loc->segmentDel(seg0);
  loc->segmentDel(seg2);
  loc->segmentDel(seg3);

  ASSERT_TRUE(loc->segment(0) == seg1);
  ASSERT_TRUE(loc->segment(1) == seg4);
  ASSERT_TRUE(loc->segment(2) == seg5);
}

TEST(Location, segmentDel_consistency)
{
  Location::Ptr loc = Customer::CustomerIs("loc");
  Segment::Ptr seg = PlaneSegment::PlaneSegmentIs("seg1");

  seg->sourceIs(loc);
  ASSERT_TRUE(loc->segment(0) == seg);
  loc->segmentDel(seg);
  ASSERT_TRUE(seg->source() == NULL);
}

TEST(Segment, sourceIs) 
{
  Location::Ptr loc1 = Customer::CustomerIs("loc1");
  Location::Ptr loc2 = Customer::CustomerIs("loc2");
  Segment::Ptr seg = BoatSegment::BoatSegmentIs("seg");
  seg->sourceIs(loc1);
  ASSERT_TRUE(seg->source() == loc1);
}

TEST(Segment, sourceIs_consistency1) 
{
  Location::Ptr loc1 = Customer::CustomerIs("loc1");
  Location::Ptr loc2 = Customer::CustomerIs("loc2");
  Location::Ptr loc3 = Customer::CustomerIs("loc3");
  Segment::Ptr seg = BoatSegment::BoatSegmentIs("seg");
  seg->sourceIs(loc1);
  ASSERT_TRUE(seg->source() == loc1);
  ASSERT_TRUE(loc1->segment(0) == seg);
  seg->sourceIs(NULL);
  ASSERT_TRUE(seg->source() == NULL);
  ASSERT_TRUE(loc1->segment(0) == NULL);
  ASSERT_TRUE(loc1->segments() == 0);
}


TEST(Segment, destination) 
{
  Location::Ptr loc1 = Customer::CustomerIs("loc1");
  Location::Ptr loc2 = Customer::CustomerIs("loc2");
  Segment::Ptr seg1 = BoatSegment::BoatSegmentIs("seg1");
  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("seg2");
  seg1->sourceIs(loc1);
  seg2->sourceIs(loc2);
  seg1->returnSegmentIs(seg2);
  ASSERT_TRUE(seg1->destination() == loc2);
  ASSERT_TRUE(seg2->destination() == loc1);
}

TEST(Segment, returnSegment) 
{
  Location::Ptr loc1 = Customer::CustomerIs("loc1");
  Location::Ptr loc2 = Customer::CustomerIs("loc2");
  Segment::Ptr seg1 = BoatSegment::BoatSegmentIs("seg1");
  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("seg2");
  seg1->sourceIs(loc1);
  seg2->sourceIs(loc2);
  seg1->returnSegmentIs(seg2);
  ASSERT_TRUE(seg1->returnSegment() == seg2);
  ASSERT_TRUE(seg2->returnSegment() == seg1);
}

TEST(Segment, returnSegment_NULL) 
{
  Location::Ptr loc1 = Customer::CustomerIs("loc1");
  Location::Ptr loc2 = Customer::CustomerIs("loc2");
  Segment::Ptr seg1 = BoatSegment::BoatSegmentIs("seg1");
  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("seg2");
  seg1->sourceIs(loc1);
  seg2->sourceIs(loc2);
  seg1->returnSegmentIs(seg2);
  seg2->returnSegmentIs(NULL);
  ASSERT_TRUE(seg1->returnSegment() == NULL);
  ASSERT_TRUE(seg2->returnSegment() == NULL);
}

TEST(Segment, name)
{
  Segment::Ptr seg1 = BoatSegment::BoatSegmentIs("seg1");
  ASSERT_TRUE(seg1->name() == "seg1");
}

TEST(Segment, Expedite) 
{
  Segment::Ptr seg1 = BoatSegment::BoatSegmentIs("seg1");
  seg1->expediteIs(ExpediteSupported);
  ASSERT_TRUE(seg1->expedite() == ExpediteSupported);
  seg1->expediteIs(ExpediteNotSupported);
  ASSERT_TRUE(seg1->expedite() == ExpediteNotSupported);
}

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

TEST(Network, locationDel)
{

}

TEST(Network, segmentDel)
{
  Network::Ptr net = Network::NetworkIs("net");
  Location::Ptr loc1 = Port::PortIs("loc1");
  Location::Ptr loc2 = Port::PortIs("loc2");
  Segment::Ptr seg1 = BoatSegment::BoatSegmentIs("seg1");
  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("seg2");

  seg1->sourceIs(loc1);
  seg2->sourceIs(loc2);
  seg1->returnSegmentIs(seg2);

  net->locationIs(loc1);
  net->locationIs(loc2);
  net->segmentIs(seg1);
  net->segmentIs(seg2);

  net->segmentDel(seg1->name());
  ASSERT_TRUE(seg1->source() == NULL);
  ASSERT_TRUE(loc1->segments() == 0);
  ASSERT_TRUE(seg2->returnSegment() == NULL);
  ASSERT_TRUE(seg1->returnSegment() == NULL);

  net->segmentDel(seg2->name());
  ASSERT_TRUE(seg2->source() == NULL);
  ASSERT_TRUE(loc2->segments() == 0);

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

  Segment::Ptr seg1 = TruckSegment::TruckSegmentIs("280N");
  Segment::Ptr seg11 = TruckSegment::TruckSegmentIs("280S");
  seg1->sourceIs(loc1);
  seg11->sourceIs(loc2);
  seg1->returnSegmentIs(seg11);
  seg1->difficultyIs(2.0);
  seg1->lengthIs(15);

  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("bay");
  Segment::Ptr seg22 = BoatSegment::BoatSegmentIs("bay");
  seg2->sourceIs(loc2);
  seg22->sourceIs(loc3);
  seg2->returnSegmentIs(seg22);
  seg2->difficultyIs(3.0);
  seg2->lengthIs(25);

  Segment::Ptr seg3 = PlaneSegment::PlaneSegmentIs("skyline");
  Segment::Ptr seg33 = PlaneSegment::PlaneSegmentIs("skyline");
  seg3->sourceIs(loc2);
  seg33->sourceIs(loc1);
  seg3->returnSegmentIs(seg33);

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

  ASSERT_TRUE(p->start()->name() == "pa");
  ASSERT_TRUE(p->end()->name() == "sf");
}

TEST(Path, Expedite)
{
  Path::Ptr p = Path::PathIs();
  p->expediteIs(ExpediteSupported);
  ASSERT_TRUE(p->expedite() == ExpediteSupported);
  p->expediteIs(ExpediteNotSupported);
  ASSERT_TRUE(p->expedite() == ExpediteNotSupported);
}

TEST(Path, ExpeditePath) 
{
  FleetDesc::Ptr fleet = FleetDesc::FleetDescIs();
  fleet->speedIs(5);
  fleet->costPerMileIs(10);

  Location::Ptr loc1 = Customer::CustomerIs("pa");
  Location::Ptr loc2 = Port::PortIs("redwood");
  Location::Ptr loc3 = BoatTerminal::BoatTerminalIs("sf");

  Segment::Ptr seg1 = TruckSegment::TruckSegmentIs("280N");
  Segment::Ptr seg11 = TruckSegment::TruckSegmentIs("280S");
  seg1->sourceIs(loc1);
  seg11->sourceIs(loc2);
  seg1->returnSegmentIs(seg11);
  seg1->difficultyIs(2.0);
  seg1->lengthIs(15);
  seg1->expediteIs(ExpediteSupported);

  Segment::Ptr seg2 = BoatSegment::BoatSegmentIs("bay");
  Segment::Ptr seg22 = BoatSegment::BoatSegmentIs("bay");
  seg2->sourceIs(loc2);
  seg22->sourceIs(loc3);
  seg22->returnSegmentIs(seg2);
  seg2->difficultyIs(3.0);
  seg2->lengthIs(25);
  seg2->expediteIs(ExpediteSupported);

  Segment::Ptr seg3 = PlaneSegment::PlaneSegmentIs("skyline");
  Segment::Ptr seg33 = PlaneSegment::PlaneSegmentIs("skyline");
  seg3->sourceIs(loc2);
  seg33->sourceIs(loc1);
  seg3->returnSegmentIs(seg33);
  seg3->expediteIs(ExpediteSupported);

  Path::Ptr p = Path::PathIs();
  p->expediteIs(ExpediteSupported);
  p->segmentNew(seg1, fleet);
  p->segmentNew(seg2, fleet);
  p->segmentNew(seg3, fleet);

  ASSERT_TRUE(p->segments() == 2);

  Path::SegmentIterator it = p->segmentIter();
  ASSERT_TRUE(*it == seg1); 
  it++;
  ASSERT_TRUE(*it == seg2);

  ASSERT_TRUE(p->cost() == 1575);
  ASSERT_TRUE(p->length() == 40);
  ASSERT_TRUE(std::abs(p->time().value() - 6.1538) < 1e-2);
}

bool pathExists(PathList::Ptr plist, string start, string end, Dollars cost,
    Hours time, Miles length, ExpediteOptions exp)
{
  for (PathList::PathIterator it = plist->pathIter();
     it != plist->pathIter() + plist->paths();
     ++it) {
    Path::Ptr p = *it;
    if (p->start()->name() == start &&
        p->end()->name() == end && 
        p->cost() == cost &&
        p->time() == time &&
        p->length() == length &&
        p->expedite() == exp)
      return true;
  }
  return false;
}

TEST(Connectivity,exploreAll)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  Dollars maxCost = Dollars::max();
  Hours maxTime = Hours::max();
  Miles maxLength = Miles::max();
  
  PathList::Ptr plist = 
    c->explore(net->location("cent"), maxLength, maxCost, maxTime, 
               ExpediteNotSupported);

  ASSERT_TRUE(pathExists(plist, "cent", "r1", 3000, 14.2857, 100,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "t2", 5000, 100, 500,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "b3", 6000, 50, 300,
                       ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "c1", 3000+8000, 14.2857+33.333, 
                         100+200, ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "r2", 5000+24000, 100+120, 500+600,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "bb3", 6000+24000, 50+66.666, 300+400,
                       ExpediteNotSupported)); 
  ASSERT_TRUE(plist->paths() == 6);
}

TEST(Connectivity,exploreCostFilter)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  Dollars maxCost = 17000;
  Hours maxTime = Hours::max();
  Miles maxLength = Miles::max();

  PathList::Ptr plist = 
    c->explore(net->location("cent"), maxLength, maxCost, maxTime, 
               ExpediteNotSupported);

  ASSERT_TRUE(pathExists(plist, "cent", "r1", 3000, 14.2857, 100,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "t2", 5000, 100, 500,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "b3", 6000, 50, 300,
                       ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "c1", 3000+8000, 14.2857+33.333, 
                         100+200, ExpediteNotSupported));  
  ASSERT_TRUE(plist->paths() == 4);
}

TEST(Connectivity,exploreTimeFilter)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  Dollars maxCost = Dollars::max();
  Hours maxTime = 48;
  Miles maxLength = Miles::max();
  PathList::Ptr plist = 
    c->explore(net->location("cent"), maxLength, maxCost, maxTime, 
               ExpediteNotSupported);

  ASSERT_TRUE(pathExists(plist, "cent", "r1", 3000, 14.2857, 100,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "c1", 3000+8000, 14.2857+33.333, 
                         100+200, ExpediteNotSupported)); 
  ASSERT_TRUE(plist->paths() == 2);
}


TEST(Connectivity,exploreLengthFilter)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  Dollars maxCost = Dollars::max();
  Hours maxTime = Hours::max();
  Miles maxLength = 350;
  PathList::Ptr plist = 
    c->explore(net->location("cent"), maxLength, maxCost, maxTime, 
               ExpediteNotSupported);

  ASSERT_TRUE(pathExists(plist, "cent", "r1", 3000, 14.2857, 100,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "b3", 6000, 50, 300,
                       ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "c1", 3000+8000, 14.2857+33.333, 
                         100+200, ExpediteNotSupported)); 
  ASSERT_TRUE(plist->paths() == 3);
}


TEST(Connectivity,connect1)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  PathList::Ptr plist = 
    c->connect(net->location("cent"), net->location("c1"));

  ASSERT_TRUE(pathExists(plist, "cent", "c1", 3000+8000, 14.2857+33.333, 
                         100+200, ExpediteNotSupported)); 
  ASSERT_TRUE(pathExists(plist, "cent", "c1", 4500+12000, 10.989+25.641, 
                         100+200, ExpediteSupported)); 
  ASSERT_TRUE(plist->paths() == 2);
}


TEST(Connectivity,connect2)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  PathList::Ptr plist = 
    c->connect(net->location("cent"), net->location("r2"));

  ASSERT_TRUE(pathExists(plist, "cent", "r2", 5000+24000, 100+120, 500+600,
                         ExpediteNotSupported)); 
  ASSERT_TRUE(plist->paths() == 1);
}

TEST(Connectivity, connect3)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  net->segmentDel("tseg2");
  PathList::Ptr plist = 
    c->connect(net->location("cent"), net->location("r2"));
  ASSERT_TRUE(plist->paths() == 0);  
}

TEST(Connectivity, connect4)
{
  Network::Ptr net = prepareNetwork1();
  Connectivity::Ptr c = Connectivity::ConnectivityIs(net);
  net->segmentDel("tseg1-ret");
  PathList::Ptr plist = 
    c->connect(net->location("cent"), net->location("r2"));
  ASSERT_TRUE(plist->paths() == 0);  
}

TEST(Activity, activity) 
{
  Activity::Manager::Ptr manager = activityManagerInstance();
  Activity::Ptr act = manager->activityNew("act");
  act->statusIs(Activity::ready);
  manager->lastActivityIs(act);
  ASSERT_TRUE(act->status() == Activity::ready);
  act->nextTimeIs(2);
  manager->nowIs(2);
  ASSERT_TRUE(act->status() == Activity::free);

}







