//! \file

#ifndef Y_Concurrent_Split_Trek_Included
#define Y_Concurrent_Split_Trek_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! base class for treks of indices
        //
        //
        //______________________________________________________________________
        class Trek
        {
        public:
            //__________________________________________________________________
            //
            //
            //! Kind of trek
            //
            //__________________________________________________________________
            enum Kind
            {
                Increase, //!< step=1, may use prefix ++
                Decrease, //!< step=-1, may use prefix --
                Standard  //!< |step| > 1
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Trek(const Kind  ) noexcept; //!< setup
            explicit Trek(const Trek &) noexcept; //!< copy
            virtual ~Trek()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Kind kind; //!< kind of trek

        private:
            Y_DISABLE_ASSIGN(Trek);
        };

    }

}

#endif


