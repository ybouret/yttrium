//! \file


#ifndef Y_Chemical_Initial_Repertory_Included
#define Y_Chemical_Initial_Repertory_Included 1

#include "y/chemical/plexus/initial/axioms.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //__________________________________________________________________
        //
        //
        //
        //! Repertory of pre-compiled Axionms
        //
        //
        //__________________________________________________________________
        class Repertory : public Proxy<const Initial::Axioms::Set>
        {
        public:
            //______________________________________________________________
            //
            //
            // Definitions
            //
            //______________________________________________________________
            static const char * const                   CallSign;       //!< "Repertory"
            typedef Initial::Axioms::Set::ConstIterator ConstIterator;  //!< alias

            //______________________________________________________________
            //
            //
            // C++
            //
            //______________________________________________________________
            explicit Repertory();           //!< setup
            virtual ~Repertory() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Repertory);     //!< display

            //______________________________________________________________
            //
            //
            // Methods
            //
            //______________________________________________________________

            //! store axiom from parsed code
            void operator()(const XCode &xcode);

            //! get axioms by name
            template <typename NAME> inline
            const Initial::Axioms & operator[](const NAME &name) const {
                return query(name);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Repertory);
            Y_PROXY_DECL();
            Initial::Axioms::Set my;
            const Initial::Axioms & query(const String &) const;
            const Initial::Axioms & query(const char * const) const;
        };


    }

}

#endif

