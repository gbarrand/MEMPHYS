#ifndef MEMPHYS_Analysis_h
#define MEMPHYS_Analysis_h

// Inheritance :
#include <MEMPHYS/IAppManager.hh>

#include <string>

namespace AIDA {
 class IAnalysisFactory;
 class ITree;
}

//JEC 10/1/06 introduction
namespace MEMPHYS {

class Analysis  : public virtual IAppManager {
  public: //IAppManager
    virtual void closeTree();         
    virtual bool initialize() {return true;}
  public:        
    Analysis(AIDA::IAnalysisFactory*,
             const std::string& format = "",
             bool aBatch = true);
    virtual ~Analysis();

    //Get tree pointer
    AIDA::ITree* tree() const {return fTree;}

  private:
    AIDA::IAnalysisFactory* fAIDA;
    bool fBatch;
    AIDA::ITree* fTree;
};

}
#endif
