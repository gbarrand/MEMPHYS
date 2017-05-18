// AIDA analysis program over MEMPHYS.root file.

// AIDA :
#include <AIDA/AIDA.h>
#include <AIDA/ITupleEntry.h>

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>

#include <iostream>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline AIDA::ITuple* cast_Tuple(AIDA::ITupleEntry* aEntry) {
  //return (AIDA::ITuple*)aEntry->cast("AIDA::ITuple");
  return dynamic_cast<AIDA::ITuple*>(aEntry);
}

inline bool get_XYZ(AIDA::ITuple& aParent,int aColumn,double& aX,double& aY,double& aZ) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(aColumn);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  if(!tuple->next()) return false;

  aX = tuple->getDouble(0);
  aY = tuple->getDouble(1);
  aZ = tuple->getDouble(2);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
inline bool dump_tracks(AIDA::ITuple& aParent) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(8);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  //if(nPart!=tracks->rows()) {
  //  std::cout << "read: nPart / tracks mismatch " 
  //              << nPart << " " << tracks->rows() << std::endl;
  //  return false;
  //}

  tuple->start();
  int irow = 0;
  while(tuple->next()) {

    int pId = tuple->getInt(0);
    int parent = tuple->getInt(1);
    float timeStart = tuple->getFloat(2);

    double dx,dy,dz;
    if(!get_XYZ(*tuple,3,dx,dy,dz)) return false;

    double mass = tuple->getDouble(4);
    double pTot = tuple->getDouble(5);
    double ETot = tuple->getDouble(6);

    double px,py,pz;
    if(!get_XYZ(*tuple,7,px,py,pz)) return false;

    double start_x,start_y,start_z;
    if(!get_XYZ(*tuple,8,start_x,start_y,start_z)) return false;

    double stop_x,stop_y,stop_z;
    if(!get_XYZ(*tuple,9,stop_x,stop_y,stop_z)) return false;

    int startVol = tuple->getInt(10);
    int stopVol = tuple->getInt(11);

    std::cout << "----> Tk{"<<irow<<"}: " 
              << " pId " << pId
              << " parent " << parent
              << " creation time " << timeStart 
              << " Volumes " << startVol << " " << stopVol << "\n"
              << " Start Pos (" << start_x 
              << "," << start_y << "," << start_z << ")\n"
              << " Stop Pos (" << stop_x 
              << "," << stop_y << "," << stop_z << ")\n"
              << " dx,dy,dz " << dx << " " << dy << " " << dz << "\n"
              << " m " << mass
              << " ETot " << ETot
              << " pTot " << pTot
              << " px,py,pz " << px << " " << py << " " << pz << "\n"
              << std::endl;

    irow++;
  }
  return true;
}

inline bool process_hits_times(AIDA::ITupleEntry* a_entry,inlib::histo::h1d& aHisto) {
  if(!a_entry) return false;
  AIDA::ITuple* tuple = cast_Tuple(a_entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    float time = tuple->getFloat(0);

    aHisto.fill(time);
  }
  return true;
}

inline bool process_hits(AIDA::ITupleEntry* a_entry,inlib::histo::h1d& aHisto) {
  if(!a_entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(a_entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    //int tubeId = tuple->getInt(0);
    //int totalPE = tuple->getInt(1);
    
    if(!process_hits_times((AIDA::ITupleEntry*)tuple->getObject(2),aHisto)) return false;
  }
  return true;
}

inline bool process_digits(AIDA::ITupleEntry* a_entry,inlib::histo::h2d& aHisto) {
  if(!a_entry) return false;
  AIDA::ITuple* tuple = cast_Tuple(a_entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    //int tubeId = tuple->getInt(0);
    double pe = tuple->getDouble(1);
    double time = tuple->getDouble(2);
    //printf("debug : ++++ : %g %g\n",time,pe);

    aHisto.fill(time,pe);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
inline bool read_data(AIDA::IAnalysisFactory& a_aida,inlib::histo::h1d& aHisto1D,inlib::histo::h2d& aHisto2D) {

  AIDA::ITreeFactory* treeFactory = a_aida.createTreeFactory();
  if(!treeFactory) {
    std::cout << "can't get a TreeFactory." << std::endl;
    return false;
  }

  AIDA::ITree* tree = treeFactory->create("MEMPHYS.root","root",true,false);
  if(!tree) {
    std::cout << "can't open data file." << std::endl;
    delete treeFactory;
    return false;
  }

  AIDA::IManagedObject* object = tree->find("Event");
  if(!object) {
    std::cout << "object Event not found in tree." << std::endl;
    delete tree;
    delete treeFactory;
    return false;
  }
  
  AIDA::ITuple* tuple = (AIDA::ITuple*)object->cast("AIDA::ITuple");
  //AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
  if(!tuple) {
    std::cout << "object not an AIDA::ITuple." << std::endl;
    delete tree;
    delete treeFactory;
    return false;
  }

  int coln = tuple->columns();
  for(int index=0;index<coln;index++) {
    std::cout << " icol = " << index
              << ", label = " << tuple->columnName(index) 
              << ", type = " << tuple->columnType(index) 
              << std::endl;
  }

  std::cout << "rows = " << tuple->rows() << std::endl;

  int nentries = tuple->rows();
  //nentries = 100000;
  //nentries = 40;
  std::cout << "traitements de " << nentries << " entrees" << std::endl;

  int irow = 0;
  tuple->start();
  while(tuple->next() && (irow<nentries)) {

    /*
    int eventId = tuple->getInt(0);
    //int inputEvtId = tuple->getInt(1);
    //int interMode = tuple->getInt(2);
    //int vtxVol = tuple->getInt(3);

    int nPart = tuple->getInt(5);
    //int leptonIndex = tuple->getInt(6);
    //int protonIndex = tuple->getInt(7);

    int nHits = tuple->getInt(9);
    int nDigits = tuple->getInt(11);
    double sumPE = tuple->getDouble(12);

    std::cout << ">>>>>>>>>>>>> Event{" << irow << "}: "
    	      << " evt Id " << eventId 
    //	      << " evt Input Id " << inputEvtId
    //	      << "\n interaction mode " << interMode
    //	      << " start in volume " << vtxVol << "\n"
    	      <<" #tracks: " << nPart
    	      <<" #hits: " << nHits
              <<" #digits: " << nDigits
              <<" sumPE " << sumPE
    	      << std::endl;
    */

    //if(!dump_tracks(*tuple)) break;

    if(!process_hits((AIDA::ITupleEntry*)tuple->getObject(10),aHisto1D)) break;    
    if(!process_digits((AIDA::ITupleEntry*)tuple->getObject(13),aHisto2D)) break;

    irow++;
  }

  delete tree;
  delete treeFactory;

  return true;
}

//////////////////////////////////////////////////////////////////////////////
/// plotting done with inlib/exlib : //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <exlib/Windows/plotter>
#define EXLIB_SCREEN_MGR Windows
#else
#include <exlib/X11/plotter>
#define EXLIB_SCREEN_MGR X11
#endif
inline void set_region_style(inlib::sg::plotter& a_plotter) {
  
  a_plotter.right_axis_visible = true;
  a_plotter.top_axis_visible = true;
  
  // ROOT is in NDC, then we take a plotter with width = height = 1

  // PAW :
  //float XSIZ = 20.0F;
  //float XLAB = 1.4F;
  //float YSIZ = 20.0F;
  //float YLAB = 1.4F;

  float XLAB = 1.4F/20.0F; //0.07 //x distance of y title to data frame.
  float YLAB = 0.8F/20.0F; //0.04 //y distance of x title to data frame.
  YLAB = 0.05F; //FIXME : cooking.

  // ROOT def margins 0.1. SoPlotter def 0.1.
  a_plotter.right_margin = 0.1f;
  a_plotter.top_margin = 0.1f;
  a_plotter.left_margin = 0.1f;
  a_plotter.bottom_margin = 0.1f;

  a_plotter.wall_style().visible = false;
  a_plotter.grid_style().visible = false;
  a_plotter.inner_frame_style().line_width = 1;
  
  std::string font = "times.ttf";
  bool smoothing = true;
  
  // To have a good matching with ROOT for text size :
  //double majic = 0.014/0.027;
  float majic = 0.6f;
  
  // X axis :
  a_plotter.x_axis().labels_style().scale = majic;
  a_plotter.x_axis().labels_style().font = font;
  a_plotter.x_axis().labels_style().smoothing = smoothing;
  a_plotter.x_axis().title_style().scale = majic;
  a_plotter.x_axis().title_style().font = font;
  a_plotter.x_axis().title_style().smoothing = smoothing;
  a_plotter.x_axis().mag_style().scale = majic;
  a_plotter.x_axis().mag_style().font = font;
  a_plotter.x_axis().mag_style().smoothing = smoothing;
  a_plotter.x_axis().line_style().width = 2;
  a_plotter.x_axis().ticks_style().width = 2;
  a_plotter.x_axis().label_height = 0.05f;
  a_plotter.x_axis().title_height = 0.06f;
  a_plotter.x_axis().title_to_axis = 1.1f*YLAB;
//a_plotter.x_axis().label_to_axis = 0.01f;
  // Set hplot tick modeling :
  a_plotter.x_axis().modeling = inlib::sg::tick_modeling_hplot();
  a_plotter.x_axis().divisions = 505;

  // Y axis :
  a_plotter.y_axis().labels_style().scale = majic;
  a_plotter.y_axis().labels_style().font = font;
  a_plotter.y_axis().labels_style().smoothing = smoothing;
  a_plotter.y_axis().title_style().font = font;
  a_plotter.y_axis().title_style().scale = majic;
  a_plotter.y_axis().title_style().smoothing = smoothing;
  a_plotter.y_axis().mag_style().scale = majic;
  a_plotter.y_axis().mag_style().font = font;
  a_plotter.y_axis().mag_style().smoothing = smoothing;
  a_plotter.y_axis().line_style().width = 2;
  a_plotter.y_axis().ticks_style().width = 2;
  a_plotter.y_axis().label_height = 0.05f;
  a_plotter.y_axis().title_height = 0.06f;
  a_plotter.y_axis().title_to_axis = XLAB;
  a_plotter.y_axis().label_to_axis = 0.01f;
  // Set hplot tick modeling :
  a_plotter.y_axis().modeling = inlib::sg::tick_modeling_hplot();
  a_plotter.y_axis().divisions = 505;
  
  // title :
  a_plotter.title_height = 0.04f;
  a_plotter.title_to_axis = 0.02f;
  a_plotter.title_style().font = font;
  a_plotter.title_style().smoothing = smoothing;
  a_plotter.title_style().color = inlib::colorf_black();

  // info box :
  //a_plotter.infos_style().font = inlib::sg::font_arialbd_ttf();
  a_plotter.infos_style().modeling = inlib::sg::infos_modeling_ROOT();
  a_plotter.infos_style().font = font;
  a_plotter.infos_style().front_face = inlib::sg::winding_cw;
  a_plotter.infos_width = 0.2f;
  a_plotter.infos_x_margin = 0.01f; //percent of plotter width.
  a_plotter.infos_y_margin = 0.01f; //percent of plotter height.
  /*
  // legend box :
  a_plotter.setParameter("legendRegionVisible","TRUE");
  a_plotter.setParameter("legendRegion.viewportRegion.backgroundColor","1 1 0");
  a_plotter.setParameter("legendRegionSize","0.15 0.16");
  a_plotter.setParameter("legendRegionOrigin","0.1 0.1");
  a_plotter.setParameter("legendRegion.horizontalMargin","2");
  a_plotter.setParameter("legendRegion.verticalMargin","2");
  */
  
  //gStyle->SetPadBottomMargin(0.15);
  //gStyle->SetPadLeftMargin(0.15);
  //gStyle->SetTitleSize(0.06,"XYZ"); //ROOT def 0.04. SoPlotter : 0.014
  //gStyle->SetLabelOffset(0.01,"Y"); //ROOT def 0.005. SoPlotter def 0.02
  //gStyle->SetTitleOffset(1.1,"Y");
  //gStyle->SetLabelSize(0.05,"XYZ"); //ROOT def 0.04. SoPlotter def 0.014.  
  
}
inline bool plot(inlib::histo::h1d& aHisto1D,inlib::histo::h2d& aHisto2D) {
  
  exlib::EXLIB_SCREEN_MGR::session smgr(std::cout);
  if(!smgr.is_valid()) return false;

  exlib::EXLIB_SCREEN_MGR::plotter plotter(smgr,1,2,0,0,800,600);
  if(!plotter.window()) return false;
  inlib::env_append_path("EXLIB_FONT_PATH",".");    
  plotter.plots().view_border = false;

  if(plotter.plots().set_current_plotter(0)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    set_region_style(sgp);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "Entries";
    inlib::sg::plottable* ptb = new inlib::sg::h1d2plot(aHisto1D);
    ptb->set_name(aHisto1D.title());
    sgp.add_plottable(ptb);
  }

  if(plotter.plots().set_current_plotter(1)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    set_region_style(sgp);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "PE";
    inlib::sg::plottable* ptb = new inlib::sg::h2d2plot(aHisto2D);
    ptb->set_name(aHisto2D.title());
    sgp.add_plottable(ptb);
  }

  plotter.show();

  smgr.steer();

  return true;
}

#ifdef APP_USE_ARCHIVE
#include <BatchLab/Core/Main.h>
extern "C" {
  void BatchLabRioInitialize(Slash::Core::ISession&);
}
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(int aArgc,char** aArgv) {

  ////////////////////////////////////////////////////////
  // Create histograms : /////////////////////////////////
  ////////////////////////////////////////////////////////
  inlib::histo::h1d hits_times("Hits times",100,0,3000);
  inlib::histo::h2d digits_time_pe("Digits PE time",100,0,3000,100,0,10);

  ////////////////////////////////////////////////////////
  /// Read data //////////////////////////////////////////
  ////////////////////////////////////////////////////////

#ifdef APP_USE_ARCHIVE
  BatchLab::Main* session = new BatchLab::Main(std::vector<std::string>());
  Slash::Core::ILibraryManager* libraryManager = Slash::libraryManager(*session);
  if(!libraryManager) {std::cout << "LibraryManager not found." << std::endl;return EXIT_FAILURE;}
  Slash::Core::ILibrary* library = libraryManager->addLibrary("Rio","main","");
  if(!library) {std::cout << "addLibrary() failed." << std::endl;return EXIT_FAILURE;}
  ::BatchLabRioInitialize(*session);
  AIDA::IAnalysisFactory* aida = static_cast<AIDA::IAnalysisFactory*>(session);
#else
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
#endif
  if(!aida) {
    std::cout << "AIDA not found." << std::endl;
    return EXIT_FAILURE;
  }

  if(!read_data(*aida,hits_times,digits_time_pe)) {
    std::cout << "can't read data file." << std::endl;
    delete aida;
    return EXIT_FAILURE;
  }

  delete aida;

  plot(hits_times,digits_time_pe);

  return EXIT_SUCCESS;
}
