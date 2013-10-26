#include "gtest/gtest.h"
#include <fstream>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include "Engine.h"

using namespace Shipping;

TEST(Location, NotifieeTest) 
{
  class Reactor : public Location::Notifiee {
  public:
    Reactor() { count = 0;}
    virtual void onSegment(Segment::Ptr seg) {
      count++;
    }
    int count;
  };

  Location::Ptr loc = Location::LocationIs("sf");
  Reactor * rec = new Reactor();
  rec->notifierIs(loc);

  ASSERT_TRUE(rec->count == 0);

  Segment::Ptr seg1 = new Segment();
  loc->segmentIs(seg1.ptr());
  ASSERT_TRUE(rec->count == 1);

  Segment::Ptr seg2 = new Segment();
  loc->segmentIs(seg2.ptr());
  ASSERT_TRUE(rec->count == 2);

}

