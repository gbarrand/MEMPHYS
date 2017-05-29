#ifndef MEMPHYS_Analysis_h
#define MEMPHYS_Analysis_h

// Inheritance :
#include "IAppManager.hh"

#include <string>

#ifdef APP_USE_INLIB_WROOT
#include <inlib/wroot/file>
#include <inlib/wroot/tree>
#endif

//JEC 10/1/06 introduction
namespace MEMPHYS {

class Analysis  : public virtual IAppManager {
  public: //IAppManager
    virtual bool initialize() {return true;}
  public:        
    Analysis(const std::string& a_file);
    virtual ~Analysis();
  protected:
#ifdef APP_USE_INLIB_WROOT
    Analysis(const Analysis& a_from):m_file(a_from.m_file.out(),"") {}
#endif
    Analysis& operator=(const Analysis& a_from) {return *this;}
  private:
    std::string m_file_name; 
#ifdef APP_USE_INLIB_WROOT
  public:
    inlib::wroot::file m_file;
  public:
#include "Event_tree_h.icc"
#include "Geometry_tree_h.icc"
#endif
};

}
#endif
