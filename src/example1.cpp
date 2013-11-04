#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {
    Ptr<Instance::Manager> manager = shippingInstanceManager();
    Ptr<Instance> stats = manager->instanceNew("myStats", "Stats");
    Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");

    fleet->attributeIs("Boat, speed", "25");
    fleet->attributeIs("Truck, speed", "60");
    fleet->attributeIs("Plane, speed", "700");
    fleet->attributeIs("Boat, cost", "10");
    fleet->attributeIs("Truck, cost", "5");
    fleet->attributeIs("Plane, cost", "2");
    fleet->attributeIs("Truck, capacity", "50");

    cout << "fleet->attribute('Boat, speed'): " << fleet->attribute("Boat, speed") << endl;
    cout << "fleet->attribute('Truck, speed'): " << fleet->attribute("Truck, speed") << endl;
    cout << "fleet->attribute('Plane, speed'): " << fleet->attribute("Plane, speed") << endl;
    cout << "fleet->attribute('Plane, cost'): " << fleet->attribute("Plane, cost") << endl;
    cout << "fleet->attribute('Truck, cost'): " << fleet->attribute("Truck, cost") << endl;
    cout << "fleet->attribute('Boat, cost'): " << fleet->attribute("Boat, cost") << endl;
    cout << "fleet->attribute('Truck, capacity'): " << fleet->attribute("Truck, capacity") << endl;

    // --- Create instances
    // -- Locations
    Ptr<Instance> sf = manager->instanceNew("sf", "Customer");  
    Ptr<Instance> la = manager->instanceNew("la", "Port");  
    Ptr<Instance> seattle = manager->instanceNew("seattle", "Truck terminal");  
    Ptr<Instance> vancouver = manager->instanceNew("vancouver", "Customer");  
    Ptr<Instance> dallas = manager->instanceNew("dallas", "Plane terminal");  
    Ptr<Instance> nola = manager->instanceNew("nola", "Customer");  
    Ptr<Instance> dc = manager->instanceNew("dc", "Port");  
    Ptr<Instance> tijuana = manager->instanceNew("tijuana", "Customer");  

    // -- Segments
    Ptr<Instance> s11 = manager->instanceNew("s11", "Truck segment");  
    Ptr<Instance> s12 = manager->instanceNew("s12", "Plane segment");  
    Ptr<Instance> s21 = manager->instanceNew("s21", "Truck segment");  
    Ptr<Instance> s22 = manager->instanceNew("s22", "Truck segment");  
    Ptr<Instance> s31 = manager->instanceNew("s31", "Truck segment");  
    Ptr<Instance> s32 = manager->instanceNew("s32", "Plane segment");  
    Ptr<Instance> s41 = manager->instanceNew("s41", "Plane segment");  
    Ptr<Instance> s42 = manager->instanceNew("s42", "Plane segment");  
    Ptr<Instance> s51 = manager->instanceNew("s51", "Plane segment");  
    Ptr<Instance> s52 = manager->instanceNew("s52", "Plane segment");  
    Ptr<Instance> s61 = manager->instanceNew("s61", "Plane segment");  
    Ptr<Instance> s62 = manager->instanceNew("s62", "Plane segment");  
    Ptr<Instance> s71 = manager->instanceNew("s71", "Plane segment");  
    Ptr<Instance> s72 = manager->instanceNew("s72", "Plane segment");  
    Ptr<Instance> s81 = manager->instanceNew("s81", "Truck segment");  
    Ptr<Instance> s82 = manager->instanceNew("s82", "Truck segment");  
    Ptr<Instance> s91 = manager->instanceNew("s91", "Truck segment");  
    Ptr<Instance> s92 = manager->instanceNew("s92", "Truck segment");  
    Ptr<Instance> s101 = manager->instanceNew("s101", "Boat segment");  
    Ptr<Instance> s102 = manager->instanceNew("s102", "Boat segment");  

    // connections
    // customer1 <---> port1
    s11->attributeIs("source", "la");
    s12->attributeIs("source", "tijuana");
    s11->attributeIs("return segment", "s12");

    s21->attributeIs("source", "la");
    s22->attributeIs("source", "seattle");
    s21->attributeIs("return segment", "s22");

    s31->attributeIs("source", "la");
    s32->attributeIs("source", "sf");
    s31->attributeIs("return segment", "s32");

    s41->attributeIs("source", "la");
    s42->attributeIs("source", "dallas");
    s41->attributeIs("return segment", "s42");

    s51->attributeIs("source", "sf");
    s52->attributeIs("source", "dallas");
    s51->attributeIs("return segment", "s52");

    s61->attributeIs("source", "dallas");
    s62->attributeIs("source", "nola");
    s61->attributeIs("return segment", "s62");    

    s71->attributeIs("source", "dallas");
    s72->attributeIs("source", "dc");
    s71->attributeIs("return segment", "s72");

    s81->attributeIs("source", "dc");
    s82->attributeIs("source", "vancouver");
    s81->attributeIs("return segment", "s82");

    s91->attributeIs("source", "sf");
    s92->attributeIs("source", "vancouver");
    s91->attributeIs("return segment", "s92");

    s101->attributeIs("source", "la");
    s102->attributeIs("source", "sf");
    s101->attributeIs("return segment", "s102");

    // -- Segment lengths
    s11->attributeIs("length", "100");
    s12->attributeIs("length", "125");
    s21->attributeIs("length", "150");
    s22->attributeIs("length", "170");
    s31->attributeIs("length", "200");
    s32->attributeIs("length", "220");
    s41->attributeIs("length", "250");
    s42->attributeIs("length", "270");
    s51->attributeIs("length", "300");
    s52->attributeIs("length", "330");
    s61->attributeIs("length", "350");
    s62->attributeIs("length", "400");
    s71->attributeIs("length", "450");
    s72->attributeIs("length", "500");
    s81->attributeIs("length", "520");
    s82->attributeIs("length", "550");
    s91->attributeIs("length", "570");
    s92->attributeIs("length", "600");
    s101->attributeIs("length","620");
    s102->attributeIs("length", "650");  

    // -- Segment difficulties
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "2");
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "4");
    s11->attributeIs("difficulty", "3");
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "2");
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "2");
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "5");
    s11->attributeIs("difficulty", "4");
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "2");
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "3");
    s11->attributeIs("difficulty", "2");
    s11->attributeIs("difficulty", "1");
    s11->attributeIs("difficulty", "4");
    s11->attributeIs("difficulty", "1");

    
    // -- Segment expedite support
    s11->attributeIs("expedite support", "yes");
    s12->attributeIs("expedite support", "yes");
    s31->attributeIs("expedite support", "yes");
    s51->attributeIs("expedite support", "yes");
    s52->attributeIs("expedite support", "yes");
    s71->attributeIs("expedite support", "yes");
    s72->attributeIs("expedite support", "yes");

    // -- Connectivity queries
    Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");


    cout << "**** explore la : distance 1500 ****" << endl;
    cout << conn->attribute("explore la : distance 1500") << endl;
    cout << endl;

    cout << "**** explore la : cost 700 time 30 ****" << endl;
    cout << conn->attribute("explore la : cost 1200 time 30") << endl;
    cout << endl;

    cout << "**** explore dc : distance 1000 cost 2000 time 50 ****" << endl;
    cout << conn->attribute("explore dc : distance 1000 cost 15000 time 50") << endl;
    cout << endl;

    cout << "**** explore dc : distance 1000 cost 2000 time 50 expedited ****" << endl;
    cout << conn->attribute("explore dc : distance 1000 cost 1500 time 35 expedited") << endl;
    cout << endl;

    cout << "*** connect la : sf ****" << endl;
    cout << conn->attribute("connect la : sf") << endl;
    cout << endl;

    cout << "*** connect nola : seattle ****" << endl;
    cout << conn->attribute("connect nola : seattle") << endl;
    cout << endl;

    // -- Statistics queries
    cout << "===== Stats attributes =====" << endl;
    cout << " --- Segments --- " << endl;
    cout << "# Truck segments : " << stats->attribute("Truck segment") << endl;
    cout << "# Plane segments : " << stats->attribute("Plane segment") << endl;
    cout << "# Boat segments : " << stats->attribute("Boat segment") << endl;
    cout << "Expediting %     : " << stats->attribute("expedite percentage") << endl;
    cout << " --- Terminals --- " << endl;
    cout << "# Plane terminals: " << stats->attribute("Plane terminal") << endl;
    cout << "# Boat terminals: " << stats->attribute("Boat terminal") << endl;
    cout << "# Truck terminals: " << stats->attribute("Truck terminal") << endl;
    cout << " --- Ports and customers --- " << endl;
    cout << "# Ports          : " << stats->attribute("Port") << endl;
    cout << "# Customers         : " << stats->attribute("Customer") << endl;

    cerr << "Done!" << endl;

    return 0;
}
