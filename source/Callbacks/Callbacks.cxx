//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,const std::vector<std::string>&);
//   }
//

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>

#include <Slash/Tools/UI.h>
#include <Slash/Tools/Data.h>
#include <Slash/Tools/AIDA.h>

// Lib :
#include <Lib/Manager.h>
#include <Lib/Out.h>
#include <Lib/smanip.h>

// AIDA :
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>
#include <AIDA/ITree.h>

// amanip :
#include <amanip/create.h>
#include <amanip/find.h>


// G4Lab :
#include <G4Lab/Interfaces/IGeant4Manager.h>

// MEMPHYS :
#include <MEMPHYS/IAppManager.hh>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_Initialize(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
// Should be executed (in a widget create callback) 
// when the GUI is constructed 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aUI.setCallback("OnX_File_exit","activate","DLD","G4Lab G4Lab_exit");
  //StoreTrajectory customized in TrackingAction.
  //aUI.executeScript("G4","/tracking/storeTrajectory 1");

  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,"Viewer_1");
  if(soViewer) {
    soViewer->setDecoration(true);
    soViewer->setViewing(true);
  }

  Slash::Core::ISession& session = aUI.session();

  session.setParameter("event.scan","yes");//Used in MEMPHYS_eventEnd callback.
    
  IGeant4Manager* g4Manager = 
    Lib_findManager(session,"Geant4Manager",IGeant4Manager);
  if(!g4Manager) {
    Lib::Out out(session.printer());
    out << "MEMPHYS_Initialize :"
        << " Geant4Manager not found."
        << Lib::endl;
  } else {
    // Declare a callback executed at each end of event :
    g4Manager->setEventEndScript("DLD","MEMPHYS MEMPHYS_eventEnd");
  }  
    
  MEMPHYS::IAppManager* appManager = 
    Lib_findManager(session,"MEMPHYS::AppManager",MEMPHYS::IAppManager);
  if(!appManager) {
    Lib::Out out(session.printer());
    out << "MEMPHYS_Initialize :"
        << " MEMPHYS::AppManager not found."
        << Lib::endl;
  } else {
    // Create the Types. Initialize G4Lab SoNodes.
    appManager->initialize();
  }
}
//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_Finalize(
 Slash::UI::IUI&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_Geant4_detector(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession& session = aUI.session();
  
  //  Slash::Data::collect(session,"PV","name==\"expHall\"");
  //Slash::Data::visualize(session,Slash::s_current());

  //Slash::Data::collect(session,"PV","name==\"WCBox\"");
  //Slash::Data::visualize(session,Slash::s_current());
 //    session.setParameter("modeling.type","wire_frame");
//    session_setColor(session,"green");

   Slash::Data::collect(session,"PV","name==\"WC\"");
   //Slash::Data::collect(session,"PV","name==\"WCCyl\"");//AT
   //Slash::Data::collect(session,"PV","name==\"WCCube\"");//AT
  Slash::Data::visualize(session,Slash::s_current());

  Slash::Data::collect(session,"PV","name==\"WCFrontCap\"");
  Slash::Data::visualize(session,Slash::s_current());

  Slash::Data::collect(session,"PV","name==\"WCBackCap\"");
  Slash::Data::visualize(session,Slash::s_current());

  Slash::Data::collect(session,"PV","name==\"WCBarrel\"");
  Slash::Data::visualize(session,Slash::s_current());

  Slash::Data::collect(session,"PV","name==\"WCBackOD\"");
  Slash::Data::visualize(session,Slash::s_current());

  Slash::Data::collect(session,"PV","name==\"WCFrontOD\"");
  Slash::Data::visualize(session,Slash::s_current());

  Slash::Data::collect(session,"PV","name==\"WCFrontCap\"");
  Slash::Data::visualize(session,Slash::s_current());



  //session.setParameter("modeling.type","solid");

   // A.T. PMT visualization
  /*
   session.setParameter("modeling.type","wire_frame");//az
   session.setParameter("modeling.transparency","0.");
   session_setColor(session,"blue");
   Slash::Data::collect(session,"PV","name==\"WCCapPMTGlass\"");
   Slash::Data::visualize(session,Slash::s_current());

   session.setParameter("modeling.type","wire_frame");
   session_setColor(session,"cyan");
  */


 
//   Slash::Data::collect(session,"PV","name==\"WCBarrelAnnulus\""); //JEC FIXME PV->Replica
//   Slash::Data::visualize(session,Slash::s_current());
//   Slash::Data::collect(session,"PV","name==\"WCBarrelCell\""); //JEC FIXME PV->Replica
//   Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"WCCapPolygon\"");
//   Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"WCPMT\"");
//   Slash::Data::visualize(session,Slash::s_current());

//    Slash::Data::collect(session,"PV(PM)","name==\"WCCapPMTGlass\"");
//    Slash::Data::visualize(session,Slash::s_current());
//    Slash::Data::collect(session,"PV_PM","name==\"WCCapPMTGlass\"");
//    Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"WCBarrelCellBlackSheet\"");
//   Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"WCCapBlackSheet\"");
//   Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"WCCapBarrelBlackSheet\"");
//   Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"WCBarrelPMT\"");
//   Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"WCBarrelPMTGlass\"");
//   Slash::Data::visualize(session,Slash::s_current());


//   Slash::Data::collect(session,"PV","name==\"RockMother_phys\"");
//   Slash::Data::visualize(session,Slash::s_current());

//   Slash::Data::collect(session,"PV","name==\"RockCeiling_phys\"");
//   Slash::Data::visualize(session,Slash::s_current());


}
//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_analysis_closeTree(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession& session = aUI.session();
  
  MEMPHYS::IAppManager* appManager = 
    Lib_findManager(session,"MEMPHYS::AppManager",MEMPHYS::IAppManager);
  if(!appManager) {
    Lib::Out out(session.printer());
    out << "MEMPHYS_analysis_closeTree :"
        << " MEMPHYS::AnalysisManager not found."
        << Lib::endl;
    return;
  }

  appManager->closeTree();
}

//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_plot_hits_times(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession& session = aUI.session();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;

  AIDA::ITree* memoryTree = amanip::find_mem(*aida);
  if(!memoryTree) return;

  std::string hname = "h_hits_times";
  if(memoryTree->find(hname)) memoryTree->rm(hname);

  AIDA::IHistogram1D* h = 
    amanip::create_histogram_1D(*aida,*memoryTree,hname,"Hits times.",100,0,3000);
  if(!h) return;

  // Fill histogram with a Slash::Data::collect :

 
  Slash::Data::collect(session,"WCPMT","");
  Slash::Data::number(session);

  Slash::Data::fill_histogram_1D(session,hname,"Times");

  // Get current plotter :
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida,Slash::s_current());
  if(plotter) {
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.plot(*h);
    delete plotter;
  }
}
#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)
//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_vis_bins(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession& session = aUI.session();

  Slash::UI::IWidget* cw = aUI.currentWidget();
  Slash::UI::IWidget* widget = aUI.findWidget("Viewer_2");
  if(!widget) return;
  aUI.setCurrentWidget(widget);

  //FIXME : check that current region is a plotter.
  Slash::Data::collect(session,"SceneGraph","highlight==true");
  Slash::Data::filter(session,"name");

  std::vector<double> mins;
  Lib::smanip::todoubles(Slash::Data::values(session,"lowerEdge"),mins);
  std::vector<double> maxs;
  Lib::smanip::todoubles(Slash::Data::values(session,"upperEdge"),maxs);
  if(maxs.size()!=mins.size()) return;
  if(!mins.size()) {
    Lib::Out out(session.printer());
    out << "MEMPHYS_vis_bins :"
        << " no highlighted bins."
        << Lib::endl;
    return;
  }
  double mn = mins[0];
  double mx = maxs[0];
  for(unsigned int index=1;index<mins.size();index++) {
    mn = MINIMUM(mn,mins[index]);
    mx = MAXIMUM(mx,maxs[index]);
  }

  Lib::Out out(session.printer());
  out << "MEMPHYS_vis_bins :"
      << " min " << mn
      << " max " << mx
      << Lib::endl;

  aUI.setCurrentWidget(cw);

  std::string filter;
  Lib::smanip::printf(filter,128,"(!(Times<%g))&&(!(Times>=%g))",mn,mx);
  Slash::Data::collect(session,"WCPMT",filter);
  Slash::Data::visualize(session,Slash::s_current());
}


} // extern "C"
