
#include "Engine.h"

using std::cout;
using std::endl;
using std::set;
using std::queue;

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

void Segment::sourceIs(Location::Ptr _source) { 
	if (source_ == _source) return;
	source_ = _source;
	source_->segmentIs(this);
}

void Segment::destinationIs(Location::Ptr _destination) {
	if (destination_ == _destination) return;
	destination_ = _destination;
	destination_->segmentIs(this);
}


void Segment::expediteIs(ExpediteOptions _expedite) {
	if (_expedite == expedite_) return;
	expedite_ = _expedite;
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		(*it)->onExpedite();
	}
}

void Segment::returnSegmentIs(Segment::Ptr seg) {
	if (return_segment_ == seg) return;
	return_segment_ = seg;
}

Segment::Segment(string name, TransportationMode mode) : name_(name), 
	length_(0), difficulty_(1.0), expedite_(ExpediteNotSupported), mode_(mode) {} 


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


Segment::Ptr Location::segment(unsigned index) const {
	if (index < segments_.size()) {
		return segments_[index];
	}
	return NULL;
}

void Location::segmentIs(Segment::Ptr seg) {

	// make operation idempotent
	for (vector<Segment::Ptr>::iterator it = segments_.begin();
			it != segments_.end();
			++it) {
		if (*it == seg) {
			return;
		}
	}

	segments_.push_back(seg);
}


void Location::segmentDel(Segment::Ptr seg) {

	for (vector<Segment::Ptr>::iterator it = segments_.begin();
			it != segments_.end();
			++it) {
		if (*it == seg) {
			segments_.erase(it);
			return;
		}
	}
}

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


void FleetDesc::speedIs(MilesPerHour _speed){
	if (speed_ == _speed) return;
	speed_ = _speed;
}

void FleetDesc::capacityIs(PackageCapacity _capacity){
	if (capacity_ == _capacity) return;
	capacity_ = _capacity;
}

void FleetDesc::costPerMileIs(DollarsPerMile _costPerMile){
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
	if (seg->mode() == BoatMode) {
		owner_->boatSegmentsIs(1 + owner_->boatSegments());
	} else if (seg->mode() == PlaneMode) {
		owner_->planeSegmentsIs(1 + owner_->planeSegments()); 
	} else if (seg->mode() == TruckMode) {
		owner_->truckSegmentsIs(1 + owner_->truckSegments());
	} else {
		// invalid segment. do nothing
	}
	owner_->segmentReactorNew(seg);
}

void StatsConfig::NetworkReactor::onSegmentDel(Segment::Ptr seg) {
	if (seg->mode() == BoatMode) {
		owner_->boatSegmentsIs(owner_->boatSegments() - 1);
	} else if (seg->mode() == PlaneMode) {
		owner_->planeSegmentsIs(owner_->planeSegments() - 1); 
	} else if (seg->mode() == TruckMode) {
		owner_->truckSegmentsIs(owner_->truckSegments() - 1);
	} else {
		// invalid segment. do nothing
	}
	owner_->segmentReactorNew(seg);
}

void StatsConfig::NetworkReactor::onLocationNew(Location::Ptr loc) {
	if (loc->type() == CustomerLocation) {
		owner_->customerLocationsIs(1 + owner_->customerLocations());
	} else if (loc->type() == PortLocation) {
		owner_->portsIs(1 + owner_->ports());
	} else if (loc->type() == TruckTerminalLocation) {
		owner_->truckTerminalsIs(1 + owner_->truckTerminals());
	} else if (loc->type() == BoatTerminalLocation) {
		owner_->boatTerminalsIs(1 + owner_->boatTerminals());
	} else if (loc->type() == PlaneTerminalLocation) {
		owner_->planeTerminalsIs(1 + owner_->planeTerminals());
	}
}

void StatsConfig::NetworkReactor::onLocationDel(Location::Ptr loc) {
	if (loc->type() == CustomerLocation) {
		owner_->customerLocationsIs(owner_->customerLocations() - 1);
	} else if (loc->type() == PortLocation) {
		owner_->portsIs(owner_->ports() - 1);
	} else if (loc->type() == TruckTerminalLocation) {
		owner_->truckTerminalsIs(owner_->truckTerminals() - 1);
	} else if (loc->type() == BoatTerminalLocation) {
		owner_->boatTerminalsIs(owner_->boatTerminals() - 1);
	} else if (loc->type() == PlaneTerminalLocation) {
		owner_->planeTerminalsIs(owner_->planeTerminals() - 1);
	}
}

void StatsConfig::SegmentReactor::onExpedite() {
	ExpediteOptions newval = notifier_->expedite();
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

void Path::expediteIs(ExpediteOptions _expedite) {
	if (_expedite == expedite_) return;
	if (segments_.empty()) {
		expedite_ = _expedite;
	}
}

void Path::segmentNew(Segment::Ptr seg, FleetDesc::Ptr fleet) {

	/* avoid paths that don't support the expedite property */
	if (expedite_ == ExpediteSupported &&
			seg->expedite() != ExpediteSupported)
		return;

	/* avoid segments that don't logicially extend the path */
	if (end_loc_ != NULL && 
			end_loc_ != seg->source()) {
		return;	
	}

	/* avoid circular paths */
	if (visited_[seg->destination()->name()])
		return;

	segments_.push_back(seg);
	end_loc_ = seg->destination();
	visited_[end_loc_->name()] = true;

	if (start_loc_ == NULL) {
		start_loc_ = seg->source();
		visited_[start_loc_->name()] = true;
	}

	DollarsPerMile costPerMile = fleet->costPerMile();
	MilesPerHour speed = fleet->speed();
	if (expedite_ == ExpediteSupported) {
		speed *= Factor(1.3);
		costPerMile *= Factor(1.5);
	}

	cost_ += seg->length() * (costPerMile * seg->difficulty());
	time_ += seg->length() / speed;
	length_ += seg->length(); 

}

void PathList::pathNew(Path::Ptr p) {
	paths_.push_back(p);
}

bool Connectivity::meetPathConstraints(Path::Ptr p, Miles length, Hours time, 
																			 Dollars cost)
{
	if (p->cost() < cost &&
			p->time() < time &&
			p->length() < length)
		return true;
	else
		return false;
}

void Connectivity::enqueueNextSegments(queue<Path::Ptr>& q, Path::Ptr& p, 
												 ExpediteOptions expedite)
{
	Location::Ptr loc = p->end();
	for (Location::SegmentIterator it = loc->segmentIterator();
			 it != loc->segmentIterator() + loc->segments();
			 ++it) {
		Segment::Ptr seg = *it;

		if (expedite == ExpediteSupported &&
				seg->expedite() == ExpediteNotSupported)
			continue;

		Path::Ptr newp = p->clone();
		unsigned oldSegmentsCount = newp->segments();
		newp->segmentNew(seg, net_->fleet(seg->mode()));

		//enqueue only if new segment extends the path
		if (newp->segments() > oldSegmentsCount) {
			q.push(newp);
			// cout << "push: path(" << newp->start()->name() << "-> " << 
			// 	newp->end()->name() << ")" << endl;
		}
	}
}

PathList::Ptr Connectivity::explore(Location::Ptr start, Miles length, 
																		Dollars cost, Hours time, 
																		ExpediteOptions expedite) 
{
	PathList::Ptr plist = PathList::PathListNew();
	queue<Path::Ptr> toExplore;

	for(Location::SegmentIterator it = start->segmentIterator();
			it != start->segmentIterator() + start->segments();
			++it) {
		Segment::Ptr seg = *it;
		Path::Ptr p = Path::PathIs();
		p->expediteIs(ExpediteNotSupported);
		p->segmentNew(seg, net_->fleet(seg->mode()));
		// cout << "seg->source(): " << seg->source()->name() <<
		// 	", seg->destination(): " << seg->destination()->name() << endl;
		// cout << "path->start(): " << p->start()->name() << ", p->end(): "
		// 	<< p->end()->name() << endl << endl; 
		toExplore.push(p);

		//expedited paths are separate paths
		if (seg->expedite() == ExpediteSupported) {
			Path::Ptr pexp = Path::PathIs();
			pexp->expediteIs(ExpediteSupported);
			pexp->segmentNew(seg, net_->fleet(seg->mode()));
			toExplore.push(pexp);
		}
	}

	while(!toExplore.empty()) {
		Path::Ptr p = toExplore.front();
		toExplore.pop();
		// cout << "pop: path(" << p->start()->name() << "-> " 
		// 	<< p->end()->name() << ")" << endl;
		if (meetPathConstraints(p, length, time, cost)) {
			plist->pathNew(p);
			enqueueNextSegments(toExplore, p, expedite);
		}
	}

	return plist;
}

PathList::Ptr Connectivity::connect(Location::Ptr start, Location::Ptr end,
																		ExpediteOptions expedite) 
{
}

} /* end namespace */
























