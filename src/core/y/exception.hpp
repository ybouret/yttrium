//! \file

#ifndef Y_Exception_Included
#define Y_Exception_Included 1

#include "y/config/starting.hpp"
#include "y/check/printf.hpp"
#include <exception>

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! base Exception class
    //
    //
    //__________________________________________________________________________
    class Exception : public std::exception
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const size_t ClassSize = 128;                              //!< this+data
        static const size_t StorySize = ClassSize-sizeof(std::exception); //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit Exception()                    noexcept;               //!< setup empty
        explicit Exception(const char *fmt,...) noexcept Y_PRINTF_API;  //!< setup formated
        virtual ~Exception()                    noexcept;               //!< cleanup
        Exception(const Exception &)            noexcept;               //!< copy

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual const char *what()                    const noexcept;        //!< defaut: "Exception"
        virtual const char *when()                    const noexcept;        //!< formated story
        Exception &         operator<< (const char *) noexcept;              //!< append text
        Exception &         operator<< (const char  ) noexcept;              //!< append char
        Exception &         add(const char *fmt,...)  noexcept Y_PRINTF_API; //!< append formated
        Exception &         pre(const char *fmt,...)  noexcept Y_PRINTF_API; //!< prepend formated
        void                display(const size_t indent=0) const;            //!< display to std::cerr

    protected:
        void format(const char * const fmt, void * const ptr) noexcept; //!< format with va_list

    private:
        Y_DISABLE_ASSIGN(Exception);
        void reset() noexcept;
        char story[StorySize];
    };

}

#endif

