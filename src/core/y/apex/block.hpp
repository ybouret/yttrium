

//! \file

#ifndef Y_Apex_Block_Included
#define Y_Apex_Block_Included 1

#include "y/apex/jig/jigs.hpp"
#include "y/apex/block/data.hpp"
#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //______________________________________________________________________
        //
        //
        //
        //! Data + representation
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

            class Briefly
            {
            public:
                Briefly(const Plan which, Block &block) noexcept;
                ~Briefly() noexcept;
            private:
                const Plan plan;
                Block     &host;
                Y_DISABLE_COPY_AND_ASSIGN(Briefly);
            };


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

            //! set all to zero, keep plan for futrure use
            void ldz() noexcept;

            //! set curr and dull from plan
            void relink() noexcept;

            //! sync dull after manual setting of curr
            void syncDull() noexcept;

            //! compute bits from current plan and syncDull
            void sync() noexcept;

            //! change representation
            Block & to(const Plan target) noexcept;

            //! return transformed representation
            template <Plan PLAN> inline
            Jig<PLAN> & make() noexcept {
                return to(PLAN).as<PLAN>();
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
        };

      

    }

}

#endif

