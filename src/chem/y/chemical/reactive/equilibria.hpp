
//! file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibria/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Library;
        
        //______________________________________________________________________
        //
        //
        //
        //! database of equilibria
        //
        //
        //______________________________________________________________________
        class Equilibria : public Proxy<const Reactor>
        {
        public:
            typedef Reactor::ConstIterator ConstIterator; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Equilibria();          //!< create empty
            virtual ~Equilibria() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Equilibria);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! declare a new equilibrium
            void decl(Equilibrium * const );

            
            //! helper to declare a derived equilibrium
            template <typename EQTYPE> inline
            EQTYPE & add(EQTYPE * const eq)
            {
                decl(eq);
                return *eq;
            }

            void operator()(Library &lib, const String &     expr); //!< internally process expression as Weasel
            void operator()(Library &lib, const char * const expr); //!< internally process expression as Weasel

            XReadable & updateK(xReal) const; //!< sequential update of constants

            void updateFragment() noexcept;        //!< update fragment for formatting all equilibria
            void graphViz(OutputStream &fp) const; //!< standalone graphViz

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
            Reactor my;

        public:
            XWritable & K; //!< synchronized K

        };


    }

}


#endif

