//! \file

#ifndef Y_Chemical_Conservation_Act_Included
#define Y_Chemical_Conservation_Act_Included 1

#include "y/chemical/plexus/conservation/law.hpp"


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
            //! group of dependent laws
            //
            //
            //__________________________________________________________________
            class Act : public Proxy<const Law::Group>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Act> List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Act(const Law &first); //!< setup
                virtual ~Act() noexcept;        //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                bool accepts(const Law &) const noexcept; //!< if law is linked to mine
                bool accepts(const Act &) const noexcept; //!< if one of the law is linked to mine
                void collect(const Law &);                //!< collect law
                void collect(const Act &);                //!< collect laws

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Act);
                virtual ConstInterface & surrogate() const noexcept;

                Law::Group my;
                
            public:
                Act * next; //!< for list
                Act * prev; //!< for list
            };

        }

    }

}

#endif
