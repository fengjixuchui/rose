#include "sage3basic.h"
#include "ATermToUntypedTraversal.h"

#define PRINT_ATERM_TRAVERSAL 0

using namespace ATermSupport;
using std::cout;
using std::cerr;
using std::endl;

ATermToUntypedTraversal::ATermToUntypedTraversal(SgSourceFile* source)
{
   SgUntypedDeclarationStatementList* sg_decls = new SgUntypedDeclarationStatementList();
   SgUntypedStatementList*            sg_stmts = new SgUntypedStatementList();
   SgUntypedFunctionDeclarationList*  sg_funcs = new SgUntypedFunctionDeclarationList();
   SgUntypedGlobalScope*          global_scope = new SgUntypedGlobalScope("",sg_decls,sg_stmts,sg_funcs);

   pSourceFile = source;
   ROSE_ASSERT(pSourceFile != NULL);

   pUntypedFile = new SgUntypedFile(global_scope);
   ROSE_ASSERT(pUntypedFile != NULL);

// DQ (2/25/2013): Set the default for source position generation to be consistent with other languages (e.g. C/C++).
   SageBuilder::setSourcePositionClassificationMode(SageBuilder::e_sourcePositionFrontendConstruction);
}

ATermToUntypedTraversal::~ATermToUntypedTraversal()
{
   delete pUntypedFile;
}


void
ATermToUntypedTraversal::fixupLocation(PosInfo & loc)
{
   int end_col = loc.getEndCol();

   // make sure start column isn't the same as end col
   if (loc.getStartLine() == loc.getEndLine() && loc.getStartCol() == loc.getEndCol())
      {
         return;
      }

   // check that end column isn't first column in the line
   if (end_col > 1)
      {
         loc.setEndCol(end_col - 1);
      }
}

PosInfo
ATermToUntypedTraversal::getLocation(ATerm term)
{
   PosInfo pinfo;

   ATerm annotations = ATgetAnnotations(term);
   if (annotations) {
      int i1,i2,i3,i4;
      ATerm loc = ATgetFirst(annotations);
      if (ATmatch(loc, "Location(<int>,<int>,<int>,<int>)", &i1,&i2,&i3,&i4)) {
#if PRINT_ATERM_TRAVERSAL
         printf("... loc: %d %d %d %d\n", i1,i2,i3,i4);
#endif
         pinfo = PosInfo(i1,i2,i3,i4);
      }
   }
   fixupLocation(pinfo);

   return pinfo;
}

void
ATermToUntypedTraversal::setSourcePositionUnknown(SgLocatedNode* locatedNode)
{
     SageInterface::setSourcePosition(locatedNode);

#if 0
  // This function sets the source position to be marked as not available (since we don't have token information)
  // These nodes WILL be unparsed in the code generation phase.

  // The SgLocatedNode has both a startOfConstruct and endOfConstruct source position.
     ROSE_ASSERT(locatedNode != NULL);

  // Make sure we never try to reset the source position of the global scope (set elsewhere in ROSE).
     ROSE_ASSERT(isSgGlobal(locatedNode) == NULL);

     ROSE_ASSERT(locatedNode->get_endOfConstruct()   == NULL);
     ROSE_ASSERT(locatedNode->get_startOfConstruct() == NULL);
     SageInterface::setSourcePosition(locatedNode);
#endif
}

void
ATermToUntypedTraversal::setSourcePosition( SgLocatedNode* locatedNode, ATerm term )
{
   PosInfo pos = getLocation(term);
   return setSourcePosition(locatedNode, pos);
}

void
ATermToUntypedTraversal::setSourcePosition( SgLocatedNode* locatedNode, PosInfo & pos )
{
   ROSE_ASSERT(locatedNode != NULL);
   ROSE_ASSERT(locatedNode->get_startOfConstruct() == NULL);
   ROSE_ASSERT(locatedNode->get_endOfConstruct()   == NULL);

   std::string filename = getCurrentFilename();

#if PRINT_SOURCE_POSITION
   std::cout << "... setSourcePosition: " << pos.getStartLine() << " " <<  pos.getStartCol();
   std::cout <<                       " " << pos.getEndLine()   << " " <<  pos.getEndCol() << std::endl;
#endif

   locatedNode->set_startOfConstruct(new Sg_File_Info(filename, pos.getStartLine(), pos.getStartCol()));
   locatedNode->get_startOfConstruct()->set_parent(locatedNode);

   locatedNode->set_endOfConstruct(new Sg_File_Info(filename, pos.getEndLine(), pos.getEndCol()));
   locatedNode->get_endOfConstruct()->set_parent(locatedNode);

   SageInterface::setSourcePosition(locatedNode);
}

void
ATermToUntypedTraversal::setSourcePositionFrom( SgLocatedNode* locatedNode, SgLocatedNode* fromNode )
{
   PosInfo pos;

   pos.setStartLine (fromNode->get_startOfConstruct()-> get_line());
   pos.setStartCol  (fromNode->get_startOfConstruct()-> get_col() );
   pos.setEndLine   (fromNode->get_endOfConstruct()  -> get_line());
   pos.setEndCol    (fromNode->get_endOfConstruct()  -> get_col() );

   return setSourcePosition(locatedNode, pos);
}

void
ATermToUntypedTraversal::setSourcePositionExcludingTerm( SgLocatedNode* locatedNode, ATerm startTerm, ATerm endTerm )
{
   PosInfo pos = getLocation(startTerm);
   PosInfo end = getLocation(endTerm);

   pos.setEndLine(end.getStartLine());
   pos.setEndCol(end.getStartCol());

   return setSourcePosition(locatedNode, pos);
}

void
ATermToUntypedTraversal::setSourcePositionIncludingTerm( SgLocatedNode* locatedNode, ATerm startTerm, ATerm endTerm )
{
   PosInfo pos = getLocation(startTerm);
   PosInfo end = getLocation(endTerm);

   pos.setEndLine(end.getEndLine());
   pos.setEndCol(end.getEndCol());

   return setSourcePosition(locatedNode, pos);
}

void
ATermToUntypedTraversal::setSourcePositionIncludingNode( SgLocatedNode* locatedNode, ATerm startTerm, SgLocatedNode* endNode )
{
   PosInfo pos = getLocation(startTerm);

   pos.setEndLine(endNode->get_endOfConstruct()->get_line());
   pos.setEndCol (endNode->get_endOfConstruct()->get_col() );

   return setSourcePosition(locatedNode, pos);
}

void
ATermToUntypedTraversal::setSourcePositionFromEndOnly( SgLocatedNode* locatedNode, SgLocatedNode* fromNode )
{
   PosInfo pos;

   pos.setStartLine (fromNode->get_endOfConstruct()-> get_line());
   pos.setStartCol  (fromNode->get_endOfConstruct()-> get_col() );
   pos.setEndLine   (fromNode->get_endOfConstruct()-> get_line());
   pos.setEndCol    (fromNode->get_endOfConstruct()-> get_col() );

   return setSourcePosition(locatedNode, pos);
}
