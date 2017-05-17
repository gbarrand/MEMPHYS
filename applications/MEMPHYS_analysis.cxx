// AIDA analysis program over MEMPHYS.root file.

// AIDA :
#include <AIDA/AIDA.h>
#include <AIDA/ITupleEntry.h>

#include <sstream>
namespace Lib {
namespace smanip {
inline std::string tostring(double a_value) {
  std::ostringstream strm;
  strm << a_value;
  return strm.str();
}
}}

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

inline bool process_hits_times(AIDA::ITuple& aParent,AIDA::IHistogram1D& aHisto) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(2);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    float time = tuple->getFloat(0);

    aHisto.fill(time);
  }
  return true;
}

inline bool process_hits(AIDA::ITuple& aParent,AIDA::IHistogram1D& aHisto) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(10);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    //int tubeId = tuple->getInt(0);
    //int totalPE = tuple->getInt(1);

    if(!process_hits_times(*tuple,aHisto)) return false;
  }
  return true;
}

inline bool process_digits(AIDA::ITuple& aParent,AIDA::IHistogram2D& aHisto) {
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(13);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
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

#ifdef APP_USE_ARCHIVE //plotting done with inlib/exlib.
#ifdef _WIN32
#include <exlib/Windows/plotter>
#define EXLIB_SCREEN_MGR Windows
#else
#include <exlib/X11/plotter>
#define EXLIB_SCREEN_MGR X11
#endif
#include <exlib/AIDA/h2plot>
inline bool plot(AIDA::IAnalysisFactory&,AIDA::IHistogram1D& aHisto1D,AIDA::IHistogram2D& aHisto2D) {
  
  exlib::EXLIB_SCREEN_MGR::session smgr(std::cout);
  if(!smgr.is_valid()) return false;

  exlib::EXLIB_SCREEN_MGR::plotter plotter(smgr,1,2,0,0,700,500);
  if(!plotter.window()) return false;
  inlib::env_append_path("EXLIB_FONT_PATH",".");    
  plotter.plots().view_border = false;

  if(plotter.plots().set_current_plotter(0)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    sgp.bins_style(0).color = inlib::colorf_blue();
    sgp.infos_style().font = inlib::sg::font_arialbd_ttf();
    sgp.infos_style().front_face = inlib::sg::winding_cw;
    sgp.infos_x_margin = 0.01f; //percent of plotter width.
    sgp.infos_y_margin = 0.01f; //percent of plotter height.
    //set_region_style(region);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "Entries";
    inlib::sg::plottable* ptb = new exlib::AIDA::h1d2plot(aHisto1D);
    sgp.add_plottable(ptb);
  }

  if(plotter.plots().set_current_plotter(1)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    sgp.bins_style(0).color = inlib::colorf_blue();
    sgp.infos_style().font = inlib::sg::font_arialbd_ttf();
    sgp.infos_style().front_face = inlib::sg::winding_cw;
    sgp.infos_x_margin = 0.01f; //percent of plotter width.
    sgp.infos_y_margin = 0.01f; //percent of plotter height.
    //set_region_style(region);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "PE";
    inlib::sg::plottable* ptb = new exlib::AIDA::h2d2plot(aHisto2D);
    sgp.add_plottable(ptb);
  }

  plotter.show();

  smgr.steer();

  return true;
}
#else
inline void set_region_style(AIDA::IPlotterRegion& aRegion) {
  // Taken from G4SPL_analysis.cxx.
  //FIXME : have to do the below with AIDA styles.

  // ROOT is in NDC, then we take a plotter with width = height = 1

  // PAW :
  //float XSIZ = 20.0F;
  //float XLAB = 1.4F;
  //float YSIZ = 20.0F;
  //float YLAB = 1.4F;

  float XLAB = 1.4F/20.0F; //0.07 //x distance of y title to data frame.
  float YLAB = 0.8F/20.0F; //0.04 //y distance of x title to data frame.
  YLAB = 0.05F; //FIXME : cooking.

  // To have a good matching with ROOT for text size :
  //double majic = 0.014/0.027;
  double majic = 0.6;
  std::string s = Lib::smanip::tostring(majic);
  //FIXME : have methods for the below :
  //aRegion.style().setParameter("textScale",s);
  aRegion.style().xAxisStyle().setParameter("magStyle.scale",s);
  aRegion.style().yAxisStyle().setParameter("magStyle.scale",s);
  aRegion.style().xAxisStyle().tickLabelStyle().setParameter("scale",s);
  aRegion.style().xAxisStyle().labelStyle().setParameter("scale",s);
  aRegion.style().yAxisStyle().tickLabelStyle().setParameter("scale",s);
  aRegion.style().yAxisStyle().labelStyle().setParameter("scale",s);

  // ROOT def margins 0.1. SoPlotter def 0.1.
  aRegion.layout().setParameter("rightMargin",0.1);
  aRegion.layout().setParameter("topMargin",0.1);
  //gStyle->SetPadBottomMargin(0.15);
  aRegion.layout().setParameter("leftMargin",0.15);
  //gStyle->SetPadLeftMargin(0.15);
  aRegion.layout().setParameter("bottomMargin",0.15);

  aRegion.style().setParameter("superposeBins","TRUE");
  aRegion.setParameter("plotter.wallStyle.visible","FALSE");
  aRegion.setParameter("plotter.gridStyle.visible","FALSE");

  //std::string font = "Hershey"; //PAW font.
  //std::string font = "TTF/arialbd"; //Default ROOT font 62.
  //std::string font = "TTF/couri";
  std::string font = "TTF/times"; //= ROOT 132 ?
  //std::string smoothing = "FALSE";
  std::string smoothing = "TRUE";

  // X axis :
  aRegion.style().xAxisStyle().tickLabelStyle().setFont(font);
  aRegion.style().xAxisStyle().tickLabelStyle().setParameter("smoothing",smoothing);
  aRegion.style().xAxisStyle().labelStyle().setFont(font);
  aRegion.style().xAxisStyle().labelStyle().setParameter("smoothing",smoothing);
  aRegion.setParameter("plotter.xAxis.magStyle.fontName",font);
  aRegion.setParameter("plotter.xAxis.magStyle.smoothing",smoothing);
  aRegion.style().xAxisStyle().lineStyle().setThickness(2);
  aRegion.setParameter("plotter.xAxis.ticksStyle.width","2");
  aRegion.style().setParameter("topAxisVisible","TRUE");
  // Set hplot tick modeling :
  aRegion.style().xAxisStyle().setParameter("modeling","hplot");
  aRegion.style().xAxisStyle().setParameter("divisions","505");

  // Y axis :
  //aRegion.style().setParameter("yAxisLogScale","TRUE");
  aRegion.style().yAxisStyle().tickLabelStyle().setFont(font);
  aRegion.style().yAxisStyle().tickLabelStyle().setParameter("smoothing",smoothing);
  aRegion.style().yAxisStyle().labelStyle().setFont(font);
  aRegion.style().yAxisStyle().labelStyle().setParameter("smoothing",smoothing);
  aRegion.setParameter("plotter.yAxis.magStyle.fontName",font);
  aRegion.setParameter("plotter.yAxis.magStyle.smoothing",smoothing);
  aRegion.style().yAxisStyle().lineStyle().setThickness(2);
  aRegion.setParameter("plotter.yAxis.ticksStyle.width","2");

  //gStyle->SetTitleSize(0.06,"XYZ"); //ROOT def 0.04. SoPlotter : 0.014
  aRegion.setParameter("plotter.xAxis.titleHeight","0.06");
  aRegion.setParameter("plotter.yAxis.titleHeight","0.06");
  //gStyle->SetLabelOffset(0.01,"Y"); //ROOT def 0.005. SoPlotter def 0.02
  aRegion.setParameter("plotter.yAxis.labelToAxis","0.01");
  //gStyle->SetTitleOffset(1.1,"Y");
  aRegion.setParameter("plotter.yAxis.titleToAxis",Lib::smanip::tostring(1.1*XLAB));
  aRegion.setParameter("plotter.xAxis.titleToAxis",Lib::smanip::tostring(1.1*YLAB));
  //gStyle->SetLabelSize(0.05,"XYZ"); //ROOT def 0.04. SoPlotter def 0.014.
  aRegion.setParameter("plotter.yAxis.labelHeight","0.05");
  aRegion.setParameter("plotter.xAxis.labelHeight","0.05");

  aRegion.style().setParameter("rightAxisVisible","TRUE");
  // Set hplot tick modeling :
  aRegion.style().yAxisStyle().setParameter("modeling","hplot");
  aRegion.style().yAxisStyle().setParameter("divisions","505");

  // title :
  aRegion.style().setParameter("titleHeight","0.04");
  aRegion.style().setParameter("titleToAxis","0.02");
  aRegion.style().titleStyle().textStyle().setFont(font);
  aRegion.style().titleStyle().textStyle().setParameter("smoothing",smoothing);
  aRegion.style().titleStyle().textStyle().setColor("0 0 1");

  // legend box :
  aRegion.setParameter("legendRegionVisible","TRUE");
  aRegion.setParameter("legendRegion.viewportRegion.backgroundColor","1 1 0");
  aRegion.setParameter("legendRegionSize","0.15 0.16");
  aRegion.setParameter("legendRegionOrigin","0.1 0.1");
  aRegion.setParameter("legendRegion.horizontalMargin","2");
  aRegion.setParameter("legendRegion.verticalMargin","2");

  // Frame :
  aRegion.setParameter("plotter.innerFrameStyle.lineWidth","2");
}
inline bool plot(AIDA::IAnalysisFactory& aAIDA,AIDA::IHistogram1D& aHisto1D,AIDA::IHistogram2D& aHisto2D) {

  AIDA::IPlotterFactory* plotterFactory = 
    aAIDA.createPlotterFactory(0,0);
  if(!plotterFactory) {
    std::cout << "can't get a PlotterFactory." << std::endl;
    return false;
  }
    
  AIDA::IPlotter* plotter = plotterFactory->create();
  if(!plotter) {
    std::cout << "can't get a plotter." << std::endl;
    return false;
  }
    
  plotter->clearRegions();
  plotter->createRegions(1,2,0);

 {plotter->setCurrentRegionNumber(0);
  AIDA::IPlotterRegion& region = plotter->currentRegion();
  set_region_style(region);
  region.style().xAxisStyle().setLabel("time");
  region.style().yAxisStyle().setLabel("Entries");
  region.plot(aHisto1D);}

 {plotter->setCurrentRegionNumber(1);
  AIDA::IPlotterRegion& region = plotter->currentRegion();
  set_region_style(region);
  region.style().xAxisStyle().setLabel("time");
  region.style().yAxisStyle().setLabel("PE");
  region.plot(aHisto2D);}

  plotter->show();

  plotter->interact();

  delete plotter;
  delete plotterFactory;

  return true;
}
#endif

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

  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) {
    std::cout << "can't get a TreeFactory." << std::endl;
    return EXIT_FAILURE;
  }

  ////////////////////////////////////////////////////////
  // Create histograms in memory tree ////////////////////
  ////////////////////////////////////////////////////////
  AIDA::ITree* memory = treeFactory->create();
  if(!memory) {
    std::cout << "can't get memory tree." << std::endl;
    return EXIT_FAILURE;
  }
  AIDA::IHistogramFactory* histogramFactory = aida->createHistogramFactory(*memory);
  if(!histogramFactory) {
    std::cout << "can't get an histogram factory." << std::endl;
    return EXIT_FAILURE;
  }
  
  AIDA::IHistogram1D* hits_times = histogramFactory->createHistogram1D("hits_times","Hits times",100,0,3000);
  if(!hits_times) {
    std::cout << "can't create histogram : time." << std::endl;
    return EXIT_FAILURE;
  }

  AIDA::IHistogram2D* digits_time_pe = 
    histogramFactory->createHistogram2D("digits_pe_time","Digits PE time",
					100,0,3000,100,0,10);
  if(!digits_time_pe) {
    std::cout << "can't create histogram : digits_time_pe." << std::endl;
    return EXIT_FAILURE;
  }

  delete histogramFactory;

  ////////////////////////////////////////////////////////
  /// Read data //////////////////////////////////////////
  ////////////////////////////////////////////////////////

  std::string fmt = (aArgc>=2?aArgv[1]:"root"); //file extension

  AIDA::ITree* tree = treeFactory->create("MEMPHYS."+fmt,fmt,true,false);
  if(!tree) {
    std::cout << "can't open data file." << std::endl;
    return EXIT_FAILURE;
  }

  AIDA::IManagedObject* object = tree->find("Event");
  if(!object) {
    std::cout << "object Event not found in tree." << std::endl;
    return EXIT_FAILURE;
  }
  //AIDA::ITuple* tuple = (AIDA::ITuple*)object->cast("AIDA::ITuple");
  AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
  if(!tuple) {
    std::cout << "object not an AIDA::ITuple." << std::endl;
    return EXIT_FAILURE;
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

    //if(!dump_tracks(*tuple)) return EXIT_FAILURE;

    if(!process_hits(*tuple,*hits_times)) return EXIT_FAILURE;
    if(!process_digits(*tuple,*digits_time_pe)) return EXIT_FAILURE;

    irow++;
  }

  delete tree;
  delete treeFactory;

  plot(*aida,*hits_times,*digits_time_pe);

  delete aida;

  return EXIT_SUCCESS;
}
