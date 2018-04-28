#ifndef glutess_rename_h
#define glutess_rename_h

/* G.Barrand : namespace protection : */
#define gluTessBeginContour osc_gluTessBeginContour
#define gluTessBeginPolygon osc_gluTessBeginPolygon
#define gluTessCallback     osc_gluTessCallback
#define gluTessEndContour   osc_gluTessEndContour
#define gluTessEndPolygon   osc_gluTessEndPolygon
#define gluTessNormal       osc_gluTessNormal
#define gluTessProperty     osc_gluTessProperty
#define gluTessVertex       osc_gluTessVertex

/* internal */
#define gluBeginPolygon     osc_gluBeginPolygon
#define gluDeleteTess       osc_gluDeleteTess
#define gluEndPolygon       osc_gluEndPolygon
#define gluGetTessProperty  osc_gluGetTessProperty
#define gluNewTess          osc_gluNewTess
#define gluNextContour      osc_gluNextContour

#define __gl_computeInterior      osc__gl_computeInterior
/* done in dict-list.h :
#define __gl_dictListDelete       osc__gl_dictListDelete
#define __gl_dictListDeleteDict   osc__gl_dictListDeleteDict
#define __gl_dictListInsertBefore osc__gl_dictListInsertBefore
#define __gl_dictListNewDict      osc__gl_dictListNewDict
#define __gl_dictListSearch       osc__gl_dictListSearch
*/
#define __gl_edgeEval      osc__gl_edgeEval
#define __gl_edgeIntersect osc__gl_edgeIntersect
#define __gl_edgeSign      osc__gl_edgeSign
/* done in memalloc.h :
#define __gl_memInit       osc__gl_memInit
*/
#define __gl_meshAddEdgeVertex        osc__gl_meshAddEdgeVertex
#define __gl_meshCheckMesh            osc__gl_meshCheckMesh
#define __gl_meshConnect              osc__gl_meshConnect
#define __gl_meshDelete               osc__gl_meshDelete
#define __gl_meshDeleteMesh           osc__gl_meshDeleteMesh
#define __gl_meshDiscardExterior      osc__gl_meshDiscardExterior
#define __gl_meshMakeEdge             osc__gl_meshMakeEdge
#define __gl_meshNewMesh              osc__gl_meshNewMesh
#define __gl_meshSetWindingNumber     osc__gl_meshSetWindingNumber
#define __gl_meshSplice               osc__gl_meshSplice
#define __gl_meshSplitEdge            osc__gl_meshSplitEdge
#define __gl_meshTessellateInterior   osc__gl_meshTessellateInterior
#define __gl_meshTessellateMonoRegion osc__gl_meshTessellateMonoRegion
#define __gl_meshUnion                osc__gl_meshUnion
#define __gl_meshZapFace              osc__gl_meshZapFace
#define __gl_noBeginData              osc__gl_noBeginData
#define __gl_noCombineData            osc__gl_noCombineData
#define __gl_noEdgeFlagData           osc__gl_noEdgeFlagData
#define __gl_noEndData                osc__gl_noEndData
#define __gl_noErrorData              osc__gl_noErrorData
#define __gl_noVertexData             osc__gl_noVertexData
#define __gl_pqHeapDelete             osc__gl_pqHeapDelete
#define __gl_pqHeapDeletePriorityQ    osc__gl_pqHeapDeletePriorityQ
#define __gl_pqHeapExtractMin         osc__gl_pqHeapExtractMin
#define __gl_pqHeapInit               osc__gl_pqHeapInit
#define __gl_pqHeapInsert             osc__gl_pqHeapInsert
#define __gl_pqHeapNewPriorityQ       osc__gl_pqHeapNewPriorityQ
#define __gl_pqSortDelete             osc__gl_pqSortDelete
#define __gl_pqSortDeletePriorityQ    osc__gl_pqSortDeletePriorityQ
#define __gl_pqSortExtractMin         osc__gl_pqSortExtractMin
#define __gl_pqSortInit               osc__gl_pqSortInit
#define __gl_pqSortInsert             osc__gl_pqSortInsert
#define __gl_pqSortIsEmpty            osc__gl_pqSortIsEmpty
#define __gl_pqSortMinimum            osc__gl_pqSortMinimum
#define __gl_pqSortNewPriorityQ       osc__gl_pqSortNewPriorityQ
#define __gl_projectPolygon           osc__gl_projectPolygon
#define __gl_renderBoundary           osc__gl_renderBoundary
#define __gl_renderCache              osc__gl_renderCache
#define __gl_renderMesh               osc__gl_renderMesh
#define __gl_transEval                osc__gl_transEval
#define __gl_transSign                osc__gl_transSign
#define __gl_vertCCW                  osc__gl_vertCCW
#define __gl_vertLeq                  osc__gl_vertLeq
   
#endif
