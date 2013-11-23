
#include "NetworkSimulation.h"

namespace Shipping {

void NetworkSimulation::NetworkReactor::onLocationNew(Location::Ptr loc) {
	Customer::Ptr cust = dynamic_cast<Customer *>(loc.ptr());
	if (cust != NULL)	
		owner_->customerReactorNew(cust);
	else
		owner_->locationReactorNew(loc);
}

void NetworkSimulation::NetworkReactor::onLocationDel(Location::Ptr loc) {
	Customer::Ptr cust = dynamic_cast<Customer *>(loc.ptr());
	if (cust != NULL)	
		owner_->customerReactorDel(cust);
	else 
		owner_->locationReactorDel(loc);
}

void NetworkSimulation::NetworkReactor::onSegmentNew(Segment::Ptr seg) {
	owner_->segmentReactorNew(seg);
}

void NetworkSimulation::NetworkReactor::onSegmentDel(Segment::Ptr seg) {
	owner_->segmentReactorDel(seg);
}

void NetworkSimulation::customerReactorNew(Customer::Ptr cust) {
	CustomerReactor::Ptr reactor = CustomerReactor::ReactorIs(this);
  reactor->notifierIs(cust);
  cust_reactor_.push_back(reactor);
}

void NetworkSimulation::customerReactorDel(Customer::Ptr cust) {
	for (vector<CustomerReactor::Ptr>::iterator it = cust_reactor_.begin();
			 it != cust_reactor_.end();
			 ++it) {
		if ((*it)->notifier() == cust) {
			cust_reactor_.erase(it);
			return;
		}
	}	
}

void NetworkSimulation::locationReactorNew(Location::Ptr loc) {
	LocationReactor::Ptr reactor = LocationReactor::ReactorIs(this);
  reactor->notifierIs(loc);
  loc_reactor_.push_back(reactor);
}

void NetworkSimulation::locationReactorDel(Location::Ptr loc) {
	for (vector<LocationReactor::Ptr>::iterator it = loc_reactor_.begin();
			 it != loc_reactor_.end();
			 ++it) {
		if ((*it)->notifier() == loc) {
			loc_reactor_.erase(it);
			return;
		}
	}	
}


void NetworkSimulation::segmentReactorNew(Segment::Ptr seg) {
  SegmentReactor::Ptr reactor = SegmentReactor::ReactorIs(this);
  reactor->notifierIs(seg);
  seg_reactor_.push_back(reactor);
}

void NetworkSimulation::segmentReactorDel(Segment::Ptr seg) {
	for (vector<SegmentReactor::Ptr>::iterator it = seg_reactor_.begin();
			 it != seg_reactor_.end();
			 ++it) {
		if ((*it)->notifier() == seg) {
			seg_reactor_.erase(it);
			return;
		}
	}	
}


} /* namespace Shipping */