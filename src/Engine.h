#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include "PtrInterface.h"
#include "Ptr.h"
#include "Instance.h"
#include "Nominal.h"

#include <typeinfo>



using std::vector;
using std::cerr;

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

	class Notifiee;

  typedef Fwk::Ptr<Location> Ptr;
  typedef vector<Fwk::Ptr<Segment> >::iterator SegmentIterator;
  typedef vector<Notifiee *>::iterator NotifieeIterator;

  static Location::Ptr LocationIs(string name) {
     Ptr loc = new Location(name);
     return loc;
  }

  class Notifiee : public Fwk::PtrInterface<Location> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    Location::Ptr notifier() const { return notifier_; }
    virtual void notifierIs(const Location::Ptr& _notifier);

    // Events
    virtual void onSegment(Fwk::Ptr<Segment> seg) {};


    static Notifiee::Ptr notifieeIs() {
       Ptr m = new Notifiee();
       return m;
    }

    ~Notifiee();

   protected:
    Notifiee() {}
    Location::Ptr notifier_;
   };

  void segmentIs(Segment *seg); //use a weak pointer to avoid circular references
	uint32_t segments() const;
	SegmentIterator segmentIterator() { return segment_.begin(); };

	void notifieeNew(Notifiee *n);
	void notifieeDel(Notifiee *n);

protected:
	 Location(string name) : name_(name) {};
	 string name_;
   vector<Notifiee *> notifiee_;
   vector<Fwk::Ptr<Segment> > segment_;

//subscribe to notifications from segments that are added to it.
};

class CustomerLocation : Location {};

class Terminal : Location {};

class Port : Location {};

class TruckTerminal : Terminal {};

class BoatTerminal : Terminal {};

class PlaneTerminal : Terminal {};

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

	Segment();

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
	Fwk::Ptr<Location> source_;
	Fwk::Ptr<Location> destination_;
	Minutes time_;
	Dollars cost_;
	PackageCapacity capacity_;
	Miles length_;
	DifficultyLevel difficulty_;
	ExpediteOptions expedite_;

};

class BoatSegment : Segment {};
//subscribe to notifications from fleet object

class PlaneSegment : Segment {};
//subscribe to notifications from fleet object

class TruckSegment : Segment {};
//subscribe to notifications from 

class Stats {};

class Connectivity {};

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

} /* end namespace */



#endif
