#include "gtest/gtest.h"
#include <fstream>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include "Engine.h"

using namespace Shipping;

TEST(Location, NotifieeTest) 
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

    int segments;
    int locations;
  };

  Network::Ptr net = Network::NetworkIs("network");
  Reactor * rec = new Reactor();
  rec->notifierIs(net);

  Location::Ptr loc1 = Location::LocationIs("sf");
  Location::Ptr loc2 = Location::LocationIs("paris");
  Segment::Ptr seg1 = Segment::SegmentIs("280", loc1, loc2);
  Segment::Ptr seg2 = Segment::SegmentIs("101", loc2, loc1);

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

}


TEST(Engine, InstanceOf) 
{
  Location::Ptr loc = Location::LocationIs("sf");

  ASSERT_TRUE(INSTANCE_OF(loc.ptr(), Location*));
  ASSERT_TRUE(!INSTANCE_OF(loc.ptr(), Segment*));
}
