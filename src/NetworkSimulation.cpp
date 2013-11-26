
#include "NetworkSimulation.h"
#define DEBUG_NETWORK_SIMULATION 1

#if DEBUG_NETWORK_SIMULATION
#define LOG(x) cout << "[" << __func__ << "]: " \
								 << x << endl;
#else
#define LOG(x)
#endif

namespace Shipping {

NetworkSimulation::NetworkSimulation(Network::Ptr _net, 
	Activity::Manager::Ptr _manager) : net_(_net), manager_(_manager) {

	net_reactor_ = NetworkReactor::ReactorIs(this);
	net_reactor_->notifierIs(net_);

}

void NetworkSimulation::NetworkReactor::onLocationNew(Location::Ptr loc) {
	LOG("reactor called");
	Customer::Ptr cust = dynamic_cast<Customer *>(loc.ptr());
	if (cust != NULL)	{
		owner_->customerReactorNew(cust);
	}
	owner_->locationReactorNew(loc);
}

void NetworkSimulation::NetworkReactor::onLocationDel(Location::Ptr loc) {
	Customer::Ptr cust = dynamic_cast<Customer *>(loc.ptr());
	if (cust != NULL)	{
		owner_->customerReactorDel(cust);
	}
	owner_->locationReactorDel(loc);
}

void NetworkSimulation::NetworkReactor::onSegmentNew(Segment::Ptr seg) {
	LOG("reactor called");
	owner_->segmentReactorNew(seg);
}

void NetworkSimulation::NetworkReactor::onSegmentDel(Segment::Ptr seg) {
	owner_->segmentReactorDel(seg);
}


void NetworkSimulation::customerReactorNew(Customer::Ptr cust) {
	LOG("creating customer reactor for segment: " << cust->name());
	CustomerReactor::Ptr reactor = CustomerReactor::ReactorIs(this,cust);
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
	LOG("creating location reactor for location: " << loc->name());
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
	LOG("creating segment reactor for segment: " << seg->name());
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

void NetworkSimulation::CustomerReactor::onTransferRate() {
	onShippingSettings_();
}

void NetworkSimulation::CustomerReactor::onShipmentSize() {
	onShippingSettings_();
}

void NetworkSimulation::CustomerReactor::onDestination() {
	onShippingSettings_();
}

void NetworkSimulation::CustomerReactor::onShippingSettings_() {
	LOG("shipping settings changed");
	if (notifier_->transferRate() > 0 &&
			notifier_->shipmentSize() > 0 &&
			notifier_->destination() != NULL) {

		LOG("initiating shipments");
		act_->statusIs(Activity::nextTimeScheduled);

	} else {
		LOG("disabling shipment injecting");
		LOG("activity is: " << act_->name());
		act_->statusIs(Activity::deleted);
	}
}


void NetworkSimulation::LocationReactor::onShipment(Shipment::Ptr shp) {
	LOG("onShipment called."); 
	Location::Ptr loc = notifier_;
	Network::Ptr net = owner_->network();

	LOG("'" << loc->name() << "' received shipment destined to: '"
		<< shp->destination()->name() << "', status is: "
		<< shp->status());

	LOG("# of pending shipments: " << loc->shipments());

	if (shp->status() == Shipment::refused) {
		return;
	}

	loc->shipmentDel(shp);

	if (shp->destination()->name() == loc->name()) {
		// update statistics
		Customer::Ptr cust = dynamic_cast<Customer *>(loc.ptr());
		if (!cust) {
			cerr << "package sent to non-Customer destination" << endl;
			return;
		}
		cust->shipmentsReceivedIs(cust->shipmentsReceived().value() + 1);
  	cust->totalCostIs(cust->totalCost().value() + shp->cost().value());
  	cust->totalLatencyIs(cust->totalLatency().value() + shp->latency().value());
  	return;
	}
	
	Segment::Ptr seg = net->routingTable()->nextSegment(
		loc->name(),shp->destination()->name());

	if (seg != NULL)
		seg->shipmentIs(shp);
}

void NetworkSimulation::SegmentReactor::forwardReactorDel(
	ForwardActivityReactor::Ptr _fwdreactor) {
	for (vector<ForwardActivityReactor::Ptr>::iterator it = reactorset_.begin();
		it != reactorset_.end();
		++it) {
		if ((*it) == _fwdreactor) {
			reactorset_.erase(it);
			return;
		}
	}
}

void NetworkSimulation::SegmentReactor::onShipmentDel(Shipment::Ptr shp) {
	Segment::Ptr seg = notifier_;
	Network::Ptr net = owner_->network();
	Location::Ptr source = seg->source();
	LOG("onShipmentDel called on segment: " << seg->name());
	LOG("# of pending shipments: " << source->shipments());
	//retransmit refused shipments at the source
	for (Location::ShipmentIterator it = source->shipmentIter();
			 it != source->shipmentIter() + source->shipments();
			 ++it) {
		LOG("about to retransmit!");
		Shipment::Ptr refshp = (*it);
		if (net->routingTable()->nextSegment(
			source->name(),refshp->destination()->name()) == seg) {
			LOG("retransmitting refused shipment from: '" << 
				source->name() << "' to segment: '" << seg->name() << "'");
			source->shipmentDel(refshp);
			refshp->statusIs(Shipment::enroute);
			seg->shipmentIs(refshp);
			break;
		}
	}
}

void NetworkSimulation::SegmentReactor::onShipment(Shipment::Ptr shp) {
	NetworkSimulation::Ptr netsim = owner_;
	Activity::Manager::Ptr manager = netsim->manager();
	Segment::Ptr seg = notifier_;
	Network::Ptr net = owner_->network();
	FleetDesc::Ptr fleet = net->fleet(seg->mode());

	LOG(seg->name() << " received shipment");

	//update cost and latency
	Dollars segcost = Dollars(seg->length().value() * 
		fleet->costPerMile().value() * seg->difficulty().value());
	shp->costIs(shp->cost().value() + segcost.value());

	LOG("updating cost of shipment to: "
		<< shp->cost().value()
	 	<< " and latency to: "
	 	<< shp->latency().value());

	//update segment statistics
	seg->shipmentsReceivedIs(seg->shipmentsReceived().value() + 1);

	// create activity && reactor
  char s[30]; sprintf(s, "%d", rand());
	Activity::Ptr act = manager->activityNew("fwd-" + string(s));

	NetworkSimulation::ForwardActivityReactor::Ptr reactor = 
		NetworkSimulation::ForwardActivityReactor::ReactorIs(
			this,manager,act.ptr(),seg,shp);
	reactorset_.push_back(reactor);

	act->lastNotifieeIs(reactor.ptr());

	// schedule activity
	/* consider expedite support */	
	Hours duration = seg->length() / fleet->speed();
	act->nextTimeIs(Time(manager->now().value() + duration.value()));
	LOG("scheduling activity: " << act->name()
		<< " for time: " << manager->now().value() + duration.value());
	act->statusIs(Activity::nextTimeScheduled);

}

void NetworkSimulation::ForwardActivityReactor::onStatus() {
	switch(act_->status()) {
		case Activity::executing: {
			LOG("executing");
			seg_->shipmentDel(shp_);
			Hours latency = manager_->now().value() - shp_->departure().value();
			shp_->latencyIs(latency);
			
			if (seg_->destination())
				seg_->destination()->shipmentIs(shp_);

			//remove reactor from segment reactor
			owner_->forwardReactorDel(this);
		} break;

    case Activity::nextTimeScheduled: {
			LOG("regsitering activity: " << act_->name());
			manager_->lastActivityIs(act_);
		}	break;

    default:
			break;
  }
}

void NetworkSimulation::InjectActivityReactor::onStatus() {

	switch(act_->status()) {

		case Activity::executing: {
			LOG("executing");
			Shipment::Ptr shp = Shipment::ShipmentIs(cust_,
				cust_->destination(), cust_->shipmentSize(), manager_->now());
			LOG("Injecting shipment to: " << cust_->name()
				<< " at time: " << manager_->now().value());
			shp->statusIs(Shipment::enroute);
			cust_->shipmentIs(shp);
			
		}	break;
		
		case Activity::free: {
			Time period = Time(24.0 / cust_->transferRate().value());
			Time next = Time(act_->nextTime().value() + period.value());
			LOG("scheduling activity: " << act_->name() 
				<< " for: " << next.value());
			act_->nextTimeIs(next);
			act_->statusIs(Activity::nextTimeScheduled);
			
		}	break;

		case Activity::nextTimeScheduled:
			LOG("regsitering activity: " << act_->name());
			manager_->lastActivityIs(act_);
			break;

		case Activity::deleted:
			LOG("remove activity: " <<
				act_->name() << " from manager");
			manager_->activityDel(act_->name());
			break;

		default:
			break;
	}
}



} /* namespace Shipping */

























