//! \file

#ifndef Y_UnitTest_Run_Included
#define Y_UnitTest_Run_Included 1

#include <exception>
#include <iostream>
#include <iomanip>

#include "y/check/claim.hpp"

namespace Yttrium
{
    //! helper to display aligned sizeof()
    struct SizeOf
    {
        static size_t Width; //!< default width

        //! display 'sizeof(className) = classSize'
        static void Display(const char  *className,
                            const size_t classSize);
    };
}

//! helper to replace sizeof()
#define Y_SIZEOF(CLASS) Yttrium::SizeOf::Display(#CLASS,sizeof(CLASS))

//! start a new test
#define Y_UTEST(NAME)                                              \
/**/  int Y_Test_##NAME(int argc, char **argv) noexcept {          \
/**/  const char *program=argv[0]; (void) argc; (void)program; try


//! leave the test
#define Y_UDONE()                                                  \
/**/    catch(const Yttrium::Exception &e) {                       \
/**/    e.display(); return 1; }                                   \
/**/    catch(const std::exception  &e) {                          \
/**/      std::cerr << " *** std::exception" << std::endl;         \
/**/      std::cerr << " *** " << e.what()   << std::endl;         \
/**/      return 1; }                                              \
/**/    catch(...) {                                               \
/**/      std::cerr << " *** Unhandled Exception" << std::endl;    \
/**/      return 1; }                                              \
/**/    return 0;                                                  \
/**/  }


#endif


