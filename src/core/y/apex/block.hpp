//! \file

#ifndef Y_Apex_Block_Included
#define Y_Apex_Block_Included 1

#include "y/apex/block/jig/jigs.hpp"
#include "y/apex/block/data.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/counted.hpp"

namespace Yttrium
{
    namespace Apex
    {


        //______________________________________________________________________
        //
        //
        //
        //! Data + representations
        //
        //
        //______________________________________________________________________
        class Block : public DataBlock, public Jigs
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxPoolOf<Block> Pool; //!< alias

            


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Block(const unsigned _shift); //!< setup with shift
            virtual ~Block() noexcept;             //!< cleanup
            Y_OSTREAM_PROTO(Block);                //!< display current

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void    ldz()             noexcept; //!< set all to zero, reset to plan1
            void    relink()          noexcept; //!< set curr and dull from plan
            void    syncDull()        noexcept; //!< sync dull after manual setting of curr
            void    sync()            noexcept; //!< compute bits from current plan and syncDull
            Block & to(const Plan)    noexcept; //!< change representation

            //! return transformed representation
            template <Plan PLAN> inline
            Jig<PLAN> & make() noexcept {
                return to(PLAN).as<PLAN>();
            }

            uint32_t tag32()                  const noexcept; //!< crc WITHOUT entry,shift,range
            uint32_t crc32()                  const noexcept; //!< crc WITH    entry,shift,range
            Block *  duplicate(const Block * const) noexcept; //!< full duplication
            void     withhold()                     noexcept; //!< ++nref
            bool     liberate()                     noexcept; //!< --nref<=0
            size_t   quantity()               const noexcept; //!< nref

            //! in-place natural representation as required plan
            static const void * To(const Plan p, natural_t &n, size_t &w) noexcept;

            template <typename T>
            inline T cast() noexcept
            {
                static const Plan p = Plan( iLog2Of<T>::Value );
                const Jig<p> &    j = make<p>();
                union {
                    typename Jig<p>::Word w;
                    T                     t;
                } alias = { j.word[0] };
                return alias.t;
            }



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t  &bytes; //!< jig1.bytes
            Block *        next;  //!< for list/pool
            Block *        prev;  //!< for list
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
            size_t         nref;

        };

        



    }

}

#endif

