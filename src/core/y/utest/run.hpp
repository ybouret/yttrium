//! \file

#ifndef Y_UnitTest_Run_Included
#define Y_UnitTest_Run_Included 1

#include <exception>
#include <iostream>
#include <iomanip>

#include "y/check/claim.hpp"

namespace Yttrium
{
    //! helper to display metrics...
    struct UnitTestDisplay
    {
        static size_t Width; //!< default width

        //! display 'sizeof(className) = classSize'
        static void SizeOf(const char  *className,
                           const size_t classSize);

        //! display 'field = value'
        static void AsU64(const char    *field,
                          const uint64_t value);
    };
}

//! helper to replace sizeof()
#define Y_SIZEOF(CLASS) Yttrium::UnitTestDisplay::SizeOf(#CLASS,sizeof(CLASS))

//! helper to display unsigned field
#define Y_USHOW(FIELD)  Yttrium::UnitTestDisplay::AsU64(#FIELD,static_cast<uint64_t>(FIELD))


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


