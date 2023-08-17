//! \file

#ifndef Y_Program_Included
#define Y_Program_Included 1

#include "y/system/exception.hpp"
#include <iostream>

//! prolog to create executable
#define Y_Program() \
int main(int argc, const char *argv[]) {\
/**/ (void) argc;\
/**/ (void) argv;\
/**/ static const char * const program = argv[0];\
/**/ (void) program;\
/**/ try

//! epilog to create executable
#define Y_End() \
/**/ catch(const Yttrium::Exception &e) { e.display();                              return 1; } \
/**/ catch(const std::exception &e) { std::cerr << "*** " << e.what() << std::endl; return 2; } \
/**/ catch(...) { std::cerr << "Unhandled exception in " << program << std::endl;   return 3; } \
/**/ return 0; }


#endif

