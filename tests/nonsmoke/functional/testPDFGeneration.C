//! PDF generator for AST
#include "rose.h"
using namespace std;
int
main ( int argc, char** argv )
{
  vector<string> argvList(argv,argv+argc);
  // accept -rose:convertFullAST
  if (CommandlineProcessing::isOption(argvList, "-rose:","convertFullAST",true))
    CppToPdfTranslator::dumpFullAST = true; 
    
  if (CommandlineProcessing::isOption(argvList, "","--help",false))
  {
    printf("--------------Tool specific instructions for pdfGenerator----\n");
    printf("To dump AST from input files only :  pdfGenerator your_input_file.c\n");
    printf("To dump full AST, including headers: pdfGenerator -rose:convertFullAST your_input_file.c\n");
    printf("---------------end of tool specific instructions --------------\n");
  }
  CppToPdfTranslator c;
  return c.translate(argvList);
  //return c.translate(argc,argv);
}













