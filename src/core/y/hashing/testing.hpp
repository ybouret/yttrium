
//! \file
#ifndef Y_HASHING_TESTING_INCLUDED
#define Y_HASHING_TESTING_INCLUDED 1


#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Hashing
    {
        class Function;
        
        //______________________________________________________________________
        //
        //
        //! making tests for hashing functions
        //
        //______________________________________________________________________
        struct Testing
        {
            const char *text; //!< to process
            const char *hash; //!< hexadecimal result
                            
            //! run tests
            static void Run( Function &H, const Testing tests[], const unsigned count );
        };
        
        //! helper to run tests
#define Y_HASHING_TEST(FUNCTION,TESTS) do { \
/**/    FUNCTION H;\
/**/    Testing::Run(H,TESTS,sizeof(TESTS)/sizeof(TESTS[0]));\
/**/  } while(false)

    }

}

#endif
