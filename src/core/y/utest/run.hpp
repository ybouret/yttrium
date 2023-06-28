//! \file

#ifndef Y_UnitTest_Run_Included
#define Y_UnitTest_Run_Included 1

#include <exception>
#include <iostream>
#include "y/config/noexcept.hpp"
#include "y/config/starting.hpp"

//! start a new test
#define Y_UTEST(NAME)                                              \
/**/  int Y_Test_##NAME(int argc, char **argv) noexcept {          \
/**/  const char *program=argv[0]; (void) argc; (void)program; try


//! leave the test
#define Y_UDONE()                                                  \
/**/    catch(const std::exception  &e) {                          \
/**/      std::cerr << " *** std::exception" << std::endl;         \
/**/      std::cerr << " *** " << e.what()   << std::endl; }       \
/**/    catch(...) {                                               \
/**/      std::cerr << " *** Unhandled Exception" << std::endl; }  \
/**/    return 0;                                                  \
/**/  }


#endif


