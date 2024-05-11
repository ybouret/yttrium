
//! \file

#ifndef Y_Chemical_Limits_Included
#define Y_Chemical_Limits_Included 1

#include "y/chemical/reactive/plexus/boundary.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::CoopHeavyList<Boundary> BList; //!< alias
        typedef BList::NodeType                BNode; //!< alias
        typedef BList::ProxyType               BBank; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! List of ordered boundaries
        //
        //
        //______________________________________________________________________
        class Boundaries : public Proxy<const BList>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with shared banks for allocation
            explicit Boundaries(const BBank &bbank,
                                const SBank &sbank) noexcept;
            virtual ~Boundaries() noexcept; //!< cleanup
            Boundaries(const Boundaries &); //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            bool contains(const Species &s) const noexcept; //!< mostly to debug
            bool validate()                 const noexcept; //!< mostly to debug
            void reset()                          noexcept; //!< free

            //! the extent make species disappear
            void operator()(const Species &s,
                            const xreal_t  x);

        private:
            Y_DISABLE_ASSIGN(Boundaries);
            BList       impl;
            const SBank repo;

            virtual ConstInterface & surrogate() const noexcept { return impl; }
        };

        
        class Limits
        {
        public:
            static const unsigned USE_NONE = 0x00;
            static const unsigned USE_REAC = 0x01;
            static const unsigned USE_PROD = 0x02;
            static const unsigned USE_BOTH = USE_REAC | USE_PROD;

            explicit Limits(const BBank &bbank,
                            const SBank &sbank) noexcept :
            reac(bbank,sbank),
            prod(bbank,sbank)
            {
            }

            Limits(const Limits &other) : reac(other.reac), prod(other.prod) {}

            virtual ~Limits() noexcept {}

            void     reset() noexcept { reac.reset(); prod.reset(); }
            unsigned state() const noexcept {
                unsigned         flag  = USE_NONE;
                if(reac->size>0) flag |= USE_REAC;
                if(prod->size>0) flag |= USE_PROD;
                return flag;
            }

            friend std::ostream & operator<<(std::ostream &os, const Limits &self)
            {
                os << '(';
                switch(self.state())
                {
                    case USE_NONE: break;
                    case USE_REAC: self.outReac(os); break;
                    case USE_PROD: self.outProd(os); break;
                    case USE_BOTH: self.outReac(os); os << '|'; self.outProd(os); break;
                }
                os << ')';
                return os;
            }

            Boundaries reac;
            Boundaries prod;
        private:
            Y_DISABLE_ASSIGN(Limits);
            void outReac(std::ostream &os) const { os << "reac=" << reac; }
            void outProd(std::ostream &os) const { os << "prod=" << prod; }

        };



    }

}

#endif

