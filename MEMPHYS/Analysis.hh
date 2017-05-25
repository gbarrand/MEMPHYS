#ifndef MEMPHYS_Analysis_h
#define MEMPHYS_Analysis_h

// Inheritance :
#include "IAppManager.hh"

#include <string>

namespace AIDA {
 class IAnalysisFactory;
 class ITree;
}

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
    Analysis(AIDA::IAnalysisFactory*,bool aBatch = true);
    virtual ~Analysis();
  protected:
#ifdef APP_USE_INLIB_WROOT
    Analysis(const Analysis& a_from):m_file(a_from.m_file.out(),"") {}
#endif
    Analysis& operator=(const Analysis& a_from) {return *this;}
  public:
    //Get tree pointer
    AIDA::ITree* tree() const {return fTree;}
  
  private:
    bool fBatch;
    AIDA::IAnalysisFactory* fAIDA;
    AIDA::ITree* fTree;
#ifdef APP_USE_INLIB_WROOT
  public:
    inlib::wroot::file m_file;
    inlib::wroot::tree* m_geom_tree;
    inlib::wroot::leaf<double>* m_leaf_wcRadius;
    inlib::wroot::leaf<double>* m_leaf_wcLength;

      inlib::wroot::tree* m_wcOffset_tree;
      inlib::wroot::leaf<double>* m_wcOffset_leaf_x;
      inlib::wroot::leaf<double>* m_wcOffset_leaf_y;
      inlib::wroot::leaf<double>* m_wcOffset_leaf_z;  
    inlib::wroot::leaf_object* m_leaf_wcOffset;
  
    inlib::wroot::leaf<double>* m_leaf_pmtRadius;
    inlib::wroot::leaf<int>* m_leaf_nPMTs;

      inlib::wroot::tree* m_pmtInfos_tree;
      inlib::wroot::leaf<int>* m_pmtInfos_leaf_pmtId;
      inlib::wroot::leaf<int>* m_pmtInfos_leaf_pmtLocation;

        inlib::wroot::tree* m_pmtOrient_tree;
        inlib::wroot::leaf<double>* m_pmtOrient_leaf_dx;
        inlib::wroot::leaf<double>* m_pmtOrient_leaf_dy;
        inlib::wroot::leaf<double>* m_pmtOrient_leaf_dz;
      inlib::wroot::leaf_object* m_pmtInfos_leaf_Orient;

        inlib::wroot::tree* m_pmtPosition_tree;
        inlib::wroot::leaf<double>* m_pmtPosition_leaf_x;
        inlib::wroot::leaf<double>* m_pmtPosition_leaf_y;
        inlib::wroot::leaf<double>* m_pmtPosition_leaf_z;
      inlib::wroot::leaf_object* m_pmtInfos_leaf_Position;

    inlib::wroot::leaf_object* m_leaf_pmtInfos;
  
#endif  
};

}
#endif
