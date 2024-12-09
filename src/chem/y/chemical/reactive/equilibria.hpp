
//! file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Library;

        class Constants : public Object, public Counted, public XVector
        {
        public:
            explicit Constants() noexcept {}
            virtual ~Constants() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Constants);
        };

        typedef ArcPtr<Constants> SharedConstants;


        //______________________________________________________________________
        //
        //
        //
        //! internal class for equilibria
        //
        //
        //______________________________________________________________________
        class Reactor : public Equilibrium::Set, public Fragment
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Reactor();
            virtual ~Reactor() noexcept;

            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void         mustInsert(const Equilibrium::Handle &); //!< insert/enroll

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SharedConstants sharedK;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };


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
            static const char * const      CallSign;      //!< "Chemical::Equilibria"
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
            void decl( Equilibrium * const );

            
            //! helper to declare a derived equilibrium
            template <typename EQTYPE> inline
            EQTYPE & add(EQTYPE * const eq)
            {
                decl(eq);
                return *eq;
            }

            void operator()(Library &lib, const String &     expr); //!< internally process expression as Weasel
            void operator()(Library &lib, const char * const expr); //!< internally process expression as Weasel


            //XReadable & K(const xReal t); //!< recompute constants


            void updateFragment() noexcept;        //!< update fragment for formatting all equilibria
            void graphViz(OutputStream &fp) const; //!< standalone graphViz

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
            Reactor reactor;

        public:
            XWritable & K;

        };


    }

}


#endif

