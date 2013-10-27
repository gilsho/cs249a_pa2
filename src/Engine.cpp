
#include "Engine.h"

namespace Shipping {

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


void Network::segmentIs(Segment::Ptr& seg) {
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

void Network::locationIs(Location::Ptr& loc) {
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

void Network::Notifiee::notifierIs(const Network::Ptr& _notifier) {
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

Segment::Segment(string name) : name_(name), time_(0), cost_(0), length_(0), capacity_(0), 
										 difficulty_(1.0) {} 



void Fleet::Notifiee::notifierIs(const Fleet::Ptr& _notifier) {
	if (_notifier == notifier_) 
		return;
	notifier_ = _notifier;
	notifier_->notifieeNew(this);
}

void Fleet::notifieeNew(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n)
			return;
	}

	notifiee_.push_back(n);
}

Fleet::Notifiee::~Notifiee() {
	if (notifier_.ptr()) {
		notifier_->notifieeDel(this);
	}
}

void Fleet::notifieeDel(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n) {
			notifiee_.erase(it);
			return;
		}
	}
}


void Fleet::speedIs(MilesPerHour _speed){
	if (speed_ == _speed) return;
	speed_ = _speed;
	for (NotifieeIterator it = notifiee_.begin();
			it != notifiee_.end();
			++it)
	{
		(*it)->onSpeed();
	}
}

void Fleet::capacityIs(Segment::PackageCapacity _capacity){
	if (capacity_ == _capacity) return;
	capacity_ = _capacity;
	for (NotifieeIterator it = notifiee_.begin();
			it != notifiee_.end();
			++it)
	{
		(*it)->onCapacity();
	}
}

void Fleet::costPerMileIs(Dollars _costPerMile){
	if (costPerMile_ == _costPerMile) return;
	costPerMile_ = _costPerMile;
	for (NotifieeIterator it = notifiee_.begin();
			it != notifiee_.end();
			++it)
	{
		(*it)->onCostPerMile();
	}
}


} /* end namespace */





























