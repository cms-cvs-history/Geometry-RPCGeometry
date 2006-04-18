/** Derived from DTGeometryAnalyzer by Nicola Amapane
 *
 *  \author M. Maggi - INFN Bari
 */

#include <memory>
#include <FWCore/Framework/interface/Frameworkfwd.h>

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCRollService.h"
#include <Geometry/Records/interface/MuonGeometryRecord.h>
//#include <Geometry/Vector/interface/GlobalPoint.h>
//#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
//#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>

#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <set>

class RPCGeometryAnalyzer : public edm::EDAnalyzer {

 public: 
  RPCGeometryAnalyzer( const edm::ParameterSet& pset);

  ~RPCGeometryAnalyzer();

  virtual void analyze( const edm::Event&, const edm::EventSetup& );
 
  const std::string& myName() { return myName_;}

 private: 

  const int dashedLineWidth_;
  const std::string dashedLine_;
  const std::string myName_;
};

RPCGeometryAnalyzer::RPCGeometryAnalyzer( const edm::ParameterSet& iConfig )
  : dashedLineWidth_(104), dashedLine_( std::string(dashedLineWidth_, '-') ), 
    myName_( "RPCGeometryAnalyzer" ) {}


RPCGeometryAnalyzer::~RPCGeometryAnalyzer() {}

void
RPCGeometryAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::ESHandle<RPCGeometry> pDD;
  iSetup.get<MuonGeometryRecord>().get( pDD );     

  std::cout << myName() << ": Analyzer..." << std::endl;
  std::cout << "start " << dashedLine_ << std::endl;


  std::cout << " Geometry node for RPCGeom is  " << &(*pDD) << std::endl;   
  std::cout << " I have "<<pDD->dets().size() << " detectors" << std::endl;
  std::cout << " I have "<<pDD->detTypes().size() << " types" << "\n" << std::endl;

  std::cout << myName() << ": Begin iteration over geometry..." << std::endl;
  std::cout << "iter " << dashedLine_ << std::endl;

  std::cout << "\n  #     id(hex)      id(dec)                   "
    "  g(x=0)   g(y=0)   g(z=0)  g(z=-1)  g(z=+1)  Ns "
    "  phi(0)  phi(s1)  phi(sN)    dphi    dphi'      ds     off"
    "       uR       uL       lR       lL" << std::endl;

  int icount = 0;

   const double dPi = 3.14159265358;
   const double radToDeg = 180. / dPi; //@@ Where to get pi from?

   std::set<RPCDetId> sids;
   for(TrackingGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it != pDD->dets().end(); it++){


     // Do we really have a RPC Roll?

     if( dynamic_cast< RPCRoll* >( *it ) != 0 ){
       ++icount;

       RPCRoll* roll = dynamic_cast< RPCRoll*>( *it );

//       // What's its DetId?


       RPCDetId detId=roll->id();
       int id = detId(); // or detId.rawId()

       if (sids.find(detId) != sids.end())
	 std::cout<<"VERYBAD ";
       std::cout << "GeomDetUnit is of type " << detId.det() << " and raw id = " << id;

//       // There's going to be a lot of messing with field width (and precision) so
//       // save input values...
       int iw = std::cout.width(); // save current width
       //       int ip = std::cout.precision(); // save current precision

       std::cout << "Parameters of roll# " << 
	 std::setw( 4 ) << icount << std::endl;
       std::cout<<std::setw(12) << id << std::dec << std::setw(12) << id << std::dec 
		 << std::setw( iw ) << detId;


//       // What's its surface?
//       // The surface knows how to transform local <-> global

	 const BoundSurface& bSurface = roll->surface();

	 // Check global coordinates of centre of RPCRoll, and how
	 // local z direction relates to global z direction

	 LocalPoint  lCentre( 0., 0., 0. );
	 GlobalPoint gCentre = bSurface.toGlobal( lCentre );

	 LocalPoint  lCentre1( 0., 0., -1.);
	 GlobalPoint gCentre1 = bSurface.toGlobal( lCentre1 );

	 LocalPoint  lCentre2( 0., 0., 1.);
	 GlobalPoint gCentre2 = bSurface.toGlobal( lCentre2 );

//       //	std::cout << "local(0,0,-1) = global " << gCentre1 << std::endl;
//       //	std::cout << "local(0,0)    = global " << gCentre  << std::endl;
//       //	std::cout << "local(0,0,+1) = global " << gCentre2 << std::endl;

       double gx  =  gCentre.x();
       double gy  =  gCentre.y();
       double gz  =  gCentre.z();
       double gz1 =  gCentre1.z();
       double gz2 =  gCentre2.z();
       if ( fabs( gx )  < 1.e-06 ) gx = 0.;
       if ( fabs( gy )  < 1.e-06 ) gy = 0.;
       if ( fabs( gz )  < 1.e-06 ) gz = 0.;
       if ( fabs( gz1 ) < 1.e-06 ) gz1 = 0.;
       if ( fabs( gz2 ) < 1.e-06 ) gz2 = 0.;

       int now = 9;
       int nop = 5;
       std::cout << 
	 std::setw( now ) << std::setprecision( nop ) << gx <<  
	 std::setw( now ) << std::setprecision( nop ) << gy << 
	 std::setw( now ) << std::setprecision( nop ) << gz << 
	 std::setw( now ) << std::setprecision( nop ) << gz1 << 
	 std::setw( now ) << std::setprecision( nop ) << gz2<<std::endl;
	 
       // Global Phi of centre of RPCRoll

       double cphi = gCentre.phi();
       double cphiDeg = cphi * radToDeg;
       if( cphiDeg < 0. ) cphiDeg = cphiDeg + 360.;
       // Handle some occasional ugly precision problems around zero
       if ( fabs(cphiDeg) < 1.e-06 ) cphiDeg = 0.;
       //        int iphiDeg = static_cast<int>( cphiDeg );
       //	std::cout << "phi(0,0,0) = " << iphiDeg << " degrees" << std::endl;

       RPCRollService rollServ(roll);
       int nStrips = rollServ.nstrips();
       std::cout << "\nStrips =  "<<std::setw( 4 ) << nStrips<<"\n";
       for(int is=0;is<nStrips;is++){
	 std::cout <<"s="<<std::setw(3)<<is+1<<" pos="
		   <<rollServ.CentreOfStrip(is+1);
	 if ((is+1)%5==0){ 
	   float str=is;
	   std::cout <<"s="<<std::setw(6)<<str<<" pos="
		     <<rollServ.CentreOfStrip(str)<<" gpos="<<
	     rollServ.LocalToGlobalPoint(rollServ.CentreOfStrip(str));
	   std::cout <<std::endl;
	 }
       }
       std::cout <<std::endl;
       
//       double cstrip1  = layer->centerOfStrip(1).phi();
//       double cstripN  = layer->centerOfStrip(nStrips).phi();

//       double phiwid   = layer->geometry()->stripPhiPitch();
//       double stripwid = layer->geometry()->stripPitch();
//       double stripoff = layer->geometry()->stripOffset();
//       double phidif   = fabs(cstrip1 - cstripN);

//       // May have one strip at 180-epsilon and other at -180+epsilon
//       // If so the raw difference is 360-(phi extent of chamber)
//       // Want to reset that to (phi extent of chamber):
//       if ( phidif > dPi ) phidif = fabs(phidif - 2.*dPi);
//       double phiwid_check = phidif/double(nStrips-1);

//       // Clean up some stupid floating decimal aesthetics
//       cstrip1 = cstrip1 * radToDeg;
//       if ( cstrip1 < 0. ) cstrip1 = cstrip1 + 360.;
//       if ( fabs( cstrip1 ) < 1.e-06 ) cstrip1 = 0.;
//       cstripN = cstripN * radToDeg;
//       if ( cstripN < 0. ) cstripN = cstrip1 + 360.;
//       if ( fabs( cstripN ) < 1.e-06 ) cstripN = 0.;

//       if ( fabs( stripoff ) < 1.e-06 ) stripoff = 0.;

//       now = 9;
//       nop = 4;
//       std::cout 
// 	<< std::setw( now ) << std::setprecision( nop ) << cphiDeg
// 	<< std::setw( now ) << std::setprecision( nop ) << cstrip1
// 	<< std::setw( now ) << std::setprecision( nop ) << cstripN
// 	<< std::setw( now ) << std::setprecision( nop ) << phiwid 
// 	<< std::setw( now ) << std::setprecision( nop ) << phiwid_check 
// 	<< std::setw( now ) << std::setprecision( nop ) << stripwid
// 	<< std::setw( now ) << std::setprecision( nop ) << stripoff ;
//       //          << std::setw(8) << (layer->getOwner())->sector() ; //@@ No sector yet!

//       // Layer geometry:  layer corner phi's...

//       std::vector<float> parameters = layer->geometry()->parameters();
//       // these parameters are half-lengths, due to GEANT
//       float bottomEdge = parameters[0];
//       float topEdge    = parameters[1];
//       float thickness  = parameters[2];
//       float apothem    = parameters[3];

//       // first the face nearest the interaction
//       // get the other face by using positive thickness
//       LocalPoint upperRightLocal(topEdge, apothem, -thickness);
//       LocalPoint upperLeftLocal(-topEdge, apothem, -thickness);
//       LocalPoint lowerRightLocal(bottomEdge, -apothem, -thickness);
//       LocalPoint lowerLeftLocal(-bottomEdge, -apothem, -thickness);
 
//       GlobalPoint upperRightGlobal = bSurface.toGlobal(upperRightLocal);
//       GlobalPoint upperLeftGlobal  = bSurface.toGlobal(upperLeftLocal);
//       GlobalPoint lowerRightGlobal = bSurface.toGlobal(lowerRightLocal);
//       GlobalPoint lowerLeftGlobal  = bSurface.toGlobal(lowerLeftLocal);

//       float uRGp = radToDeg * upperRightGlobal.phi();
//       float uLGp = radToDeg * upperLeftGlobal.phi();
//       float lRGp = radToDeg * lowerRightGlobal.phi();
//       float lLGp = radToDeg * lowerLeftGlobal.phi();

        
//       now = 9;
//       std::cout 
// 	<< std::setw( now ) << uRGp
// 	<< std::setw( now ) << uLGp
// 	<< std::setw( now ) << lRGp
// 	<< std::setw( now ) << lLGp 
// 	<< std::endl;
        
//       // Reset the values we changed
//       std::cout << std::setprecision( ip ) << std::setw( iw );
  
       sids.insert(detId);
     }
    
   }

//   //   for (TrackingGeometry::DetTypeContainer::const_iterator it = pDD->detTypes().begin(); it != pDD->detTypes().end(); it ++){
//   //   }

   std::cout << dashedLine_ << " end" << std::endl;
}

//define this as a plug-in
#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(RPCGeometryAnalyzer)