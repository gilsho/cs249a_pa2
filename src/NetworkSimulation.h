
#ifndef NETWORK_SIMULATION_H
#define NETWORK_SIMULATION_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <stdint.h>
#include "PtrInterface.h"
#include "Ptr.h"
#include "Nominal.h"
#include "Engine.h"
#include "ActivityImpl.h"

using std::cerr;

namespace Shipping {


class NetworkSimulation : public Fwk::PtrInterface<NetworkSimulation> {
public:
	typedef Fwk::Ptr<NetworkSimulation> Ptr;

	static Ptr NetworkSimulationNew(Network::Ptr _net, Activity::Manager::Ptr _manager) {
		Ptr netsim = new NetworkSimulation(_net, _manager);
		return netsim;
	}

	Activity::Manager::Ptr activityManager() const;

protected:

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

	class InjectActivityReactor;
  class ForwardActivityReactor;

	class CustomerReactor : public Customer::Notifiee {
  public:
  	typedef Fwk::Ptr<NetworkSimulation::CustomerReactor> Ptr;

		static Ptr ReactorIs(NetworkSimulation *n, Customer::Ptr _cust) {
			Ptr m = new CustomerReactor(n, _cust);
			return m;
		}

    void onTransferRate();
    void onShipmentSize();
    void onDestination();

  protected:
    CustomerReactor(NetworkSimulation *n, Customer::Ptr _cust) : owner_(n) {
    	Activity::Manager::Ptr manager = owner_->manager();
    	char s[30]; sprintf(s, "%d", rand());
    	act_ = manager->activityNew("inject-" + string(s));
    	reactor_ = NetworkSimulation::InjectActivityReactor::ReactorIs(
				manager,act_.ptr(), _cust);
    	act_->lastNotifieeIs(reactor_.ptr());
    }
    void onShippingSettings_();
		NetworkSimulation *owner_;
		Activity::Ptr act_;
		Fwk::Ptr<InjectActivityReactor> reactor_;		
  };

  class LocationReactor : public Location::Notifiee {
  public:
  	typedef Fwk::Ptr<NetworkSimulation::LocationReactor> Ptr;

		static Ptr ReactorIs(NetworkSimulation *n) {
			Ptr m = new LocationReactor(n);
			return m;
		}

    void onShipment(Shipment::Ptr shp);

  protected:
    LocationReactor(NetworkSimulation *n) : owner_(n) {}
		NetworkSimulation *owner_;
  };

  class SegmentReactor : public Segment::Notifiee {
  public:
  	typedef Fwk::Ptr<NetworkSimulation::SegmentReactor> Ptr;

		static Ptr ReactorIs(NetworkSimulation *n) {
			Ptr m = new SegmentReactor(n);
			return m;
		}

		void forwardReactorDel(Fwk::Ptr<ForwardActivityReactor> _reactor);

		// events
  	void onShipment(Shipment::Ptr shp);
  	void onShipmentDel(Shipment::Ptr shp);

  protected:
    SegmentReactor(NetworkSimulation *n) : owner_(n) {}
		NetworkSimulation *owner_;
		vector<Fwk::Ptr<ForwardActivityReactor> > reactorset_;
  };


	class InjectActivityReactor : public Activity::Notifiee {
  public:
  	typedef Fwk::Ptr<InjectActivityReactor> Ptr;

  	static Ptr ReactorIs(Activity::Manager::Ptr _manager,
  											 Activity *_act,
  											 Customer::Ptr _cust) {
  		Ptr reactor = new InjectActivityReactor(_manager, _act, _cust);
  		return reactor;
  	}

    virtual void onStatus();

 protected:
 		InjectActivityReactor(Activity::Manager::Ptr _manager, Activity *_act, 
 			Customer::Ptr _cust) : Activity::Notifiee(_act), 
 			manager_(_manager), act_(_act), cust_(_cust) {}

 		Activity::Manager::Ptr manager_;
 		Activity::Ptr act_;
 		Customer::Ptr cust_;
  };


  class ForwardActivityReactor : public Activity::Notifiee {
  public:
  	typedef Fwk::Ptr<ForwardActivityReactor> Ptr;

  	static Ptr ReactorIs(SegmentReactor *_owner,
  											 Activity::Manager::Ptr _manager,
  											 Activity *_act,
  											 Segment::Ptr _seg,
  											 Shipment::Ptr _shp) {
  		Ptr reactor = new ForwardActivityReactor(_owner,
  			_manager, _act, _seg, _shp);
  		return reactor;
  	}

    virtual void onStatus();

 protected:
 		ForwardActivityReactor(SegmentReactor *_owner, 
 			Activity::Manager::Ptr _manager, Activity *_act, 
 			Segment::Ptr _seg,	Shipment::Ptr _shp) : 
 			Activity::Notifiee(_act), owner_(_owner),
 			manager_(_manager), act_(_act), seg_(_seg), shp_(_shp) {}
 		
 		SegmentReactor *owner_;
 		Activity::Manager::Ptr manager_;
 		Activity::Ptr act_;
 		Segment::Ptr seg_;
 		Shipment::Ptr shp_;
  };

  void customerReactorNew(Customer::Ptr cust);
	void customerReactorDel(Customer::Ptr cust);

	void locationReactorNew(Location::Ptr loc);
	void locationReactorDel(Location::Ptr loc);

	void segmentReactorNew(Segment::Ptr seg);
	void segmentReactorDel(Segment::Ptr seg);

	Network::Ptr network() const { return net_; }

	Activity::Manager::Ptr manager() const { return manager_; }

	NetworkSimulation(Network::Ptr _net, 
		Activity::Manager::Ptr _manager); 

	Network::Ptr net_;
	NetworkReactor::Ptr net_reactor_;
	vector<CustomerReactor::Ptr> cust_reactor_;
	vector<LocationReactor::Ptr> loc_reactor_;
	vector<SegmentReactor::Ptr> seg_reactor_;

	Activity::Manager::Ptr manager_;
};


} /* namespace Shipping */



#endif /* NETWORK_SIMULATION_H */


