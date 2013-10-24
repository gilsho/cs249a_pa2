#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"

using Fwk::Nominal;

namespace Shipping {


class Cost : Ordinal {
public:
	Cost(unsigned int num) {
		
	}
};

class Time : Ordinal {};

class Location 
{

public:
	Segment segment() const;

protected:
	vector<Segment> segments_;


//subscribe to notifications from segments that are added to it.
};

class CustomerLocation : Location {};

class Terminal : Location {};

class TruckTerminal : Terminal {};

class BoatTerminal : Terminal {};

class PlaneTerminal : Terminal {};

class Port : Location {};


class Segment 
{

	class Difficulty : Ordinal
	{
		DifficultyLevel(float val) {
			throw "illegal difficulty value";
		}
	};

	enum ExpediteOptions {
		ExpediteNotSupported,
		ExpediteSupported
	};

public:
	Location source() const;
	void sourceIs(Location loc);

	Nominal length() const;
	void lengthIs(Nominal);

	Segment destination() const;
	void destinationIs(Segment seg);

	DifficultyLevel difficulty() const;
	void difficultyIs(DifficultyLevel dif);

	ExpediteOptions expedite() const;
	void expediteIs(ExpediteOptions exp_opt);

protected:
	Time time_;
	Cost cost_;

	Capacity capcity_;

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
	Speed speed() const;
	void speed(Nominal spd);

	Capcity capcity() const;
	void capcityIs(Nominal cap);

	Cost costPerMile() const;
	void costPerMile(Nominal cst);
};

class BoatFleet : Fleet {};

class PlaneFleet : Fleet {};

class TruckFleet : Fleet {};

} /* end namespace */



#endif
