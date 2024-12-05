
//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {



            class Law : public Actors
            {
            public:
                static const char * const CallSign;
                typedef CxxListOf<Law>    List;
                

                explicit Law(const Actor::List &);
                virtual ~Law() noexcept;

                //! if <alpha|C> <0, return <alpha|C>^2/alpha^2 else 0
                xReal excess(XAdd &xadd, const XReadable &C, const Level L) const;

                //! copy source into target and modify actors
                void  excess(XAdd &xadd, XWritable &target, const XReadable  &source, const Level level) const;

                const xReal   denom; //!< alpha^2
                const XArray  alpha; //!< weights of actors
                const XMatrix xproj; //!< Id * alpha^2 - |alpha><alpha|


                Law * next;
                Law * prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                
            };

        }

    }

}

#endif
