#ifndef MEMPHYS_Analysis_h
#define MEMPHYS_Analysis_h

// Inheritance :
#include "IAppManager.hh"

#include <string>

#ifdef APP_USE_AIDA
namespace AIDA {
 class IAnalysisFactory;
 class ITree;
}
#endif

#ifdef APP_USE_INLIB_WROOT
#include <inlib/wroot/file>
#include <inlib/wroot/tree>
#endif

//JEC 10/1/06 introduction
namespace MEMPHYS {

class Analysis  : public virtual IAppManager {
  public: //IAppManager
    virtual void closeTree();         
    virtual bool initialize() {return true;}
  public:        
#ifdef APP_USE_AIDA
    Analysis(AIDA::IAnalysisFactory*,bool aBatch = true);
#else
    Analysis(bool aBatch = true);
#endif
    virtual ~Analysis();
  protected:
#ifdef APP_USE_INLIB_WROOT
    Analysis(const Analysis& a_from):m_file(a_from.m_file.out(),"") {}
#endif
    Analysis& operator=(const Analysis& a_from) {return *this;}
  public:
    //Get tree pointer
#ifdef APP_USE_AIDA
    AIDA::ITree* tree() const {return fTree;}
#endif  
  private:
    bool fBatch;
#ifdef APP_USE_AIDA
    AIDA::IAnalysisFactory* fAIDA;
    AIDA::ITree* fTree;
#endif
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
