#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "fwk/NamedInterface.h"
#include "fwk/BaseNotifiee.h"
#include "fwk/PtrInterface.h"
#include "fwk/Ptr.h"
#include "Instance.h"
#include "Nominal.h"
//#include "Ptr.h"
//#include "PtrInterface.h"

using std::vector;
using std::cerr;

namespace Shipping {


class Minutes : Ordinal<Minutes, unsigned int> {};

class Miles : Ordinal<Miles, unsigned int> {};

class Dollars : Ordinal<Dollars, float> {};

class MilesPerHour : Ordinal<MilesPerHour, float> {};


class Segment;

class Location : public Fwk::NamedInterface {

public:

	typedef Fwk::Ptr<Location const> PtrConst;
  typedef Fwk::Ptr<Location> Ptr;

	class NotifieeConst : public virtual Fwk::NamedInterface::NotifieeConst {
   public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      typedef Fwk::Ptr<NotifieeConst> Ptr;
      Fwk::String name() const { return (notifier_?notifier_->name():Fwk::String()); }
      Location::PtrConst notifier() const { return notifier_; }
      bool isNonReferencing() const { return isNonReferencing_; }
      NotifieeConst const * lrNext() const { return lrNext_; }
      NotifieeConst * lrNext() { return lrNext_; }
      // Non-const interface =============================================
       ~NotifieeConst();
      virtual void notifierIs(const Location::PtrConst& _notifier);
      void isNonReferencingIs(bool _isNonReferencing);
      virtual void onLocationNew( Location::Ptr ) {};
      virtual void onLocationDel( Location::Ptr ) {};
      void lrNextIs(NotifieeConst * _lrNext) {
         lrNext_ = _lrNext;
      }
      static NotifieeConst::Ptr NotifieeConstIs() {
         Ptr m = new NotifieeConst();
         m->referencesDec(1);
         // decr. refer count to compensate for initial val of 1
         return m;
      }
      // Constructors ====================================================
   protected:
      Location::PtrConst notifier_;
      bool isNonReferencing_;
      NotifieeConst * lrNext_;
      NotifieeConst(): Fwk::NamedInterface::NotifieeConst(),
            isNonReferencing_(false),
            lrNext_(0) {}
   };

   class Notifiee : public virtual NotifieeConst, public virtual Fwk::NamedInterface::Notifiee {
   public:
      typedef Fwk::Ptr<Notifiee const> PtrConst;
      typedef Fwk::Ptr<Notifiee> Ptr;
      Location::PtrConst notifier() const { return NotifieeConst::notifier(); }
      Location::Ptr notifier() { return const_cast<Location *>(NotifieeConst::notifier().ptr()); }
      // Non-const interface =============================================
      static Notifiee::Ptr NotifieeIs() {
         Ptr m = new Notifiee();
         m->referencesDec(1);
         // decr. refer count to compensate for initial val of 1
         return m;
      }
      // Constructors ====================================================
   protected:
      Notifiee(): Fwk::NamedInterface::Notifiee() {}
   };
	
	Segment segment(unsigned index) const;

protected:
	vector<Segment> segments_;

//subscribe to notifications from segments that are added to it.
};

class CustomerLocation : Location {};

class Terminal : Location {};

class Port : Location {};

class TruckTerminal : Terminal {};

class BoatTerminal : Terminal {};

class PlaneTerminal : Terminal {};

class Segment : public Fwk::NamedInterface
{

public:

	typedef Fwk::Ptr<Segment const> PtrConst;
  typedef Fwk::Ptr<Segment> Ptr;

	class NotifieeConst : public virtual Fwk::NamedInterface::NotifieeConst {
   public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      typedef Fwk::Ptr<NotifieeConst> Ptr;
      Fwk::String name() const { return (notifier_?notifier_->name():Fwk::String()); }
      Segment::PtrConst notifier() const { return notifier_; }
      bool isNonReferencing() const { return isNonReferencing_; }
      NotifieeConst const * lrNext() const { return lrNext_; }
      NotifieeConst * lrNext() { return lrNext_; }
      // Non-const interface =============================================
       ~NotifieeConst();
      virtual void notifierIs(const Segment::PtrConst& _notifier);
      void isNonReferencingIs(bool _isNonReferencing);
      virtual void onSegmentNew( Segment::Ptr ) {};
      virtual void onSegmentDel ( Segment::Ptr ) {}; 
      virtual void onExpedite ( Segment::Ptr ) {};
      void lrNextIs(NotifieeConst * _lrNext) {
         lrNext_ = _lrNext;
      }
      static NotifieeConst::Ptr NotifieeConstIs() {
         Ptr m = new NotifieeConst();
         m->referencesDec(1);
         // decr. refer count to compensate for initial val of 1
         return m;
      }
      // Constructors ====================================================
   protected:
      Segment::PtrConst notifier_;
      bool isNonReferencing_;
      NotifieeConst * lrNext_;
      NotifieeConst(): Fwk::NamedInterface::NotifieeConst(),
            isNonReferencing_(false),
            lrNext_(0) {}
   };

   class Notifiee : public virtual NotifieeConst, public virtual Fwk::NamedInterface::Notifiee {
   public:
      typedef Fwk::Ptr<Notifiee const> PtrConst;
      typedef Fwk::Ptr<Notifiee> Ptr;
      Segment::PtrConst notifier() const { return NotifieeConst::notifier(); }
      Segment::Ptr notifier() { return const_cast<Segment *>(NotifieeConst::notifier().ptr()); }
      // Non-const interface =============================================
      static Notifiee::Ptr NotifieeIs() {
         Ptr m = new Notifiee();
         m->referencesDec(1);
         // decr. refer count to compensate for initial val of 1
         return m;
      }
      // Constructors ====================================================
   protected:
      Notifiee(): Fwk::NamedInterface::Notifiee() {}
   };

	class PackageCapacity : Ordinal<Miles, unsigned int> {};

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

	Location source() const;
	void sourceIs(Location loc);

	Miles length() const;
	void lengthIs(Miles miles);

	Segment destination() const;
	void destinationIs(Segment seg);

	DifficultyLevel difficulty() const;
	void difficultyIs(DifficultyLevel dif);

	ExpediteOptions expedite() const;
	void expediteIs(ExpediteOptions exp_opt);

protected:
	Minutes time_;
	Dollars cost_;
	PackageCapacity capcity_;

	Miles length_;
	Location source_;
	Location destination_;
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

class Fleet : public Fwk::NamedInterface {

public:

	typedef Fwk::Ptr<Fleet const> PtrConst;
  typedef Fwk::Ptr<Fleet> Ptr;

	class NotifieeConst : public virtual Fwk::NamedInterface::NotifieeConst {
   public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      typedef Fwk::Ptr<NotifieeConst> Ptr;
      Fwk::String name() const { return (notifier_?notifier_->name():Fwk::String()); }
      Fleet::PtrConst notifier() const { return notifier_; }
      bool isNonReferencing() const { return isNonReferencing_; }
      NotifieeConst const * lrNext() const { return lrNext_; }
      NotifieeConst * lrNext() { return lrNext_; }
      // Non-const interface =============================================
       ~NotifieeConst();
      virtual void notifierIs(const Fleet::PtrConst& _notifier);
      void isNonReferencingIs(bool _isNonReferencing);
      virtual void onSpeed( Fleet::Ptr ) {};
      virtual void onCapacity( Fleet::Ptr ) {};
      virtual void onCostPerMile ( Fleet::Ptr ) {};
      void lrNextIs(NotifieeConst * _lrNext) {
         lrNext_ = _lrNext;
      }
      static NotifieeConst::Ptr NotifieeConstIs() {
         Ptr m = new NotifieeConst();
         m->referencesDec(1);
         // decr. refer count to compensate for initial val of 1
         return m;
      }
      // Constructors ====================================================
   protected:
      Fleet::PtrConst notifier_;
      bool isNonReferencing_;
      NotifieeConst * lrNext_;
      NotifieeConst(): Fwk::NamedInterface::NotifieeConst(),
            isNonReferencing_(false),
            lrNext_(0) {}
   };

   class Notifiee : public virtual NotifieeConst, public virtual Fwk::NamedInterface::Notifiee {
   public:
      typedef Fwk::Ptr<Notifiee const> PtrConst;
      typedef Fwk::Ptr<Notifiee> Ptr;
      Fleet::PtrConst notifier() const { return NotifieeConst::notifier(); }
      Fleet::Ptr notifier() { return const_cast<Fleet *>(NotifieeConst::notifier().ptr()); }
      // Non-const interface =============================================
      static Notifiee::Ptr NotifieeIs() {
         Ptr m = new Notifiee();
         m->referencesDec(1);
         // decr. refer count to compensate for initial val of 1
         return m;
      }
      // Constructors ====================================================
   protected:
      Notifiee(): Fwk::NamedInterface::Notifiee() {}
   };

	MilesPerHour speed() const;
	void speed(MilesPerHour _speed);

	Segment::PackageCapacity capcity() const;
	void capcityIs(Segment::PackageCapacity _capacity);

	Dollars costPerMile() const;
	void costPerMile(Dollars _costPerMile);

private:

	MilesPerHour speed_;
	Segment::PackageCapacity capacity_;
	Dollars costPerMile_;


};

class BoatFleet : Fleet {};

class PlaneFleet : Fleet {};

class TruckFleet : Fleet {};

} /* end namespace */



#endif
