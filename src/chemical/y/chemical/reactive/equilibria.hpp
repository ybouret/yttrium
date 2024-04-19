
//! \file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibrium/constant.hpp"
#include "y/chemical/type/entities.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class EqsFormat : public Entities, public Components::Formatting
        {
        public:
            explicit EqsFormat() noexcept {}
            virtual ~EqsFormat() noexcept {}

            std::ostream & display(std::ostream &os, Equilibrium &eq) const
            {
                pad(os << '<' << eq.name << '>',eq) << Equilibrium::Separator;
                print(os,eq)                        << Equilibrium::Separator;
                os << eq.K(0);
                return os;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(EqsFormat);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Equilibria is a set of equilibria
        //
        //
        //______________________________________________________________________
        class Equilibria :
        public Proxy<const Equilibrium::Set>,
        public EqsFormat
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Equilibrium::Set::ConstIterator ConstIterator; //!< alias
            typedef Equilibrium::Set::Iterator      Iterator;      //!< alias
            static const char * const               CallSign;      //!< "Chemical::Equilibria"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Equilibria();          //!< setup empty
            virtual ~Equilibria() noexcept; //!< cleanup

            //! display formatted
            friend std::ostream & operator<<(std::ostream&,const Equilibria&);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            size_t topLevel() const noexcept; //!< next topLevel

            //! insert a newly created equilibria
            template <typename EQUILIBRIUM> inline
            EQUILIBRIUM & insert(EQUILIBRIUM *eq)
            {
                const Equilibrium::Handle handle(eq);
                insert(handle);
                return *eq;
            }

            //! make a new constant equilibirum
            template <typename NAME> inline
            Equilibrium & make(const NAME &name, const XReal value)
            {
                return insert( new ConstantEquilibrium(name,topLevel(),value) );
            }

            //! check valid equilibria and update formatting
            void finalize();



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            Equilibrium::Set edb;
            void                    insert(const Equilibrium::Handle &);
            virtual ConstInterface &surrogate() const noexcept;
        };

    }

}

#endif

