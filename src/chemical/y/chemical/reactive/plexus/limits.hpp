
//! \file

#ifndef Y_Chemical_Limits_Included
#define Y_Chemical_Limits_Included 1

#include "y/chemical/reactive/plexus/limit.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::CoopHeavyList<Limit> LimitsType; //!< alias
        typedef LimitsType::NodeType        LimitsNode; //!< alias
        typedef LimitsType::ProxyType       LimitsBank; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! List of ordered limits
        //
        //
        //______________________________________________________________________
        class Limits : public Proxy<const LimitsType>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with shared banks for allocation
            explicit Limits(const LimitsBank &lbank,
                            const SBank      &sbank) noexcept;
            virtual ~Limits() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            bool contains(const Species &s) const noexcept; //!< mostly to debug
            bool validate()                 const noexcept; //!< mostly to debug

            void reset() noexcept;

            //! the extent make species disappear
            void operator()(const Species &s,
                            const xreal_t  x);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Limits);
            LimitsType  impl;
            const SBank repo;

            virtual ConstInterface & surrogate() const noexcept { return impl; }
        };

        class Boundaries
        {
        public:
            explicit Boundaries(const LimitsBank &lbank,
                                const SBank      &sbank) noexcept :
            reac(lbank,sbank),
            prod(lbank,sbank)
            {
            }

            virtual ~Boundaries() noexcept {}
            void     reset()      noexcept { reac.reset(); prod.reset(); }

            Limits reac;
            Limits prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundaries);
        };

        class Restraints
        {
        public:

            virtual ~Restraints();
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Restraints);
        };



    }

}

#endif

