
#define INSTANCE_OF(x, C) (dynamic_cast<C>((x)) != 0)

#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
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
};

class Miles : Ordinal<Miles, unsigned int> {
public:
	Miles(unsigned int _miles) : Ordinal<Miles, unsigned int>(_miles) {}
};

class Dollars : Ordinal<Dollars, float> {
public:
	Dollars(float _dollars) : Ordinal<Dollars, float>(_dollars) {}
};

class MilesPerHour : Ordinal<MilesPerHour, float> {
public:
	MilesPerHour(float _milesPerHour) : Ordinal<MilesPerHour, float>(_milesPerHour) {}
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

class CustomerLocation : Location {
public:
	typedef Fwk::Ptr<CustomerLocation> Ptr;
};

class Terminal : Location {
public:	
	typedef Fwk::Ptr<Terminal> Ptr;
};

class Port : Location {
public:
	typedef Fwk::Ptr<Port> Ptr;
};

class TruckTerminal : Terminal {
public:
	typedef Fwk::Ptr<TruckTerminal> Ptr;
};

class BoatTerminal : Terminal {
public:
	typedef Fwk::Ptr<BoatTerminal> Ptr;
};

class PlaneTerminal : Terminal {
public:
	typedef Fwk::Ptr<PlaneTerminal> Ptr;
};

class Segment : public Fwk::PtrInterface<Location>
{

public:

  typedef Fwk::Ptr<Segment> Ptr;

	class PackageCapacity : Ordinal<PackageCapacity, unsigned int> {
	public:
		PackageCapacity(unsigned int _capacity) : Ordinal<PackageCapacity, unsigned int>(_capacity) {}
	};

	class DifficultyLevel : Ordinal<DifficultyLevel, float> 
	{
	public:
		DifficultyLevel(float difficulty) : Ordinal<DifficultyLevel, float>(difficulty) {
			if (difficulty < 1.0 || difficulty > 5.0) {
				throw "illegal difficulty value";
			}
		}
	};

	enum ExpediteOptions {
		ExpediteNotSupported,
		ExpediteSupported
	};

  static Segment::Ptr SegmentIs(string name, Location::Ptr& _source, 
  															Location::Ptr& _destination) {
  	Ptr m = new Segment(name);
  	return m;
  };

	string name() const { return name_; }
	void nameIs(string name) { name_ = name;}

	Location source() const;
	// void sourceIs(Location loc);

	Miles length() const;
	void lengthIs(Miles miles);

	Segment destination() const;
	// void destinationIs(Segment seg);

	DifficultyLevel difficulty() const;
	void difficultyIs(DifficultyLevel dif);

	ExpediteOptions expedite() const;
	void expediteIs(ExpediteOptions exp_opt);

protected:
	Segment(string name);
	string name_;
	Fwk::Ptr<Location> source_;
	Fwk::Ptr<Location> destination_;
	Minutes time_;
	Dollars cost_;
	PackageCapacity capacity_;
	Miles length_;
	DifficultyLevel difficulty_;
	ExpediteOptions expedite_;

};

class BoatSegment : Segment {
public:
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

class PlaneSegment : Segment {};
//subscribe to notifications from fleet object

class TruckSegment : Segment {};
//subscribe to notifications from 

class Network : public Fwk::PtrInterface<Network> {
public:

	class Notifiee;

	typedef Fwk::Ptr<Network> Ptr;
  typedef vector<Notifiee *>::iterator NotifieeIterator;


	Location::Ptr location(string name);
	void locationIs(Location::Ptr& loc);

	Segment::Ptr segment(string name);
	void segmentIs(Segment::Ptr& seg);

  static Network::Ptr NetworkIs(string name) {
     Ptr net = new Network(name);
     return net;
  }

  class Notifiee : public Fwk::PtrInterface<Notifiee> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    Network::Ptr notifier() const { return notifier_; }
    virtual void notifierIs(const Network::Ptr& _notifier);

    // Events
    virtual void onLocationNew(Location::Ptr loc) {};
    virtual void onSegmentNew(Segment::Ptr seg) {};


    static Notifiee::Ptr notifieeIs() {
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
  vector<Notifiee *> notifiee_;
	map<string, Location::Ptr> locations_;
	map<string, Segment::Ptr> segments_;
};


class Fleet {

public:

   typedef Fwk::Ptr<Fleet> Ptr;

	MilesPerHour speed() const;
	void speed(MilesPerHour _speed);

	Segment::PackageCapacity capcity() const;
	void capcityIs(Segment::PackageCapacity _capacity);

	Dollars costPerMile() const;
	void costPerMile(Dollars _costPerMile);

protected:

	MilesPerHour speed_;
	Segment::PackageCapacity capacity_;
	Dollars costPerMile_;


};

class BoatFleet : Fleet {};

class PlaneFleet : Fleet {};

class TruckFleet : Fleet {};

class Path {};

class Stats {};

class Connectivity {};



} /* end namespace */



#endif
