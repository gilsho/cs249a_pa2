
#define INSTANCE_OF(x, C) (dynamic_cast<C>((x)) != 0)

#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <stdint.h>
#include "PtrInterface.h"
#include "Ptr.h"
#include "Instance.h"
#include "Nominal.h"

#include <typeinfo>

using std::vector;
using std::cerr;
using std::map;

namespace Shipping {


class Minutes : Ordinal<Minutes, unsigned int> {
public:
	Minutes(unsigned int _minutes) : Ordinal<Minutes, unsigned int>(_minutes) {}

	bool operator==(const Minutes& v) const
	{ return Nominal::value_ == v.value_; }
};

class Miles : Ordinal<Miles, unsigned int> {
public:
	Miles(unsigned int _miles) : Ordinal<Miles, unsigned int>(_miles) {}

	bool operator==(const Miles& v) const
	{ return Nominal::value_ == v.value_; }

};

class Dollars : Ordinal<Dollars, float> {
public:
	Dollars(float _dollars) : Ordinal<Dollars, float>(_dollars) {}
		
	bool operator==(const Dollars& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }

};

class MilesPerHour : Ordinal<MilesPerHour, float> {
public:
	MilesPerHour(float _milesPerHour) : Ordinal<MilesPerHour, float>(_milesPerHour) {}

	bool operator==(const MilesPerHour& v) const
	{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }
};

class PackageCapacity : Ordinal<PackageCapacity, unsigned int> {
public:
	PackageCapacity(unsigned int _capacity) : Ordinal<PackageCapacity, unsigned int>(_capacity) {}

	bool operator==(const PackageCapacity& v) const
	{ return Nominal::value_ == v.value_; }
};

class Segment;

class Location : public Fwk::PtrInterface<Location> {
public:

  typedef Fwk::Ptr<Location> Ptr;
  typedef vector<Fwk::Ptr<Segment> >::iterator SegmentIterator;

  static Location::Ptr LocationIs(string name) {
     Ptr loc = new Location(name);
     return loc;
  }

	string name() const { return name_; }
	void nameIs(string name) { name_ = name;}

  void segmentIs(Segment *seg); //use a weak pointer to avoid circular references
	uint32_t segments() const;
	SegmentIterator segmentIterator() { return segment_.begin(); };

protected:
	Location(string name) : name_(name) {};
	string name_;
  vector<Fwk::Ptr<Segment> > segment_;

//subscribe to notifications from segments that are added to it.
};

class CustomerLocation : public Location {
public:
	typedef Fwk::Ptr<CustomerLocation> Ptr;

	static Ptr CustomerLocationIs(string name) {
		Ptr loc = new CustomerLocation(name);
		return loc;
	} 

protected:
	CustomerLocation(string name) : Location(name) {}
};

class Terminal : public Location {
public:	
	typedef Fwk::Ptr<Terminal> Ptr;

	static Ptr TerminalIs(string name) {
		Ptr term = new Terminal(name);
		return term;
	}

protected:
	Terminal(string name) : Location(name) {}
};

class Port : public Location {
public:
	typedef Fwk::Ptr<Port> Ptr;

	static Ptr PortIs(string name) {
		Ptr port = new Port(name);
		return port;
	}

protected:
	Port(string name) : Location(name) {}
};

class TruckTerminal : public Terminal {
public:
	typedef Fwk::Ptr<TruckTerminal> Ptr;

	static Ptr TruckTerminalIs(string name) {
		Ptr term = new TruckTerminal(name);
		return term;
	}

protected:
	TruckTerminal(string name) : Terminal(name) {}
};

class BoatTerminal : public Terminal {
public:
	typedef Fwk::Ptr<BoatTerminal> Ptr;

	static Ptr BoatTerminalIs(string name) {
		Ptr term = new BoatTerminal(name);
		return term;
	}

protected:
	BoatTerminal(string name) : Terminal(name) {}
};

class PlaneTerminal : public Terminal {
public:
	typedef Fwk::Ptr<PlaneTerminal> Ptr;

	static Ptr PlaneTerminalIs(string name) {
		Ptr term = new PlaneTerminal(name);
		return term;
	}

protected:
	PlaneTerminal(string name) : Terminal(name) {}
};


class Segment : public Fwk::PtrInterface<Segment>
{

public:

	class Notifiee;
  
  typedef Fwk::Ptr<Segment> Ptr;
  typedef vector<Notifiee *>::iterator NotifieeIterator;


  class Notifiee : public Fwk::PtrInterface<Notifiee> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    Segment::Ptr notifier() const { return notifier_; }
    virtual void notifierIs(const Segment::Ptr _notifier);

    // Events
    virtual void onExpedite() {};

    static Notifiee::Ptr NotifieeIs() {
       Ptr m = new Notifiee();
       return m;
    }

    ~Notifiee();

   protected:
    Notifiee() {}
    Segment::Ptr notifier_;

   };

	class DifficultyLevel : Ordinal<DifficultyLevel, float> 
	{
	public:
		DifficultyLevel(float difficulty) : Ordinal<DifficultyLevel, float>(difficulty) {
			if (difficulty < 1.0 || difficulty > 5.0) {
				throw "illegal difficulty value";
			}
		}

		bool operator==(const DifficultyLevel& v) const
		{ return std::abs(Nominal::value_ - v.value_) < 1e-2; }
	};

	enum ExpediteOptions {
		ExpediteNotSupported,
		ExpediteSupported
	};

  static Segment::Ptr SegmentIs(string name, Location::Ptr _source, 
  															Location::Ptr _destination) {
  	Ptr m = new Segment(name);
  	return m;
  };

	string name() const { return name_; }
	void nameIs(string name) { name_ = name;}

	Location::Ptr source() const { return source_; }
	// void sourceIs(Location loc);

	Miles length() const { return length_; }
	void lengthIs(Miles miles);

	Location::Ptr destination() const { return destination_; }
	// void destinationIs(Segment seg);

	DifficultyLevel difficulty() const { return difficulty_; }
	void difficultyIs(DifficultyLevel dif);

	ExpediteOptions expedite() const { return expedite_; }
	void expediteIs(ExpediteOptions _expedite);

	void notifieeNew(Notifiee *n);
	void notifieeDel(Notifiee *n);

protected:
	Segment(string name);
	string name_;
	Fwk::Ptr<Location> source_;
	Fwk::Ptr<Location> destination_;
	Miles length_;
	DifficultyLevel difficulty_;
	ExpediteOptions expedite_;
	vector<Notifiee *> notifiee_;

};

class BoatSegment : public Segment {
public:
	typedef Fwk::Ptr<BoatSegment> Ptr;

	static Ptr BoatSegmentIs(string name) {
		Ptr seg = new BoatSegment(name);
		return seg;
	}

protected:
	BoatSegment(string name) : Segment(name) {}


	// BoatSegment(Location::Ptr& _source, Location::Ptr& _destination) {
	// 	if (INSTANCE_OF	(_source.ptr(), TruckTerminal*) ||
	// 		  INSTANCE_OF(_source.ptr(), PlaneTerminal*) ||
	// 		  INSTANCE_OF(_destination.ptr(), TruckTerminal*) ||
	// 		  INSTANCE_OF(_destination.ptr(), PlaneTerminal*)) {
	// 		throw "illegal boat segment";
	// 	}
	// }
};
//subscribe to notifications from fleet object

class PlaneSegment : public Segment {
public:
	typedef Fwk::Ptr<PlaneSegment> Ptr;

	static Ptr PlaneSegmentIs(string name) {
		Ptr seg = new PlaneSegment(name);
		return seg;
	}

protected:
	PlaneSegment(string name) : Segment(name) {}
};
//subscribe to notifications from fleet object

class TruckSegment : public Segment {
public:
	typedef Fwk::Ptr<TruckSegment> Ptr;

	static Ptr TruckSegmentIs(string name) {
		Ptr seg = new TruckSegment(name);
		return seg;
	}

protected:
	TruckSegment(string name) : Segment(name) {}
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

	PackageCapacity capacity() const { return capacity_; }
	void capacityIs(PackageCapacity _capacity);

	Dollars costPerMile() const { return costPerMile_; }
	void costPerMileIs(Dollars _costPerMile);

protected:
	FleetDesc() : speed_(0), capacity_(0), costPerMile_(0) {}
	string name_;
	MilesPerHour speed_;
	PackageCapacity capacity_;
	Dollars costPerMile_;

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

class Path {};

class Network : public Fwk::PtrInterface<Network> {
public:

	class Notifiee;

	typedef Fwk::Ptr<Network> Ptr;
  typedef vector<Notifiee *>::iterator NotifieeIterator;


	Location::Ptr location(string name);
	void locationIs(Location::Ptr loc);
	void locationDel(string name);

	Segment::Ptr segment(string name);
	void segmentIs(Segment::Ptr seg);
	void segmentDel(string name);

  static Network::Ptr NetworkIs(string name) {
     Ptr net = new Network(name);
     return net;
  }

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
	Network(string name) : name_(name) {};
	string name_;
	map<string, Location::Ptr> locations_;
	map<string, Segment::Ptr> segments_;
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
		SegmentReactor(StatsConfig *s) : owner_(s) {}
		StatsConfig *owner_;
		Segment::ExpediteOptions oldval;
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

class Connectivity {};


} /* end namespace */



#endif
