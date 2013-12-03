#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include "Instance.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using boost::lexical_cast;

int main(int argc, char *argv[]) {

  Ptr<Instance::Manager> manager = shippingInstanceManager();
  Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");

  fleet->attributeIs("Truck, speed", "60");
  fleet->attributeIs("Truck, cost", "5");
  fleet->attributeIs("Truck, capacity", "100");

  Ptr<Instance> top = manager->instanceNew("top", "Customer");  
  Ptr<Instance> topterm = manager->instanceNew("top-term", "Truck terminal");  

  Ptr<Instance> topsegu = manager->instanceNew("topseg-u", "Truck segment");  
  Ptr<Instance> topsegd = manager->instanceNew("topseg-d", "Truck segment");  

  topsegu->attributeIs("length", "100");
  topsegd->attributeIs("length", "100");

  topsegd->attributeIs("source", "top");
  topsegu->attributeIs("source", "top-term");
  topsegu->attributeIs("return segment", "topseg-d");



  vector<Ptr<Instance> > midsegdown;
  vector<Ptr<Instance> > midsegup;
  vector<Ptr<Instance> > mid;
  vector<Ptr<Instance> > botsegup;
  vector<Ptr<Instance> > botsegdown;
  vector<Ptr<Instance> > bottom;

  float bot_recv_avg = 0,
        bot_ref_avg = 0,
        mid_recv_avg = 0,
        mid_ref_avg = 0,
        recv_avg = 0,
        ref_avg = 0;

  for (int i=0; i<10; i++) {
    string midname = "mid-" + lexical_cast<string>(i);
    string segdown_name = "midseg-d" + lexical_cast<string>(i);
    string segup_name = "midseg-u" + lexical_cast<string>(i);
    Ptr<Instance> term = manager->instanceNew(midname, 
      "Truck terminal");  
    Ptr<Instance> segdown = manager->instanceNew(segdown_name, "Truck segment");  
    Ptr<Instance> segup = manager->instanceNew(segup_name, "Truck segment");  

    segup->attributeIs("length", "100");
    segdown->attributeIs("length", "100");

    segdown->attributeIs("source", "top-term");
    segup->attributeIs("source", midname);
    segup->attributeIs("return segment", segdown_name);

    mid.push_back(term);
    midsegdown.push_back(segdown);
    midsegup.push_back(segup);
  }

  for (int i=0; i<100; i++) {
    string midname = "mid-" + lexical_cast<string>(i/10);
    string name = "bottom-" + lexical_cast<string>(i);
    string segdown_name = "botseg-d" + lexical_cast<string>(i);
    string segup_name = "botseg-u" + lexical_cast<string>(i);

    Ptr<Instance> cust = manager->instanceNew(name, 
      "Customer");  

    Ptr<Instance> segdown = manager->instanceNew(segdown_name, 
      "Truck segment");  
    Ptr<Instance> segup = manager->instanceNew(segup_name, 
      "Truck segment");  

    segup->attributeIs("length", "100");
    segdown->attributeIs("length", "100");

    segdown->attributeIs("source", midname);
    segup->attributeIs("source", name);
    segup->attributeIs("return segment", segdown_name);

    bottom.push_back(cust);
    botsegdown.push_back(segdown);
    botsegup.push_back(segup);
  }

  // start shipping
  for (int i=0; i<100; i++) {
    Ptr<Instance> cust = bottom[i];

    cust->attributeIs("transfer rate", "4");
    cust->attributeIs("shipment size", "100");
    // int r = rand() % 1000;
    // cust->attributeIs("shipment size", lexical_cast<string>(r));
    cust->attributeIs("destination", "top");
  }

  // manager->attributeIs("scale", "1000");
  manager->attributeIs("time", "100");

  // print statistics of segments
  for (int i=0; i<100; i++) {
    Ptr<Instance> seg = botsegdown[i];
    string name = "botseg-d-" + lexical_cast<string>(i);
    cout << "Shipments received/refused in " << name << ": "
      << seg->attribute("shipments received") << "/"
      << seg->attribute("shipments refused")
      << endl;
  }

  // print statistics of segments
  for (int i=0; i<10; i++) {
    string name = "midseg-d-" + lexical_cast<string>(i);
    Ptr<Instance> seg = midsegdown[i];
    cout << "Shipments received/refused in " << name << ": "
      << seg->attribute("shipments received") << "/"
      << seg->attribute("shipments refused")
      << endl;
  }

    cout << "Shipments received/refused in " << "topseg-d" << ": "
      << topsegd->attribute("shipments received") << "/"
      << topsegd->attribute("shipments refused")
      << endl;

  for (int i=0; i<100; i++) {
    Ptr<Instance> seg = botsegup[i];
    string name = "botseg-u-" + lexical_cast<string>(i);
    cout << "Shipments received/refused in " << name << ": "
      << seg->attribute("shipments received") << "/"
      << seg->attribute("shipments refused")
      << endl;
    bot_recv_avg += atof(seg->attribute("shipments received").c_str());
    bot_ref_avg += atof(seg->attribute("shipments refused").c_str());
  }

  recv_avg += bot_recv_avg;
  ref_avg += bot_ref_avg;

  bot_recv_avg /= 100;
  bot_ref_avg /= 100;

  // print statistics of segments
  for (int i=0; i<10; i++) {
    string name = "midseg-u-" + lexical_cast<string>(i);
    Ptr<Instance> seg = midsegup[i];
    cout << "Shipments received/refused in " << name << ": "
      << seg->attribute("shipments received") << "/"
      << seg->attribute("shipments refused")
      << endl;
    mid_recv_avg += atof(seg->attribute("shipments received").c_str());
    mid_ref_avg += atof(seg->attribute("shipments refused").c_str());
  }

  recv_avg += mid_recv_avg;
  ref_avg += mid_ref_avg;

  mid_recv_avg /= 10;
  mid_ref_avg /= 10;

  recv_avg += atof(topsegu->attribute("shipments received").c_str());
  ref_avg += atof(topsegu->attribute("shipments refused").c_str());

  recv_avg /= 111;
  ref_avg /= 111;

  cout << "Shipments received/refused in " << "topseg-u" << ": "
      << topsegu->attribute("shipments received") << "/"
      << topsegu->attribute("shipments refused")
      << endl;

  cout << "received/latency/cost in top: " 
      << top->attribute("shipments received") << "/"
      << top->attribute("average latency") << "/"
      << top->attribute("total cost")
      << endl;

  cout << "bottom segment received/refused avg: " << bot_recv_avg << "/" 
       << bot_ref_avg << endl;
      
  cout << "mid segment received/refused avg: " << mid_recv_avg <<"/"
       << mid_ref_avg << endl;

  cout << "all received/refused avg: " << recv_avg << "/" << ref_avg << endl;

  return 0;
}
