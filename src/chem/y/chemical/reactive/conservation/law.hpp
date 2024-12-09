
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


            //__________________________________________________________________
            //
            //
            //
            //! single conservation law
            //
            //
            //__________________________________________________________________
            class Law : public Actors, public GraphViz::Vizible
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< "Chemical::Conservation::Law"
                typedef CxxListOf<Law>    List;     //!< alias
                static const char * const Scheme;   //!< color scheme
                
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Law(const Actor::List &); //!< setup from a list of actors
                virtual ~Law() noexcept;           //!< cleanup
                Y_OSTREAM_PROTO(Law);              //!< display as Law

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! if <alpha|C> <0, return <alpha|C>^2/alpha^2 else 0
                xReal excess(XAdd &xadd, const XReadable &C, const Level L) const;

                //! copy source into target and modify actors
                void  project(XAdd &xadd, XWritable &target, const XReadable  &source, const Level level) const;

                //! emit GraphViz code
                void  viz(OutputStream &fp) const;

                bool  isRelatedTo(const Law &) const noexcept;


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const xReal   denom; //!< alpha^2
                const XArray  alpha; //!< weights of actors
                const XMatrix xproj; //!< Id * alpha^2 - |alpha><alpha|
                const size_t  place; //!< in laws

                Law * next; //!< for laws
                Law * prev; //!< for laws

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                
            };

        }

    }

}

#endif
