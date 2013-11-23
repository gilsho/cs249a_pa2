
#ifndef NETWORK_SIMULATION_H
#define NETWORK_SIMULATION_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <stdint.h>
#include "PtrInterface.h"
#include "Ptr.h"
#include "Nominal.h"
#include "Engine.h"
#include "ActivityImpl.h"

using std::cerr;

namespace Shipping {

class Shipment : public Fwk::PtrInterface<Shipment> {
public:

	typedef Fwk::Ptr<Shipment> Ptr;

	static Ptr ShipmentIs(Customer::Ptr _source, 
												Customer::Ptr _destination,
												Hours _departure) {
		Ptr m = new Shipment(_source, _destination, _departure);
		return m;
	}

	Customer::Ptr source() const { return source_; }
	Customer::Ptr destination() const { return destination_; }
	Hours departure() const { return departure_; }

protected:
	Shipment(Customer::Ptr _source, Customer::Ptr _destination, 
		Hours _departure) : source_(_source), destination_(_destination),
			departure_(_departure) {}

	Customer::Ptr source_;
	Customer::Ptr destination_;
	Hours departure_;
};

class NetworkSimulation : public Fwk::PtrInterface<NetworkSimulation> {
public:
	typedef Fwk::Ptr<NetworkSimulation> Ptr;

	static Ptr NetworkSimulationNew(Activity::Manager::Ptr _manager) {
		Ptr netsim = new NetworkSimulation(_manager);
		return netsim;
	}

	Activity::Manager::Ptr activityManager() const;


protected:
	NetworkSimulation(Activity::Manager::Ptr _manager) : manager_(_manager) {}

	class NetworkReactor : public Network::Notifiee {
	public:
		typedef Fwk::Ptr<NetworkSimulation::NetworkReactor> Ptr;

		static Ptr ReactorIs(NetworkSimulation *n) {
			Ptr m = new NetworkReactor(n);
			return m;
		}

		void onSegmentNew(Segment::Ptr seg);
		void onSegmentDel(Segment::Ptr seg);
		
		void onLocationNew(Location::Ptr loc);
		void onLocationDel(Location::Ptr loc);

	protected:
		NetworkReactor(NetworkSimulation *n) : owner_(n) {}
		NetworkSimulation *owner_;
	};

	class CustomerReactor : public Customer::Notifiee {
  public:
  	typedef Fwk::Ptr<NetworkSimulation::CustomerReactor> Ptr;

		static Ptr ReactorIs(NetworkSimulation *n) {
			Ptr m = new CustomerReactor(n);
			return m;
		}

    void onTransferRate() {}
    void onShipmentSize() {}
    void onDestination() {}

  protected:
    CustomerReactor(NetworkSimulation *n) : owner_(n) {}
		NetworkSimulation *owner_;
  };

  class SegmentReactor : public Segment::Notifiee {
  public:
  	typedef Fwk::Ptr<NetworkSimulation::SegmentReactor> Ptr;

		static Ptr ReactorIs(NetworkSimulation *n) {
			Ptr m = new SegmentReactor(n);
			return m;
		}

  	void onExpedite() {}

  protected:
    SegmentReactor(NetworkSimulation *n) : owner_(n) {}
		NetworkSimulation *owner_;
  };


	NetworkReactor::Ptr net_reactor_;
	vector<CustomerReactor::Ptr> customer_reactors_;
	vector<SegmentReactor::Ptr> segment_reactors_;

	Activity::Manager::Ptr manager_;
};


} /* namespace Shipping */



#endif /* NETWORK_SIMULATION_H */


