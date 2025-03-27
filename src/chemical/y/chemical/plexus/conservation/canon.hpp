
//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            //__________________________________________________________________
            //
            //
            //
            // definition for group of laws
            //
            //
            //__________________________________________________________________
            typedef Small::BareLightList<const Law> LList; //!< alias
            typedef LList::NodeType                 LNode; //!< alias

            //__________________________________________________________________
            //
            //
            //
            //! list of interdependent laws
            //
            //
            //__________________________________________________________________
            class Canon : public Object, public LList, public Assembly
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Canon(const Law &first); //!< setup with first law
                virtual ~Canon() noexcept;        //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                bool     accepts(const Law &)  const noexcept; //!< if common actor
                bool     accepts(const Canon&) const noexcept; //!< if common actor
                void     update()                    noexcept; //!< update assembly
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Canon * next; //!< for list
                Canon * prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Canon);
            };

        }

    }

}

#endif

