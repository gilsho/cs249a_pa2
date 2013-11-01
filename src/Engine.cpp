
#include "Engine.h"

using std::cout;
using std::endl;

namespace Shipping {


void Segment::notifieeNew(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n)
			return;
	}

	notifiee_.push_back(n);
}

void Segment::notifieeDel(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n) {
			notifiee_.erase(it);
			return;
		}
	}
}

void Segment::expediteIs(ExpediteOptions _expedite) {
	// if (_expedite == expedite_) return;
	expedite_ = _expedite;
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		(*it)->onExpedite();
	}
}

Segment::Segment(string name) : name_(name), time_(0), cost_(0), length_(0),
										 difficulty_(1.0) {} 


void Segment::Notifiee::notifierIs(const Segment::Ptr _notifier) {
		if (_notifier == notifier_) 
		return;
	notifier_ = _notifier;
	notifier_->notifieeNew(this);
}


Segment::Notifiee::~Notifiee() {
	if (notifier_.ptr()) {
		notifier_->notifieeDel(this);
	}
}


/*
	// make operation idempotent
	if (segments_.find(seg->name) != segments_.end())
		return;
	segments_[seg->name] = seg;
	// for (SegmentIterator it = segment_.begin(); 
	// 		 it != segment_.end();
	// 		 ++it) {
	// 	if (it->ptr() == seg) 
	// 		return;
	// }

	Segment::Ptr p = seg;
	// segment_.push_back(p);
	*/

Segment::Ptr Network::segment(string name) {
	if (segments_.find(name) == segments_.end())
		return NULL;
	return segments_[name];
}

void Network::segmentIs(Segment::Ptr seg) {
	// make operation idempotent
	if (segments_.find(seg->name()) != segments_.end())
		return;

	segments_[seg->name()] = seg;

	// notification
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		(*it)->onSegmentNew(seg);
	}
}

void Network::segmentDel(string name) {
	if (segments_.find(name) == segments_.end()) return;
	map<string, Segment::Ptr>::iterator itseg = segments_.find(name);
	segments_.erase(itseg);

	// notification
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		(*it)->onSegmentDel(itseg->second);
	}
}


Location::Ptr Network::location(string name) {
	if (locations_.find(name) == locations_.end())
		return NULL;
	return locations_[name];
}

void Network::locationIs(Location::Ptr loc) {
	// make operation idempotent
	if (locations_.find(loc->name()) != locations_.end())
		return;

	locations_[loc->name()] = loc;

	// notification
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		(*it)->onLocationNew(loc);
	}
}

void Network::locationDel(string name) {
	if (locations_.find(name) == locations_.end()) return;
	map<string, Location::Ptr>::iterator itseg = locations_.find(name);
	locations_.erase(itseg);

	// notification
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		(*it)->onLocationDel(itseg->second);
	}
}


void Network::notifieeNew(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n)
			return;
	}

	notifiee_.push_back(n);
}

void Network::notifieeDel(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n) {
			notifiee_.erase(it);
			return;
		}
	}
}

void Network::Notifiee::notifierIs(const Network::Ptr _notifier) {
	if (_notifier == notifier_) 
		return;
	notifier_ = _notifier;
	notifier_->notifieeNew(this);
}

Network::Notifiee::~Notifiee() {
	if (notifier_.ptr()) {
		notifier_->notifieeDel(this);
	}
}


void Fleet::speedIs(MilesPerHour _speed){
	if (speed_ == _speed) return;
	speed_ = _speed;
}

void Fleet::capacityIs(PackageCapacity _capacity){
	if (capacity_ == _capacity) return;
	capacity_ = _capacity;
}

void Fleet::costPerMileIs(Dollars _costPerMile){
	if (costPerMile_ == _costPerMile) return;
	costPerMile_ = _costPerMile;
}

StatsConfig::StatsConfig(Network::Ptr _network)  {
	network_ = _network;
	customer_locations_ = 0;
	ports_ = 0; 
	truck_terminals_ = 0;
	boat_terminals_ = 0;
	plane_terminals_ = 0; 
	truck_segments_ = 0;
	boat_segments_ = 0; 
	plane_segments_ = 0;
	net_reactor_ = NetworkReactor::ReactorIs(this);
	net_reactor_->notifierIs(network_);
}


void StatsConfig::segmentReactorNew(Segment::Ptr seg)
{
	SegmentReactor::Ptr reactor = SegmentReactor::ReactorIs(this);
	reactor->notifierIs(seg);
	seg_reactor_.push_back(reactor);
}

void StatsConfig::NetworkReactor::onSegmentNew(Segment::Ptr seg) {
	if (INSTANCE_OF(seg.ptr(), BoatSegment*)) {
		owner_->boatSegmentsIs(1 + owner_->boatSegments());
	} else if (INSTANCE_OF(seg.ptr(), PlaneSegment*)) {
		owner_->planeSegmentsIs(1 + owner_->planeSegments()); 
	} else if (INSTANCE_OF(seg.ptr(), TruckSegment*)) {
		owner_->truckSegmentsIs(1 + owner_->truckSegments());
	} else {
		// invalid segment. do nothing
	}
	owner_->segmentReactorNew(seg);
}

void StatsConfig::NetworkReactor::onSegmentDel(Segment::Ptr seg) {
	if (INSTANCE_OF(seg.ptr(), BoatSegment*)) {
		owner_->boatSegmentsIs(owner_->boatSegments() - 1);
	} else if (INSTANCE_OF(seg.ptr(), PlaneSegment*)) {
		owner_->planeSegmentsIs(owner_->planeSegments() - 1); 
	} else if (INSTANCE_OF(seg.ptr(), TruckSegment*)) {
		owner_->truckSegmentsIs(owner_->truckSegments() - 1);
	} else {
		// invalid segment. do nothing
	}
	owner_->segmentReactorNew(seg);
}

void StatsConfig::NetworkReactor::onLocationNew(Location::Ptr loc) {
	if (INSTANCE_OF(loc.ptr(), CustomerLocation*)) {
		owner_->customerLocationsIs(1 + owner_->customerLocations());
	} else if (INSTANCE_OF(loc.ptr(), Port*)) {
		owner_->portsIs(1 + owner_->ports());
	} else if (INSTANCE_OF(loc.ptr(), TruckTerminal*)) {
		owner_->truckTerminalsIs(1 + owner_->truckTerminals());
	} else if (INSTANCE_OF(loc.ptr(), BoatTerminal*)) {
		owner_->boatTerminalsIs(1 + owner_->boatTerminals());
	} else if (INSTANCE_OF(loc.ptr(), PlaneTerminal*)) {
		owner_->planeTerminalsIs(1 + owner_->planeTerminals());
	}
}

void StatsConfig::NetworkReactor::onLocationDel(Location::Ptr loc) {
	if (INSTANCE_OF(loc.ptr(), CustomerLocation*)) {
		owner_->customerLocationsIs(owner_->customerLocations() - 1);
	} else if (INSTANCE_OF(loc.ptr(), Port*)) {
		owner_->portsIs(owner_->ports() - 1);
	} else if (INSTANCE_OF(loc.ptr(), TruckTerminal*)) {
		owner_->truckTerminalsIs(owner_->truckTerminals() - 1);
	} else if (INSTANCE_OF(loc.ptr(), BoatTerminal*)) {
		owner_->boatTerminalsIs(owner_->boatTerminals() - 1);
	} else if (INSTANCE_OF(loc.ptr(), PlaneTerminal*)) {
		owner_->planeTerminalsIs(owner_->planeTerminals() - 1);
	}
}

void StatsConfig::SegmentReactor::onExpedite() {
	Segment::ExpediteOptions newval = notifier_->expedite();
	if (oldval == newval) return;
	oldval = newval;
	if (newval) {
		owner_->expeditedIs(owner_->expedited() + 1);
	} else {
		owner_->expeditedIs(owner_->expedited() - 1);
	}
}


void StatsConfig::customerLocationsIs(Stats::EntityCount _customer_locations) { 
	if (customer_locations_ == _customer_locations) return;
	customer_locations_ = _customer_locations; 
}

void StatsConfig::portsIs(Stats::EntityCount _ports) { 
	if (ports_ == _ports) return;
	ports_ = _ports; 
}
void StatsConfig::truckTerminalsIs(Stats::EntityCount _truck_terminals) { 
	if (truck_terminals_ == _truck_terminals) return;
	truck_terminals_ = _truck_terminals; 
}
void StatsConfig::boatTerminalsIs(Stats::EntityCount _boat_terminals) { 
	if (boat_terminals_ == _boat_terminals) return;
	boat_terminals_ = _boat_terminals; 
}
void StatsConfig::planeTerminalsIs(Stats::EntityCount _plane_terminals) { 
	if (plane_terminals_ == _plane_terminals) return;
	plane_terminals_ = _plane_terminals; 
}
void StatsConfig::truckSegmentsIs(Stats::EntityCount _truck_segments) { 
	if (truck_segments_ == _truck_segments) return;
	truck_segments_ = _truck_segments; 
}
void StatsConfig::boatSegmentsIs(Stats::EntityCount _boat_segments) { 
	if (_boat_segments == boat_segments_) return;
	boat_segments_ = _boat_segments; 
}
void StatsConfig::planeSegmentsIs(Stats::EntityCount _plane_segments) { 
	if (plane_segments_ == _plane_segments) return;
	plane_segments_ = _plane_segments; 
}
void StatsConfig::expeditedIs(Stats::EntityCount _expedited) { 
	if (expedited_ == _expedited) return;
	expedited_ = _expedited; 
}

} /* end namespace */





























