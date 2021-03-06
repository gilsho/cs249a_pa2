
#define INSTANCE_OF(x, C) (dynamic_cast<C>((x)) != 0)

#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <queue>
#include <set>
#include <stdint.h>
#include "PtrInterface.h"
#include "Ptr.h"
#include "Instance.h"
#include "Nominal.h"
#include "Activity.h"
#include <typeinfo>

using std::vector;
using std::cerr;
using std::endl;
using std::map;
using std::queue;
using std::set;

namespace Shipping {

class Factor : public Ordinal<Factor, float>
{
public:
	Factor(float factor) : Ordinal<Factor, float>(factor) {}

	bool operator==(const Factor& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }

};

class DifficultyLevel : public Ordinal<DifficultyLevel, float> 
{
public:

	static const float minval = 1.0;
	static const float maxval = 5.0;

	static const DifficultyLevel min() { return minval; }
	static const DifficultyLevel max() { return maxval; }

	DifficultyLevel(float difficulty) : 
		Ordinal<DifficultyLevel, float>(difficulty) {
		if (difficulty < minval || difficulty > maxval) {
			cerr << "DifficultLevel: attempted to set illegal value" << endl;
			throw "invalid value";
		}
	}

	bool operator==(const DifficultyLevel& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }
};

class Hours : public Ordinal<Hours, float> {
public:

	static const float minval = 0;
	static const float maxval = 24*365;
	
	static const Hours min() { return minval; }
	static const Hours max() { return maxval; }

	Hours(float _hours) : Ordinal<Hours, float>(_hours) {
		if (_hours < minval || _hours > maxval) {
			cerr << "Hours: attempted to set illegal value" << endl;
			throw "invalid value";
		}
	}

	bool operator==(const Hours& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }

	Hours operator+=(const Hours& v)
	{ return (Nominal::value_ += v.value_); }

};

class Dollars : public Ordinal<Dollars, float> {
public:

	static const float minval = 0;
	static const float maxval = 10e9;

	static const Dollars min() { return minval; }
	static const Dollars max() { return maxval; }

	Dollars(float _dollars) : Ordinal<Dollars, float>(_dollars) {
		if (_dollars < minval || _dollars > maxval) {
			cerr << "Dollars: attempted to set illegal value" << endl;
			throw "invalid value";
		}
	}
		
	bool operator==(const Dollars& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }

	Dollars operator+=(const Dollars& v)
	{ return (Nominal::value_ += v.value_); }

};

class DollarsPerMile : public Ordinal<DollarsPerMile, float> {
public:

	static const float minval = 0;
	static const float maxval = 10e5;

	static const DollarsPerMile min() { return minval; }
	static const DollarsPerMile max() { return maxval; }

	DollarsPerMile(float _dollarsPerMile) : 
		Ordinal<DollarsPerMile, float>(_dollarsPerMile) {
			if (_dollarsPerMile < minval || _dollarsPerMile > maxval) {
				cerr << "DollarsPerMile: attempted to set illegal value" << endl;
				throw "invalid value";
			}
		}
		
	bool operator==(const DollarsPerMile& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }

	DollarsPerMile operator*(const DifficultyLevel& v) const
	{ return this->value() * v.value(); }

	DollarsPerMile operator*=(const Factor& v)
	{ return (Nominal::value_ *= v.value()); }

};

class MilesPerHour : public Ordinal<MilesPerHour, float> {
public:

	static const float minval = 0;
	static const float maxval = 10e5;

	static const MilesPerHour min() { return minval; }
	static const MilesPerHour max() { return maxval; }

	MilesPerHour(float _milesPerHour) : 
		Ordinal<MilesPerHour, float>(_milesPerHour) {
		if (_milesPerHour < minval || _milesPerHour > maxval) {
				cerr << "MilesPerHour: attempted to set illegal value" << endl;
				throw "invalid value";
			}
		}

	bool operator==(const MilesPerHour& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }

	MilesPerHour operator*=(const Factor& v)
	{ return (Nominal::value_ *= v.value()); }
};

class Miles : public Ordinal<Miles, float> {
public:

	static const float minval = 0;
	static const float maxval = 10e5;

	static const Miles min() { return minval; }
	static const Miles max() { return maxval; }

	Miles(float _miles) : Ordinal<Miles, float>(_miles) {
		if (_miles < minval /*|| _miles > maxval*/) {
			cerr << "Miles: attempted to set illegal value" << endl;
			throw "invalid value";
		}
	}

	bool operator==(const Miles& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }

	Dollars operator*(const DollarsPerMile& v) const
	{ return v.value() * this->value(); }

	Hours operator/(const MilesPerHour& v) const
	{ return this->value() / v.value(); }

	Miles operator+=(const Miles& v)
	{ return Nominal::value_ += v.value_; }
	
};


class Packages : public Ordinal<Packages, unsigned int> {
public:

	static const float minval = 0;
	static const float maxval = 10e5;
	
	static const Packages min() { return minval; }
	static const Packages max() { return maxval; }

	Packages(unsigned int _capacity) : 
		Ordinal<Packages, unsigned int>(_capacity) {
			if (_capacity > maxval) {
				cerr << "MilesPerHour: attempted to set illegal value" << endl;
				throw "invalid value";
			}
		}

	bool operator==(const Packages& v) const
	{ return Nominal::value_ == v.value_; }

	Packages operator+=(const Packages& v)
	{ return value_ += v.value_; }

	Packages operator-=(const Packages& v)
	{ return value_ -= v.value_; }

};

class ShipmentsPerDay : public Ordinal<ShipmentsPerDay, unsigned int> {
public:

	static const float minval = 0;
	static const float maxval = 10e5;
	
	static const ShipmentsPerDay min() { return minval; }
	static const ShipmentsPerDay max() { return maxval; }

	ShipmentsPerDay(unsigned int _shipmentsPerDay) : 
		Ordinal<ShipmentsPerDay, unsigned int>(_shipmentsPerDay) {
			if (_shipmentsPerDay > maxval) {
				cerr << "ShipmentsPerDay: attempted to set illegal value" << endl;
				throw "invalid value";
			}
		}
};


class Shipments : public Ordinal<Shipments, unsigned int> {
public:

	static const float minval = 0;
	static const float maxval = 10e5;
	
	static const Shipments min() { return minval; }
	static const Shipments max() { return maxval; }

	Shipments(unsigned int _shipments) : 
		Ordinal<Shipments, unsigned int>(_shipments) {
			if (_shipments > maxval) {
				cerr << "Shipments: attempted to set illegal value" << endl;
				throw "invalid value";
			}
		}
};

class HoursPerShipment : public Ordinal<HoursPerShipment, float> {
public:

	static const float minval = 0;
	static const float maxval = 24*365;
	
	static const HoursPerShipment min() { return minval; }
	static const HoursPerShipment max() { return maxval; }

	HoursPerShipment(float _hoursPerShipment) : 
		Ordinal<HoursPerShipment, float>(_hoursPerShipment) {
		if (_hoursPerShipment < minval || _hoursPerShipment > maxval) {
			cerr << "HoursPerShipment: attempted to set illegal value" << endl;
			throw "invalid value";
		}
	}
};

enum ExpediteOptions {
	ExpediteNotSupported,
	ExpediteSupported
};

enum TransportationMode {
	TruckMode,
	BoatMode,
	PlaneMode
};

enum LocationType {
	CustomerLocation,
	PortLocation,
	TruckTerminalLocation,
	BoatTerminalLocation,
	PlaneTerminalLocation
};


class Shipment;

class Segment;

class Location : public Fwk::PtrInterface<Location> {
public:

	class Notifiee;

  typedef Fwk::Ptr<Location> Ptr;
  typedef vector<Fwk::Ptr<Segment> >::iterator SegmentIterator;
  typedef vector<Fwk::Ptr<Shipment> >::iterator ShipmentIterator;
  typedef vector<Notifiee *>::iterator NotifieeIterator;

  class Notifiee : public Fwk::PtrInterface<Notifiee> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    Location::Ptr notifier() const { return notifier_; }
    virtual void notifierIs(const Location::Ptr _notifier);

    // Events
    virtual void onShipment(Fwk::Ptr<Shipment> shp) {};

    static Notifiee::Ptr NotifieeIs() {
       Ptr m = new Notifiee();
       return m;
    }

    ~Notifiee();

   protected:
    Notifiee() {}
    Location::Ptr notifier_;

   };

	string name() const { return name_; }
	void nameIs(string name) { name_ = name;}

  void segmentIs(Fwk::Ptr<Segment> seg); 
  Fwk::Ptr<Segment> segment(unsigned index) const;
  void segmentDel(Fwk::Ptr<Segment> seg);
	uint32_t segments() const { return segments_.size(); }
	SegmentIterator segmentIterator() { return segments_.begin(); };

	virtual void shipmentIs(Fwk::Ptr<Shipment> shp);
	ShipmentIterator shipmentIter() { return shipments_.begin(); }
	void shipmentDel(Fwk::Ptr<Shipment> shp);
	uint32_t shipments() const { return shipments_.size(); }

	LocationType type() const { return type_; }

	void notifieeNew(Notifiee *n);
	void notifieeDel(Notifiee *n);


protected:
	Location(string name, LocationType type) : name_(name), type_(type) {};
	string name_;
	LocationType type_;
  vector<Fwk::Ptr<Segment> > segments_;
  vector<Fwk::Ptr<Shipment> > shipments_;
  vector<Notifiee *> notifiee_;
};

class Customer : public Location {
public:
	typedef Fwk::Ptr<Customer> Ptr;
	typedef vector<Notifiee *>::iterator NotifieeIterator;

	static Ptr CustomerIs(string name) {
		Ptr loc = new Customer(name);
		return loc;
	} 

	class Notifiee : public Location::Notifiee {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    virtual void notifierIs(const Customer::Ptr _notifier);
    Customer::Ptr notifier() const { return notifier_; }

    // Events
    virtual void onTransferRate() {};
    virtual void onShipmentSize() {};
    virtual void onDestination() {};
   	virtual void onShipment(Fwk::Ptr<Shipment> shp) {};

    static Notifiee::Ptr NotifieeIs() {
       Ptr m = new Notifiee();
       return m;
    }

    ~Notifiee();

   protected:
    Notifiee() {}
    Customer::Ptr notifier_;
   };

  // void shipmentIs(Fwk::Ptr<Shipment> shp);

	/* source attributes */
  void transferRateIs(ShipmentsPerDay _transferRate);
	ShipmentsPerDay transferRate() const { return transferRate_; }

	void shipmentSizeIs(Packages _shipmentSize);
	Packages shipmentSize() const { return shipmentSize_; }

	void destinationIs(Customer::Ptr _destination);
	Customer::Ptr destination() const { return destination_; }

	/* destination attributes */

	Shipments shipmentsReceived() const { return shipmentsReceived_; }
	void shipmentsReceivedIs(Shipments _shipmentsReceived) 
	{ shipmentsReceived_ = _shipmentsReceived; }
		
	Dollars totalCost() const { return totalCost_; }
	void totalCostIs(Dollars _totalCost) { totalCost_ = _totalCost; }

	Hours totalLatency() const { return totalLatency_; }
	void totalLatencyIs(Hours _totalLatency) 
	{ totalLatency_ = _totalLatency; }


protected:
	Customer(string name) : Location(name, CustomerLocation),
		 transferRate_(0), shipmentSize_(0), destination_(NULL),
			totalCost_(0), totalLatency_(0), shipmentsReceived_(0) {}
	
	ShipmentsPerDay transferRate_;
	Packages shipmentSize_;
	Customer::Ptr destination_;
	Dollars totalCost_;
	Hours totalLatency_;
	Shipments shipmentsReceived_;
};

class Terminal : public Location {
public:	
	typedef Fwk::Ptr<Terminal> Ptr;

protected:
	Terminal(string name, LocationType type) : Location(name, type) {}
};

class Port : public Location {
public:
	typedef Fwk::Ptr<Port> Ptr;

	static Ptr PortIs(string name) {
		Ptr port = new Port(name);
		return port;
	}

protected:
	Port(string name) : Location(name, PortLocation) {}
};

class TruckTerminal : public Terminal {
public:
	typedef Fwk::Ptr<TruckTerminal> Ptr;

	static Ptr TruckTerminalIs(string name) {
		Ptr term = new TruckTerminal(name);
		return term;
	}

protected:
	TruckTerminal(string name) : Terminal(name, TruckTerminalLocation) {}
};

class BoatTerminal : public Terminal {
public:
	typedef Fwk::Ptr<BoatTerminal> Ptr;

	static Ptr BoatTerminalIs(string name) {
		Ptr term = new BoatTerminal(name);
		return term;
	}

protected:
	BoatTerminal(string name) : Terminal(name, BoatTerminalLocation) {}
};

class PlaneTerminal : public Terminal {
public:
	typedef Fwk::Ptr<PlaneTerminal> Ptr;

	static Ptr PlaneTerminalIs(string name) {
		Ptr term = new PlaneTerminal(name);
		return term;
	}

protected:
	PlaneTerminal(string name) : Terminal(name, PlaneTerminalLocation) {}
};


class Segment : public Fwk::PtrInterface<Segment>
{

public:

	class Notifiee;
  
  typedef Fwk::Ptr<Segment> Ptr;
  typedef vector<Notifiee *>::iterator NotifieeIterator;
  typedef vector<Fwk::Ptr<Shipment> >::iterator ShipmentIterator;


  class Notifiee : public Fwk::PtrInterface<Notifiee> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    Segment::Ptr notifier() const { return notifier_; }
    virtual void notifierIs(const Segment::Ptr _notifier);

    // Events
    virtual void onExpedite() {};
    virtual void onShipment(Fwk::Ptr<Shipment> shp) {};
    virtual void onShipmentDel(Fwk::Ptr<Shipment> shp) {};

    static Notifiee::Ptr NotifieeIs() {
       Ptr m = new Notifiee();
       return m;
    }

    ~Notifiee();

   protected:
    Notifiee() {}
    Segment::Ptr notifier_;

   };

	string name() const { return name_; }
	void nameIs(string name) { name_ = name;}

	Location::Ptr source() const { return source_; }
	virtual void sourceIs(Location::Ptr _source);

	Miles length() const { return length_; }
	void lengthIs(Miles _length) { length_ = _length; }

	Location::Ptr destination() const;

	DifficultyLevel difficulty() const { return difficulty_; }
	void difficultyIs(DifficultyLevel _difficulty) { difficulty_ = _difficulty; } 

	ExpediteOptions expedite() const { return expedite_; }
	void expediteIs(ExpediteOptions _expedite);

	Segment::Ptr returnSegment() const { return return_segment_; }
	void returnSegmentIs(Ptr seg);

	void shipmentIs(Fwk::Ptr<Shipment> shp);
	void shipmentDel(Fwk::Ptr<Shipment> shp);

	Shipments shipmentsInFlight() const { return Shipments(shipments_.size()); }

	Shipments shipmentsReceived() const { return shipmentsReceived_; }
	void shipmentsReceivedIs(Shipments _shipmentsReceived) 
	{ shipmentsReceived_ = _shipmentsReceived; }

	Shipments shipmentsRefused() const { return shipmentsRefused_; }
	void shipmentsRefusedIs(Shipments _shipmentsRefused) 
	{ shipmentsRefused_ = _shipmentsRefused; }

	Shipments capacity() const { return capacity_; }
	void capacityIs(Shipments _capacity) { capacity_ = _capacity; }


	TransportationMode mode() const { return mode_; }

	void notifieeNew(Notifiee *n);
	void notifieeDel(Notifiee *n);

protected:
	Segment(string name, TransportationMode mode);
	string name_;
	TransportationMode mode_;
	Location::Ptr source_;
	Miles length_;
	DifficultyLevel difficulty_;
	ExpediteOptions expedite_;
	Ptr return_segment_;
	vector<Fwk::Ptr<Shipment> > shipments_;
	Shipments shipmentsReceived_;
	Shipments shipmentsRefused_;
	Shipments capacity_;
	vector<Notifiee *> notifiee_;

};

class BoatSegment : public Segment {
public:
	typedef Fwk::Ptr<BoatSegment> Ptr;

	static Ptr BoatSegmentIs(string name) {
		Ptr seg = new BoatSegment(name, BoatMode);
		return seg;
	}

virtual void sourceIs(Location::Ptr _source);


protected:
	BoatSegment(string name, TransportationMode mode) : Segment(name, mode) {}
};

//subscribe to notifications from fleet object

class PlaneSegment : public Segment {
public:
	typedef Fwk::Ptr<PlaneSegment> Ptr;

	static Ptr PlaneSegmentIs(string name) {
		Ptr seg = new PlaneSegment(name, PlaneMode);
		return seg;
	}

virtual void sourceIs(Location::Ptr _source);


protected:
	PlaneSegment(string name, TransportationMode mode) : Segment(name, mode) {}
};
//subscribe to notifications from fleet object

class TruckSegment : public Segment {
public:
	typedef Fwk::Ptr<TruckSegment> Ptr;

	static Ptr TruckSegmentIs(string name) {
		Ptr seg = new TruckSegment(name, TruckMode);
		return seg;
	}

virtual void sourceIs(Location::Ptr _source);

protected:
	TruckSegment(string name, TransportationMode mode) : Segment(name, mode) {}
};


class Shipment : public Fwk::PtrInterface<Shipment> {
public:

	typedef Fwk::Ptr<Shipment> Ptr;

	enum Status {
		ready,
		enroute,
		refused,
		delivered
	};

	static Ptr ShipmentIs(Customer::Ptr _source, 
												Customer::Ptr _destination,
												Packages _packages,
												Time _departure) {
		Ptr m = new Shipment(_source, _destination, 
			_packages, _departure);
		return m;
	}

	Customer::Ptr source() const { return source_; }
	Customer::Ptr destination() const { return destination_; }
	Packages packages() const { return packages_; }
	
	Dollars cost() const { return cost_; }
	void costIs(Dollars _cost) { cost_ = _cost; }

	Hours latency() const { return latency_; }
	void latencyIs(Hours _latency) { latency_ = _latency; }

	Time departure() const { return departure_; }

	Status status() const { return status_; }
	void statusIs(Status _status) { status_ = _status; }

protected:
	Shipment(Customer::Ptr _source, Customer::Ptr _destination, 
		Packages _packages, Time _departure) : source_(_source), destination_(_destination),
		status_(enroute), packages_(_packages), cost_(0), latency_(0),
		departure_(_departure) {}

	Customer::Ptr source_;
	Customer::Ptr destination_;
	Status status_;
	Packages packages_;
	Dollars cost_;
	Hours latency_;
	Time departure_;
};


class FleetDesc : public Fwk::PtrInterface<FleetDesc> {

public:

  typedef Fwk::Ptr<FleetDesc> Ptr;

  static FleetDesc::Ptr FleetDescIs() {
     Ptr fleet = new FleetDesc();
     return fleet;
  }

	MilesPerHour speed() const { return speed_; }
	void speedIs(MilesPerHour _speed);

	Packages capacity() const { return capacity_; }
	void capacityIs(Packages _capacity);

	DollarsPerMile costPerMile() const { return costPerMile_; }
	void costPerMileIs(DollarsPerMile _costPerMile);

protected:
	FleetDesc() : speed_(1), capacity_(1), costPerMile_(1) {}
	string name_;
	MilesPerHour speed_;
	Packages capacity_;
	DollarsPerMile costPerMile_;

};

class BoatFleetDesc : public FleetDesc {
public:
	typedef Fwk::Ptr<BoatFleetDesc> Ptr;

  static BoatFleetDesc::Ptr BoatFleetDescIs() {
     Ptr fleet = new BoatFleetDesc();
     return fleet;
  }

protected:
	BoatFleetDesc() : FleetDesc() {}
};

class PlaneFleetDesc : public FleetDesc {
	public:
	typedef Fwk::Ptr<PlaneFleetDesc> Ptr;

  static PlaneFleetDesc::Ptr PlaneFleetDescIs() {
     Ptr fleet = new PlaneFleetDesc();
     return fleet;
  }

protected:
	PlaneFleetDesc() : FleetDesc() {}
};

class TruckFleetDesc : public FleetDesc {
	public:
	typedef Fwk::Ptr<TruckFleetDesc> Ptr;

  static TruckFleetDesc::Ptr TruckFleetDescIs() {
     Ptr fleet = new TruckFleetDesc();
     return fleet;
  }

protected:
	TruckFleetDesc() : FleetDesc() {}
};


class Network : public Fwk::PtrInterface<Network> {
public:

	class Notifiee;
	class RoutingTable;

	typedef Fwk::Ptr<Network> Ptr;
  typedef vector<Notifiee *>::iterator NotifieeIterator;
  typedef map<string, Location::Ptr>::iterator LocationIterator;
  typedef map<string, Segment::Ptr>::iterator SegmentIterator;


	Location::Ptr location(string name);
	LocationIterator locationIter() { return locations_.begin(); }
	LocationIterator locationIterEnd() { return locations_.end(); }
	uint32_t locations() { return locations_.size(); }
	void locationIs(Location::Ptr loc);
	void locationDel(string name);

	Segment::Ptr segment(string name);
	void segmentIs(Segment::Ptr seg);
	void segmentDel(string name);

	BoatFleetDesc::Ptr boatFleet() const { return boat_fleet_; }
	void boatFleetIs(BoatFleetDesc::Ptr _desc) { boat_fleet_ = _desc; }

	TruckFleetDesc::Ptr truckFleet() const { return truck_fleet_; }
	void truckFleetIs(TruckFleetDesc::Ptr _desc) { truck_fleet_ = _desc; }

	PlaneFleetDesc::Ptr planeFleet() const { return plane_fleet_; }
	void planeFleetIs(PlaneFleetDesc::Ptr _plane) { plane_fleet_ = _plane; }

	Fwk::Ptr<RoutingTable> routingTable();

	FleetDesc::Ptr fleet(TransportationMode mode) const {
		switch(mode) {
			case TruckMode: return truckFleet().ptr();
			case BoatMode: return boatFleet().ptr();
			case PlaneMode: return planeFleet().ptr();
		}
		return NULL; //execution shouldn't get here
	};

  static Network::Ptr NetworkIs(string name) {
     Ptr net = new Network(name);
     return net;
  }

  class RoutingTable : public Fwk::PtrInterface<RoutingTable> {
	public:
		typedef Fwk::Ptr<Network::RoutingTable> Ptr;
		Segment::Ptr nextSegment(string source, string dest) { 
			if (rtable_.find(source) == rtable_.end())
				return NULL;

			if (rtable_[source].find(dest) == rtable_[source].end())
				return NULL;

			return rtable_[source][dest]; 
		}
	protected:
		map<string, map<string, Segment::Ptr> > rtable_;
	};

  class Notifiee : public Fwk::PtrInterface<Notifiee> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    Network::Ptr notifier() const { return notifier_; }
    virtual void notifierIs(const Network::Ptr _notifier);

    // Events
    virtual void onLocationNew(Location::Ptr loc) {};
    virtual void onLocationDel(Location::Ptr loc) {};

    virtual void onSegmentNew(Segment::Ptr seg) {};
    virtual void onSegmentDel(Segment::Ptr seg) {};

    static Notifiee::Ptr NotifieeIs() {
       Ptr m = new Notifiee();
       return m;
    }

    ~Notifiee();

   protected:
    Notifiee() {}
    Network::Ptr notifier_;
   };

	void notifieeNew(Notifiee *n);
	void notifieeDel(Notifiee *n);

protected:
	Network(string name) : name_(name), routing_table_(NULL) {};
	~Network();
	string name_;
	map<string, Location::Ptr> locations_;
	map<string, Segment::Ptr> segments_;
	BoatFleetDesc::Ptr boat_fleet_;
	PlaneFleetDesc::Ptr plane_fleet_;
	TruckFleetDesc::Ptr truck_fleet_;
	Fwk::Ptr<RoutingTable> routing_table_;
  vector<Notifiee *> notifiee_;
	

};

class Stats : public Fwk::PtrInterface<Stats> {
public:
	typedef Fwk::Ptr<Stats> Ptr;
	typedef uint32_t EntityCount;	

	inline Network::Ptr network() { return network_; } 
	inline EntityCount customerLocations() { return customer_locations_; }
	inline EntityCount ports() { return ports_; }
	inline EntityCount truckTerminals() { return truck_terminals_; }
	inline EntityCount boatTerminals() { return boat_terminals_; }
	inline EntityCount planeTerminals() { return plane_terminals_; }
	inline EntityCount segments() 
	{ return truck_segments_ + boat_segments_ + plane_segments_; }
	inline EntityCount truckSegments() { return truck_segments_; }
	inline EntityCount boatSegments() { return boat_segments_; }
	inline EntityCount planeSegments() { return plane_segments_; }
	inline EntityCount expedited() { return expedited_; }

protected:
	Network::Ptr network_;
	EntityCount customer_locations_;
	EntityCount ports_;
	EntityCount truck_terminals_;
	EntityCount boat_terminals_;
	EntityCount plane_terminals_;
	EntityCount truck_segments_;
	EntityCount boat_segments_;
	EntityCount plane_segments_;
	EntityCount expedited_;
};

class StatsConfig : public Stats {

public:

	typedef Fwk::Ptr<StatsConfig> Ptr;

	static Ptr StatsConfigIs(Network::Ptr _network) {
		Ptr stats = new StatsConfig(_network);
		return stats;
	}

	class SegmentReactor : public Segment::Notifiee {
	public:
		typedef Fwk::Ptr<StatsConfig::SegmentReactor> Ptr;

		static Ptr ReactorIs(StatsConfig *s) {
			Ptr m = new SegmentReactor(s);
			return m;
		}

		void onExpedite();

	protected:
		SegmentReactor(StatsConfig *s) : 
			owner_(s), oldval(ExpediteNotSupported) {}
		StatsConfig *owner_;
		ExpediteOptions oldval;
	};

	typedef vector<SegmentReactor::Ptr >::iterator SegmentReactorIterator;
	
	void segmentReactorNew(Segment::Ptr);

	void customerLocationsIs(Stats::EntityCount _customer_locations);
	void portsIs(Stats::EntityCount _ports);
	void truckTerminalsIs(Stats::EntityCount _truck_terminals);
	void boatTerminalsIs(Stats::EntityCount _boat_terminals);
	void planeTerminalsIs(Stats::EntityCount _plane_terminals);
	void truckSegmentsIs(Stats::EntityCount _truck_segments);
	void boatSegmentsIs(Stats::EntityCount _boat_segments);
	void planeSegmentsIs(Stats::EntityCount _plane_segments);
	void expeditedIs(Stats::EntityCount _expedited);

protected:
	StatsConfig(Network::Ptr _network);

	class NetworkReactor : public Network::Notifiee {
	public:
		typedef Fwk::Ptr<StatsConfig::NetworkReactor> Ptr;

		static Ptr ReactorIs(StatsConfig *s) {
			Ptr m = new NetworkReactor(s);
			return m;
		}

		void onSegmentNew(Segment::Ptr seg);
		void onSegmentDel(Segment::Ptr seg);
		
		void onLocationNew(Location::Ptr loc);
		void onLocationDel(Location::Ptr loc);

	protected:
		NetworkReactor(StatsConfig *s) : owner_(s) {}
		StatsConfig *owner_;
	};


	NetworkReactor::Ptr net_reactor_;
	vector<SegmentReactor::Ptr> seg_reactor_;

};

class Path : public Fwk::PtrInterface<Path> {

public:
	typedef Fwk::Ptr<Path> Ptr;
	typedef vector<Segment::Ptr>::iterator SegmentIterator;

	static Ptr PathIs() {
		Ptr m = new Path();
		return m;
	}

	Path::Ptr clone() {
		Ptr c = new Path();
		c->start_loc_ = start_loc_;
		c->end_loc_ = end_loc_;
		c->visited_ = visited_;
		c->cost_ = cost_;
		c->time_ = time_;
		c->length_ = length_;
		c->expedite_ = expedite_;
		c->segments_ = segments_;
		return c;
	}

	void segmentNew(Segment::Ptr, FleetDesc::Ptr);
	unsigned segments() { return segments_.size(); }
	SegmentIterator segmentIter() { return segments_.begin(); }

	ExpediteOptions expedite() const { return expedite_; }
	void expediteIs(ExpediteOptions _expedite);

	Location::Ptr end() const {return end_loc_; }
	Location::Ptr start() const {return start_loc_; }

	Dollars cost() const { return cost_; }
	Miles length() const { return length_; }
	Hours time() const { return time_; }

protected:
	Path() : start_loc_(NULL), end_loc_(NULL), cost_(0), time_(0), length_(0),
		expedite_(ExpediteNotSupported) {}
	Location::Ptr start_loc_;
	Location::Ptr end_loc_;
	map<string, bool> visited_;
	Dollars cost_;
	Hours time_;
	Miles length_;
	ExpediteOptions expedite_;
	vector<Segment::Ptr> segments_;

};

class PathList : public Fwk::PtrInterface<PathList> {
public:
	typedef Fwk::Ptr<PathList> Ptr;
	typedef vector<Path::Ptr>::iterator PathIterator;

	static Ptr PathListNew() {
		Ptr m = new PathList();
		return m;
	}

	unsigned paths() const { return paths_.size(); }
	vector<Path::Ptr>::iterator pathIter() { return paths_.begin(); }
	void pathNew(Path::Ptr);

protected:
	PathList() {}
	vector<Path::Ptr> paths_;

};


class Connectivity : public Fwk::PtrInterface<Connectivity> {

public:
	typedef Fwk::Ptr<Connectivity> Ptr;

	static Ptr ConnectivityIs(Network::Ptr net) {
		Ptr m = new Connectivity(net);
		return m;
	}

	Network::Ptr network() const { return net_; }
	PathList::Ptr explore(Location::Ptr start, Miles distance, Dollars cost, 
												Hours time, ExpediteOptions expedite);
	PathList::Ptr connect(Location::Ptr start, Location::Ptr end);

protected:
	Connectivity(Network::Ptr net) { net_ = net; }
	Network::Ptr net_;
	void enqueueNextSegments(queue<Path::Ptr> & q, Path::Ptr& p, 
												 	 ExpediteOptions expedite);
	bool meetPathConstraints(Path::Ptr p, Miles length, 
													 Hours time, Dollars cost);
};


} /* end namespace */



#endif
