
//! \file

#ifndef Y_Chemical_Engine_Included
#define Y_Chemical_Engine_Included 1

#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! internal data for solver
        //
        //
        //______________________________________________________________________
        class Engine : public ProBank, public ProList, public Assembly
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Engine();           //!< setup
            virtual ~Engine() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void  update()         noexcept; //!< update assembly with prospects
            void  reset()          noexcept; //!< clear assembly and prospects
            void  show(XMLog &xml) const;    //!< display prospects


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };

    }

}

#endif

