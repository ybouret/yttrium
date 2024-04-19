
//! \file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibrium/constant.hpp"
#include "y/chemical/type/entities.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Equilibria : 
        public Proxy<const Equilibrium::Set>,
        public Entities,
        public Components::Formatting
        {
        public:
            typedef Equilibrium::Set::ConstIterator ConstIterator; //!< alias
            typedef Equilibrium::Set::Iterator      Iterator;      //!< alias
             static const char * const              CallSign;      //!< "Chemical::Equilibria"

            explicit Equilibria();
            virtual ~Equilibria() noexcept;
            
            friend std::ostream & operator<<(std::ostream&,const Equilibria&);

            size_t   level() const noexcept;
            //Iterator begin() noexcept;
            //Iterator end()   noexcept;


            template <typename EQUILIBRIUM> inline
            EQUILIBRIUM & insert(EQUILIBRIUM *eq)
            {
                const Equilibrium::Handle handle(eq);
                insert(handle);
                return *eq;
            }

            template <typename NAME> inline
            Equilibrium & operator()(const NAME &name, const XReal value)
            {
                return insert( new ConstantEquilibrium(name,level(),value) );
            }

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

