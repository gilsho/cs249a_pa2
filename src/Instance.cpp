#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <boost/tokenizer.hpp>
#include "Instance.h"
#include "Engine.h"

typedef boost::tokenizer<boost::char_separator<char> >::iterator Tokenizer;

using namespace std;

namespace Shipping {

//
// Rep layer classes
//

class ManagerImpl : public Instance::Manager {
public:
		ManagerImpl();

		// Manager method
		Ptr<Instance> instanceNew(const string& name, const string& type);

		// Manager method
		Ptr<Instance> instance(const string& name);

		// Manager method
		void instanceDel(const string& name);

		Network::Ptr network() const { return net; }

private:
		map<string,Ptr<Instance> > instance_;
		Network::Ptr net;
};

class LocationRep : public Instance {
public:

		LocationRep(const string& name, ManagerImpl* manager) :
				Instance(name), manager_(manager)
		{
				// Nothing else to do.
		}

		// Instance method
		string attribute(const string& name);

		// Instance method
		void attributeIs(const string& name, const string& v);

protected:
		ManagerImpl *manager_;
		int segmentNumber(const string& name);
		Location::Ptr loc;

};
																																																	
class CustomerRep : public LocationRep {
public:

		CustomerRep(const string& name, ManagerImpl *manager) :
				LocationRep(name, manager)
		{
				loc = Customer::CustomerIs(name);  
				manager_->network()->locationIs(loc); 
		}
};
																																																	
class PortRep : public LocationRep {
public:

		PortRep(const string& name, ManagerImpl *manager) :
				LocationRep(name, manager)
		{
				loc = Port::PortIs(name); 
				manager_->network()->locationIs(loc);   
		}
};
																																																	
class TruckTerminalRep : public LocationRep {
public:

		TruckTerminalRep(const string& name, ManagerImpl *manager) :
				LocationRep(name, manager)
		{
				loc = TruckTerminal::TruckTerminalIs(name);  
				manager_->network()->locationIs(loc);  
		}
};

class BoatTerminalRep : public LocationRep {
public:

		BoatTerminalRep(const string& name, ManagerImpl *manager) :
				LocationRep(name, manager)
		{
				loc = BoatTerminal::BoatTerminalIs(name); 
				manager_->network()->locationIs(loc);   
		}
};

class PlaneTerminalRep : public LocationRep {
public:

		PlaneTerminalRep(const string& name, ManagerImpl *manager) :
				LocationRep(name, manager)
		{
				loc = PlaneTerminal::PlaneTerminalIs(name);  
				manager_->network()->locationIs(loc);  
		}
};

class SegmentRep : public Instance {
public:

		SegmentRep(const string& name, ManagerImpl* manager) :
				Instance(name), manager_(manager)
		{
				// Nothing else to do.
		}

		// Instance method
		string attribute(const string& name);

		// Instance method
		void attributeIs(const string& name, const string& v);

protected:
		ManagerImpl *manager_;
		Segment::Ptr seg;

};

class BoatSegmentRep : public SegmentRep {
public:

		BoatSegmentRep(const string& name, ManagerImpl *manager) :
				SegmentRep(name, manager)
		{
				seg = BoatSegment::BoatSegmentIs(name);   
				manager_->network()->segmentIs(seg); 
		}
};

class TruckSegmentRep : public SegmentRep {
public:

		TruckSegmentRep(const string& name, ManagerImpl *manager) :
				SegmentRep(name, manager)
		{
				seg = TruckSegment::TruckSegmentIs(name);   
				manager_->network()->segmentIs(seg); 
		}
};

class PlaneSegmentRep : public SegmentRep {
public:

		PlaneSegmentRep(const string& name, ManagerImpl *manager) :
				SegmentRep(name, manager)
		{
				seg = PlaneSegment::PlaneSegmentIs(name);   
				manager_->network()->segmentIs(seg);
		}
};


class FleetRep : public Instance {
public:

		FleetRep(const string& name, ManagerImpl* manager) :
				Instance(name), manager_(manager)
		{
				boat_fleet_ = BoatFleetDesc::BoatFleetDescIs();
				plane_fleet_ = PlaneFleetDesc::PlaneFleetDescIs();
				truck_fleet_ = TruckFleetDesc::TruckFleetDescIs();

				Network::Ptr net = manager_->network();
				net->boatFleetIs(boat_fleet_);
				net->truckFleetIs(truck_fleet_);
				net->planeFleetIs(plane_fleet_);
		}

		// Instance method
		string attribute(const string& name);

		// Instance method
		void attributeIs(const string& name, const string& v);

protected:
		Ptr<ManagerImpl> manager_;
		FleetDesc::Ptr selectFleet(const string& s);
		string parseFleet(const string& s);
		string parseAttribute(const string& s);
		BoatFleetDesc::Ptr boat_fleet_;
		PlaneFleetDesc::Ptr plane_fleet_;
		TruckFleetDesc::Ptr truck_fleet_;
};

class StatsRep : public Instance {
public:

		StatsRep(const string& name, ManagerImpl* manager) :
				Instance(name), manager_(manager)
		{
				stats_ = StatsConfig::StatsConfigIs(manager_->network());
		}

		// Instance method
		string attribute(const string& name);

		// Instance method
		void attributeIs(const string& name, const string& v) {}

protected:
		ManagerImpl *manager_;
		Stats::Ptr stats_;
};

class ConnRep : public Instance {
public:

		ConnRep(const string& name, ManagerImpl* manager) :
				Instance(name), manager_(manager)
		{
				conn_ = Connectivity::ConnectivityIs(manager_->network());
		}

		// Instance method
		string attribute(const string& name);

		// Instance method
		void attributeIs(const string& name, const string& v) {}

protected:
		string printPathList(PathList::Ptr plist, bool metaData);
		string printPath(Path::Ptr p);
		string printPathMetaData(Path::Ptr p);
		string printSegment(Segment::Ptr seg);
		bool exploreArgs(Tokenizer& token, Tokenizer& token_end, 
										 Location::Ptr& start, Dollars& maxCost, 
										 Hours& maxTime, Miles& maxLength, 
										 ExpediteOptions& expedite);
		bool connectArgs(Tokenizer& token, Tokenizer& token_end,
				Location::Ptr& start, Location::Ptr& end);

		string exploreQuery(Tokenizer& token, Tokenizer& token_end);
		string connectQuery(Tokenizer& token, Tokenizer& token_end);

		ManagerImpl *manager_;
		Connectivity::Ptr conn_;
};


ManagerImpl::ManagerImpl() {
		net = Network::NetworkIs("network");
		StatsRep *stats = new StatsRep("stats", this);
		instance_["stats"] = stats;

		FleetRep *fleet = new FleetRep("fleet", this);
		instance_["fleet"] = fleet;

		ConnRep *conn = new ConnRep("conn", this);
		instance_["conn"] = conn;

}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
		
		if (instance_.find(name) != instance_.end()) {
			cerr << "Manager: attempted to create instance that already exists " 
					 << "Aborting operation." << endl;
			return NULL;
		}


		if (type == "Customer") {
				Ptr<CustomerRep> t = new CustomerRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Port") {
				Ptr<PortRep> t = new PortRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Truck terminal") {
				Ptr<TruckTerminalRep> t = new TruckTerminalRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Boat terminal") {
				Ptr<BoatTerminalRep> t = new BoatTerminalRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Plane terminal") {
				Ptr<PlaneTerminalRep> t = new PlaneTerminalRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Truck segment") {
				Ptr<TruckSegmentRep> t = new TruckSegmentRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Boat segment") {
				Ptr<BoatSegmentRep> t = new BoatSegmentRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Plane segment") {
				Ptr<PlaneSegmentRep> t = new PlaneSegmentRep(name, this);
				instance_[name] = t;
				return t;
		}

		if (type == "Stats") {
				return instance_["stats"];
		}

		if (type == "Conn") {
				return instance_["conn"];
		}

		if (type == "Fleet") {
				return instance_["fleet"];
		}

		cerr << "Manager: attempted to create instance of invalid type." 
				 << endl;

		return NULL;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
		map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);
		return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
		map<string,Ptr<Instance> >::iterator t = instance_.find(name);
		if(t != instance_.end()) {
				instance_.erase(t);    
		} else {
			cerr << "Manager: attempted to delete nonexistant instance." 
				 	 << endl;
		}
}


string LocationRep::attribute(const string& name) {
		int i = segmentNumber(name);
		if (i < 0 || (unsigned) i >= loc->segments()) {
			cerr << "LocationRep: attempted to read invalid segment" << endl;
			return "";
		}
		Segment::Ptr seg = *(loc->segmentIterator() + i);
		return seg->name();
}


void LocationRep::attributeIs(const string& name, const string& v) {
		cerr << "LocationRep: invalid attempt to set attribute" << endl;
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
		if (name.substr(0, segmentStrlen) == segmentStr) {
				const char* t = name.c_str() + segmentStrlen;
				return atoi(t);
		}
		return 0;
}


string SegmentRep::attribute(const string& name) {

		ostringstream s;
		s << setprecision(2) << fixed;

		if (name == "source") {
				return seg->source()->name();
		}

		if (name == "length") {
				ostringstream s;
				s << seg->length().value();
				return s.str();
		}

		if (name == "return segment") {
				return seg->returnSegment()->name();
		}

		if (name == "difficulty") {
				ostringstream s;
				s << seg->difficulty().value();
				return s.str();
		}

		if (name == "expedite support") {
				if (seg->expedite() == ExpediteSupported) {
						return "yes";
				}
				return "no";
		}

		cerr << "SegmentRep: attempted to access invalid attribute" << endl;
		return "";

}


void SegmentRep::attributeIs(const string& name, const string& v) {

		if (name == "source") {
				Location::Ptr loc = manager_->network()->location(v);
				if (loc == NULL) {
					cerr << "SegmentRep: attempted to set source to invalid location" 
							 << endl;
					return;
				}
				seg->sourceIs(loc);
		}

		else if (name == "length") {
				Miles m = atof(v.c_str());
				seg->lengthIs(m);
		}

		else if (name == "return segment") {
				Segment::Ptr retseg = manager_->network()->segment(v);
				if (seg->mode() != retseg->mode()) {
					cerr << "SegmentRep: return segment has different mode than current"
							 << "segment" << endl;
					return;
				}

				seg->destinationIs(retseg->source());
				retseg->destinationIs(seg->source());
				seg->returnSegmentIs(retseg);
		}

		else if (name == "difficulty") {
				seg->difficultyIs(atof(v.c_str()));
		}

		else if (name == "expedite support") {
				if (v == "yes") {
						seg->expediteIs(ExpediteSupported);
				} else if (v == "no") {
						seg->expediteIs(ExpediteNotSupported);
				} else {
					cerr << "SegmentRep: invalid value for expedite support." << endl;
				}
		}
}

FleetDesc::Ptr FleetRep::selectFleet(const string& s) {
		if (s == "Boat")
				return boat_fleet_;
		if (s == "Plane")
				return plane_fleet_;
		if (s == "Truck")
				return truck_fleet_;
		return NULL;
}

string FleetRep::parseFleet(const string& s) {
		int split = s.find(", ");
		if (split < 0)
				return "";
		return s.substr(0, split);
}

string FleetRep::parseAttribute(const string& s) {
		int split = s.find(", ") + 2;
		if (split < 0)
				return "";
		return s.substr(split);
}

string FleetRep::attribute(const string& name) {
		string str_fleet = parseFleet(name);
		string str_attr = parseAttribute(name);
		FleetDesc::Ptr fleet = selectFleet(str_fleet);
		
		ostringstream s;
		s << setprecision(2) << fixed;
		if (str_attr == "speed") {
				s << fleet->speed().value();
				return s.str();
		}

		if (str_attr == "cost") {
				s << fleet->costPerMile().value();
				return s.str();
		}

		if (str_attr == "capacity") {
				s << fleet->capacity().value();
				return s.str();
		}

		return "";
}


void FleetRep::attributeIs(const string& name, const string& v) {
		string str_fleet = parseFleet(name);
		string str_attr = parseAttribute(name);
		FleetDesc::Ptr fleet = selectFleet(str_fleet);

		ostringstream s;
		if (str_attr == "speed") {
				fleet->speedIs(atof(v.c_str()));
		}

		else if (str_attr == "cost") {
				fleet->costPerMileIs(atof(v.c_str()));
		}

		else if (str_attr == "capacity") {
				fleet->capacityIs(atof(v.c_str()));
		}

}


string StatsRep::attribute(const string& name) {
		
		ostringstream s;
		s << setprecision(2) << fixed;
		if (name == "Customer") {
				s << stats_->customerLocations();
				return s.str();
		}

		if (name == "Port") {
				s << stats_->ports();
				return s.str();
		}

		if (name == "Truck terminal") {
				s << stats_->truckTerminals();
				return s.str();
		}

		if (name == "Boat terminal") {
				s << stats_->boatTerminals();
				return s.str();
		}

		if (name == "Plane terminal") {
				s << stats_->planeTerminals();
				return s.str();
		}

		if (name == "Truck segment") {
				s << stats_->truckSegments();
				return s.str();
		}

		if (name == "Boat segment") {
				s << stats_->boatSegments();
				return s.str();
		} 

		if (name == "Plane segment") {
				s << stats_->planeSegments();
				return s.str();
		}

		if (name == "expedite percentage") {
				float percent = 100.00 * stats_->expedited() / stats_->segments();
				s << percent;
				return s.str();
		}

		return "";
}


bool ConnRep::exploreArgs(Tokenizer& token, Tokenizer& token_end,
													Location::Ptr& start, Dollars& maxCost, 
													Hours& maxTime, Miles& maxLength, 
													ExpediteOptions& expedite)
{
		Network::Ptr net = manager_->network();
		start = net->location(*token++);
		if (start == NULL) return false;
		if (*token++ != ":") return false;
		while (token != token_end) {
				if (*token == "distance") {
						token++;
						maxLength = atof(token->c_str());
						token++;
				} else if (*token == "cost") {
						token++;
						maxCost = atof(token->c_str());
						token++;
				} else if (*token == "time") {
						token++;
						maxTime = atof(token->c_str());
						token++;
				} else if (*token == "expedited") {
						expedite = ExpediteSupported;
						token++;
				} else {
						return false;
				}
		}
		return true;
}

bool ConnRep::connectArgs(Tokenizer& token, Tokenizer& token_end,
													Location::Ptr& start, Location::Ptr& end) {
		Network::Ptr net = manager_->network();
		start = net->location(*token++);
		if (start == NULL) return false;
		if (*token++ != ":") return false;
		end = net->location(*token++);
		if (end == NULL) return false;
		return true;
}

string ConnRep::printPath(Path::Ptr p)
{

	ostringstream s;
	for (Path::SegmentIterator it = p->segmentIter();
			 it != p->segmentIter() + p->segments();
			 ++it) {

		s << printSegment(*it);
	}
	s << (p->end() ? p->end()->name() : "None");
	// if (p->end())
	// 	s <<  p->end()->name();
	// else 
	// 	s << "None";
	return s.str();
}

string ConnRep::printPathMetaData(Path::Ptr p)
{
	ostringstream s;
	s << setprecision(2) << fixed;

	string expString = "no";
	if (p->expedite() == ExpediteSupported) {
		expString = "yes";
	}

	s << p->cost().value() << " " 
		<< p->time().value() << " "  
		<< expString << ";";
	return s.str();
}

string ConnRep::printSegment(Segment::Ptr seg) {
	ostringstream s;
	s << setprecision(2) << fixed;
	string rev = "None";
	if (seg->returnSegment() != NULL) 
		rev = seg->returnSegment()->name();
	
	s << seg->source()->name()
	  << "("
	  << seg->name() << ":" 
	  << seg->length().value() << ":" 
	  << rev
	  << ") ";

	return s.str();

}

string ConnRep::printPathList(PathList::Ptr plist, bool metaData)
{
	ostringstream s;
	for (PathList::PathIterator it = plist->pathIter();
		 it != plist->pathIter() + plist->paths();
		 ++it) {
		if (metaData) {
			s << printPathMetaData(*it) << " ";
		}
		s << printPath(*it) << endl;
	}
	return s.str();
}

string ConnRep::exploreQuery(Tokenizer& token, Tokenizer& token_end) {
		Dollars maxCost(Dollars::max());
		Hours maxTime(Hours::max());
		Miles maxLength(Miles::max());
		ExpediteOptions expedite = ExpediteNotSupported;
		Location::Ptr start;
		if (!exploreArgs(token, token_end, start, maxCost, maxTime, maxLength, 
										 expedite))
				return "";  
		PathList::Ptr plist = conn_->explore(start, maxLength, maxCost, 
																				 maxTime, expedite);    
		return printPathList(plist, false);
}

string ConnRep::connectQuery(Tokenizer& token, Tokenizer& token_end) {
		Location::Ptr start, end;
		if (!connectArgs(token, token_end, start, end))
				return "";
		PathList::Ptr plist = conn_->connect(start, end);

		return printPathList(plist, true);
}

void tokenizeString(string name, string sepstr, Tokenizer& token, 
										Tokenizer& token_end)
{
		boost::char_separator<char> sep(sepstr.c_str());
		boost::tokenizer<boost::char_separator<char> > tokenizedLine(name, sep);
		token = tokenizedLine.begin();
		token_end = tokenizedLine.end();
}

string ConnRep::attribute(const string& name) {

		Tokenizer token;
		Tokenizer token_end;
		tokenizeString(name," ",token, token_end);

		string str = "";
		if (*token == "explore") {
				str = exploreQuery(++token, token_end);
		} else if (*token == "connect") {
				str = connectQuery(++token, token_end);
		}

		if (str == "") {
			cerr << "ConnRep: invalid query" << endl;
		}

		return str;
}


}

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
		return new Shipping::ManagerImpl();
}
