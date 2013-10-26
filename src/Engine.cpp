
#include "Engine.h"

namespace Shipping {

void Location::segmentIs(Segment *seg) {
	// make operation idempotent
	for (SegmentIterator it = segment_.begin(); 
			 it != segment_.end();
			 ++it) {
		if (it->ptr() == seg) 
			return;
	}

	Segment::Ptr p = seg;
	segment_.push_back(p);

	// notification
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		(*it)->onSegment(p);
	}
}


void Location::notifieeNew(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n)
			return;
	}

	notifiee_.push_back(n);
}

void Location::notifieeDel(Notifiee * n) {
	for (NotifieeIterator it = notifiee_.begin();
			 it != notifiee_.end();
			 ++it) {
		if (*it == n) {
			notifiee_.erase(it);
			return;
		}
	}
}

void Location::Notifiee::notifierIs(const Location::Ptr& _notifier) {
	if (_notifier == notifier_) 
		return;
	notifier_ = _notifier;
	notifier_->notifieeNew(this);
}

Location::Notifiee::~Notifiee() {
	if (notifier_.ptr()) {
		notifier_->notifieeDel(this);
	}
}

Segment::Segment() : time_(0), cost_(0), length_(0), capacity_(0), 
										 difficulty_(1.0) {} 

} /* end namespace */





























