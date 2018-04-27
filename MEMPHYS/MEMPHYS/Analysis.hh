#ifndef MEMPHYS_Analysis_h
#define MEMPHYS_Analysis_h

#include <string>

#ifdef APP_USE_INLIB_WROOT
#include <inlib/wroot/file>
#include <inlib/wroot/tree>
#endif

//JEC 10/1/06 introduction
namespace MEMPHYS {

class Analysis  {
  public:        
    Analysis(const std::string& a_file,bool a_use_file);
    virtual ~Analysis();
  protected:
#ifdef APP_USE_INLIB_WROOT
    Analysis(const Analysis&):m_use_file(false),m_file(0) {}
#endif
    Analysis& operator=(const Analysis&) {return *this;}
  public:
    bool use_file() const {return m_use_file;}
  private:
    std::string m_file_name;
    bool m_use_file;
#ifdef APP_USE_INLIB_WROOT
  public:
    inlib::wroot::file* m_file;
  public:
#include "Event_tree_h.icc"
#include "Geometry_tree_h.icc"
#endif
};

}
#endif
