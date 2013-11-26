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
    Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");

    fleet->attributeIs("Boat, speed", "25");
    fleet->attributeIs("Boat, cost", "10");

    fleet->attributeIs("Truck, speed", "60");
    fleet->attributeIs("Truck, cost", "5");
    fleet->attributeIs("Truck, capacity", "50");

    fleet->attributeIs("Plane, speed", "700");
    fleet->attributeIs("Plane, cost", "2");

    Ptr<Instance> la = manager->instanceNew("la", "Customer");  
    Ptr<Instance> sf = manager->instanceNew("sf", "Truck terminal");  
    Ptr<Instance> seattle = manager->instanceNew("seattle", "Truck terminal");  
    Ptr<Instance> vancouver = manager->instanceNew("vancouver", "Customer");  

    Ptr<Instance> s11 = manager->instanceNew("s11", "Truck segment");  
    Ptr<Instance> s12 = manager->instanceNew("s12", "Truck segment");  
    Ptr<Instance> s21 = manager->instanceNew("s21", "Truck segment");  
    Ptr<Instance> s22 = manager->instanceNew("s22", "Truck segment");  
    Ptr<Instance> s31 = manager->instanceNew("s31", "Truck segment");  
    Ptr<Instance> s32 = manager->instanceNew("s32", "Truck segment");  
    // Ptr<Instance> s41 = manager->instanceNew("s41", "Truck segment");  
    // Ptr<Instance> s42 = manager->instanceNew("s42", "Truck segment");  

    // -- Segment lengths
    s11->attributeIs("length", "100");
    s12->attributeIs("length", "150");
    s21->attributeIs("length", "200");
    s22->attributeIs("length", "250");
    s31->attributeIs("length", "300");
    s32->attributeIs("length", "350");
    // s41->attributeIs("length", "400");
    // s42->attributeIs("length", "450");

    // -- Connections
    s11->attributeIs("source", "la");
    s12->attributeIs("source", "sf");
    s11->attributeIs("return segment", "s12");

    s21->attributeIs("source", "sf");
    s22->attributeIs("source", "seattle");
    s21->attributeIs("return segment", "s22");

    s31->attributeIs("source", "seattle");
    s32->attributeIs("source", "vancouver");
    s31->attributeIs("return segment", "s32");

    // --shipment settings
    la->attributeIs("transfer rate", "12");
    la->attributeIs("shipment size", "5");
    la->attributeIs("destination", "vancouver");

    vancouver->attributeIs("transfer rate", "12");
    vancouver->attributeIs("shipment size", "5");
    vancouver->attributeIs("destination", "la");

    manager->attributeIs("time", "100");
    Ptr<Instance> s41 = manager->instanceNew("s41", "Truck segment");
    Ptr<Instance> s42 = manager->instanceNew("s42", "Truck segment");
    // s41->attributeIs("length", "20");
    // s42->attributeIs("length", "20");
    // s41->attributeIs("source", "vancouver");
    // s42->attributeIs("source", "sf");
    // s41->attributeIs("return segment", "s42");

    // manager->attributeIs("time", "150");

    // Check stats
    cout << "Shipments received/refused in s11: "
        << s11->attribute("shipments received") << "/"
        << s11->attribute("shipments refused")
        << endl;

    cout << "Shipments received/refused in s12: "
        << s12->attribute("shipments received") << "/"
        << s12->attribute("shipments refused")
        << endl;

    cout << "Shipments received/refused in s21: "
        << s21->attribute("shipments received") << "/"
        << s21->attribute("shipments refused")
        << endl;

    cout << "Shipments received/refused in s22: "
        << s22->attribute("shipments received") << "/"
        << s22->attribute("shipments refused")
        << endl;

    cout << "Shipments received/refused in s31: "
        << s31->attribute("shipments received") << "/"
        << s31->attribute("shipments refused")
        << endl;

    cout << "Shipments received/refused in s32: "
        << s32->attribute("shipments received") << "/"
        << s32->attribute("shipments refused")
        << endl;

    cout << "Shipments received/refused in s41: "
        << s41->attribute("shipments received") << "/"
        << s41->attribute("shipments refused")
        << endl;

    cout << "Shipments received/refused in s42: "
        << s42->attribute("shipments received") << "/"
        << s42->attribute("shipments refused")
        << endl;


    cout << "received/latency/cost in Vancouver: " 
        << vancouver->attribute("shipments received") << "/"
        << vancouver->attribute("average latency") << "/"
        << vancouver->attribute("total cost")
        << endl;

    cout << "received/latency/cost in LA: " 
        << la->attribute("shipments received") << "/"
        << la->attribute("average latency") << "/"
        << la->attribute("total cost")
        << endl;
    
    return 0;
}
