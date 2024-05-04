//! \file

#ifndef Y_Ink_Filter_Sobel_Included
#define Y_Ink_Filter_Sobel_Included 1

#include "y/ink/ops/filter/square.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            //! Sobel3X
            struct Sobel3X
            {
                static const char * const Name;       //!< name
                static const int          Data[3][3]; //!< data
            };
            
            //! Sobel3X
            struct Sobel3Y
            {
                static const char * const Name;       //!< name
                static const int          Data[3][3]; //!< data
            };
            
            //! Sobel3
            struct Sobel3
            {
                static const char * const Name; //!< name
                typedef Sobel3X X;              //!< alias
                typedef Sobel3Y Y;              //!< alias
            };
            
            
            //! Sobel5X
            struct Sobel5X
            {
                static const char * const Name;       //!< name
                static const int          Data[5][5]; //!< data
            };
            
            //! Sobel5Y
            struct Sobel5Y
            {
                static const char * const Name;       //!< name
                static const int          Data[5][5]; //!< data
            };
            
            //! Sobel5
            struct Sobel5
            {
                static const char * const Name; //!< name
                typedef Sobel5X X;              //!< alias
                typedef Sobel5Y Y;              //!< alias
            };

            //! Sobel7X
            struct Sobel7X
            {
                static const char * const Name;       //!< name
                static const int          Data[7][7]; //!< data
            };
            
            //! Sobel7Y
            struct Sobel7Y
            {
                static const char * const Name;       //!< name
                static const int          Data[7][7]; //!< data
            };

            //! Sobel7
            struct Sobel7
            {
                static const char * const Name; //!< name
                typedef Sobel7X X;              //!< alias
                typedef Sobel7Y Y;              //!< alias
            };

        }
    }

}

#endif

