//! \file

#ifndef Y_UnitTests_Driver_Include
#define Y_UnitTests_Driver_Include 1

#include "y/config/starting.hpp"
#include "y/check/static.hpp"

#include <cstring>
#include <iostream>
#include <iomanip>

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! a unit test record
    //
    //__________________________________________________________________________
    class UnitTest
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef int (*proc)(int, char **); //!< local program interface

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        UnitTest(const char *, proc) noexcept; //!< setup
        ~UnitTest()                  noexcept; //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void display(std::ostream &, const size_t) const; //!< display centered
        bool is_near(const char *other) const noexcept;   //!< fuzzy search

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const char *  const name; //!< test name
        proc                func; //!< test entry point

    private:
        Y_DISABLE_COPY_AND_ASSIGN(UnitTest);
    };

    //__________________________________________________________________________
    //
    //
    //! a collection of unit tests
    //
    //__________________________________________________________________________
    class UnitTests
    {
    public:
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! record a new test, check if enough space and unique name
        int  operator()(const char     *name,
                        UnitTest::proc  func) noexcept;

        //! display tests or try to launch one with remaining args
        int operator()(int      argc,
                       char **  argv) noexcept;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        virtual ~UnitTests() noexcept;

    protected:
        //! setup with user's memory
        explicit UnitTests(void *addr, const size_t size) noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(UnitTests);
        void       clear() noexcept;
        UnitTest  *query(const char *name) noexcept;

        UnitTest    *utest;     //!< repository
        size_t       count;     //!< declared
        size_t       width;     //!< max name length
        const size_t capacity;  //!< initial capacity
    };


    //__________________________________________________________________________
    //
    //
    //! provider of memory for a given number of unit tests
    //
    //__________________________________________________________________________
    template <size_t N>
    class UnitTestsProvider : public UnitTests
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~UnitTestsProvider() noexcept {}

        //! setup with internal memory
        inline explicit UnitTestsProvider() noexcept :
        UnitTests(mock,N), mock() {
            Y_STATIC_CHECK(sizeof(mock_t)==sizeof(UnitTest),MismatchSizeofTest);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(UnitTestsProvider);
        struct mock_t { void *head; void *tail; };
        mock_t mock[N];
    };

}

//! declare a COUNT of possible unit tests
#define Y_UTEST_DECL(COUNT)                \
/**/  int main(int argc, char **argv) { \
/**/  static Yttrium::UnitTestsProvider<COUNT> utests;

//! register a new test
#define Y_UTEST(NAME) do {                                          \
/**/ int Y_Test_##NAME(int,char**);                                 \
/**/ static const char       name[] = #NAME;                        \
/**/ const        int        ret    = utests(name,Y_Test_##NAME);   \
/**/ if(0!=ret) return ret;                                         \
/**/ } while(false)

//! run/display tests
#define Y_UTEST_EXEC()         \
/**/    return utests(argc,argv); \
/**/  }


#endif


