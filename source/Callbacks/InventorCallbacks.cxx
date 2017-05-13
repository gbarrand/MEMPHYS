//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

//
// HEPVis and Inventor related callbacks.
//

#include <Slash/Tools/UI.h>
#include <Slash/Tools/Data.h>
#include <Slash/Tools/Inventor.h>

#include <Lib/mmanip.h>

#include <Inventor/nodes/SoOrthographicCamera.h>

// HEPVis :
#include <HEPVis/SbMath.h>
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_eventEnd(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession& session = aUI.session();

  std::string event_scan;
  session.parameterValue("event.scan",event_scan);
  if(event_scan=="no") return;

  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;

  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;

  soRegion->clear("dynamicScene");

  session.setParameter("modeling.trajectories","pickable");
  //session.setParameter("modeling.trajectories","immediate_all");

  //JEC 16/1/06 if too much tarjectory: 
  // FATAL if data_collect commented don't forget to comment data_visualize    
  
  Slash::Data::collect(session,"Trajectory");
  Slash::Data::visualize(session,Slash::s_current());


  //Enter secondary interactive loop to
  // play with the event :
  aUI.echo("Play with the event...");
  aUI.steer();

}
//////////////////////////////////////////////////////////////////////////////
void MEMPHYS_Layout_default(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
// Used in scripts/OnX/Pages.onx 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;

  soPage->deleteRegions();
  soPage->titleVisible.setValue(FALSE);
  soPage->createRegions("SoDisplayRegion",1,1,0);

  // First region ; side view :
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  soRegion->color.setValue(SbColor(0,0,0));

  // 10 * m :
  float size = 70000;

  SoCamera* soCamera = soRegion->getCamera();
  if(soCamera->isOfType(SoOrthographicCamera::getClassTypeId()))
    ((SoOrthographicCamera*)soCamera)->height.setValue(size);
  soCamera->nearDistance.setValue(1);
  soCamera->focalDistance.setValue(size);
  soCamera->farDistance.setValue(100*size);
}

} // extern "C"
