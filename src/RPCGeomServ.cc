#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include <sstream>
#include <iomanip>

RPCGeomServ::RPCGeomServ::RPCGeomServ(const RPCDetId& id) : 
  _id(&id), _n(""), _t(-99), _z(true), _a(true)
{}


RPCGeomServ::~RPCGeomServ()
{}
  

std::string 
RPCGeomServ::name()
{
  if (_n.size()<1){
    std::string buf;


    if (_id->region()==0){
      buf="W";
      {
	std::stringstream os;
	os << std::setw(2)<<std::setfill('+')<<_id->ring()
	   <<std::setfill(' ')<<"/";
	buf += os.str();
      }
      
      {
	std::stringstream os;
	os <<"RB"<<_id->station();
	if (_id->station()<=2)
	  if (_id->layer()==1)
	    os<<"in";
	  else
	    os<<"out";
	os<<"/";
	buf += os.str();
      }
      
      
      {
	std::stringstream os;
	os <<"S"<<std::setw(2)<<std::setfill('0')
	   <<_id->sector()<<std::setfill(' ');
	if (_id->station()>2){
	  
	  if (_id->sector()== 4 && _id->station()==4){
	    if ( _id->subsector()==1){
	      os<<"--";
	    }
	    else if ( _id->subsector()==2){
	      os <<"-+";
	    }
	    else if ( _id->subsector()==3){
	      os <<"+-";
	    }
	    else if ( _id->subsector()==4){
	      os <<"++";
	    }
	  }
	  else {
	    if (_id->subsector()==1)
	      os <<"-";
	    else
	      os <<"+";
	  }
	}
	os<<"_";
	buf += os.str();
      }
      {
	std::stringstream os;
	if (_id->roll()==1)
	  os<<"Backward";
	else if (_id->roll() == 3)
	  os<<"Forward";
	else if (_id->roll() == 2)
	os <<"Middle";
	buf += os.str();
      }
    }
    else {
      buf="D";
      
      {
	std::stringstream os;
	os << std::setw(2)<<std::setfill('+')<<_id->station()*_id->region()
	   <<std::setfill(' ')<<"/";
	buf += os.str();    
      }
      
      {
	std::stringstream os;
	os <<"RE"<<_id->station()<<"/"<<_id->ring();
	buf += os.str();
      }
    }

    _n=buf;
  }
  return _n;
}

int 
RPCGeomServ::eta_partition()
{
  if (_t<-90){
    if (_id->region() == 0 ){
      if (this->inverted()) {
	_t = 3*(_id->ring())+ (3-_id->roll())-1;
      }else{
	_t = 3*(_id->ring())+ _id->roll()-2;
      }
    }else{
      _t = _id->region() * (3*(3-_id->ring()) + _id->roll() + 7);
    }
  }
  return _t;
} 


bool
RPCGeomServ::inverted()
{
  // return !(this->zpositive() && this->aclockwise());
  return !(this->zpositive());
}


bool
RPCGeomServ::zpositive()
{
  if (_id->region()==0 && _t<-90 ){
    if (_id->ring()<0){
      _z=false;
    }
    if (_id->ring()==0){
      if (_id->sector() == 1 || _id->sector() == 4 ||
	  _id->sector() == 5 || _id->sector() == 8 ||
	  _id->sector() == 9 || _id->sector() == 12){
	_z=false;
      }
    } 
  }
 
  return _z;
}

bool
RPCGeomServ::aclockwise()
{
  if (_id->region()==0 && _t<-90 ){
    if (_id->ring() > 0){
      if (_id->layer()==2){
	_a=false;
      }
    }else if(_id->ring() <0){
      if (_id->layer()==1){
	_a=false;
      }
    }else if(_id->ring() ==0) {
      if ((_id->sector() == 1 || _id->sector() == 4 ||
	   _id->sector() == 5 || _id->sector() == 8 ||
	   _id->sector() == 9 || _id->sector() == 12) && _id->layer()==1)
	_a=false;
      else if ((_id->sector() == 2 || _id->sector() == 3 ||
		_id->sector() == 6 || _id->sector() == 7 ||
		_id->sector() == 10|| _id->sector() == 11) && _id->layer()==2)
	_a=false;
    }
  }
  return _a;
}





RPCGeomServ::RPCGeomServ() : _id(0), _n(""), _t (-99), _z(false), _a(false)
{} 

