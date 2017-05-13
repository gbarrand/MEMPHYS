
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITupleEntry.h>
#include <AIDA/ITuple.h>

//FIXME : CINT can't dynamic_cast, then we use the below function
// if working with AIDA-3.2.1 to cast a ITupleEntry to ITuple.

AIDA::ITuple* cast_ITuple(AIDA::ITupleEntry* aEntry) {
  return dynamic_cast<AIDA::ITuple*>(aEntry);
}
