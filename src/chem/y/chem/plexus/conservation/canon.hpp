//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chem/plexus/conservation.hpp"

namespace Yttrium
{
    namespace Chemical
    {



        //______________________________________________________________________
        //
        //
        //! light list of DEPENDENT conservations
        //
        //______________________________________________________________________
        typedef Small::BareLightList<const Conservation> Conservations;
        
        //______________________________________________________________________
        //
        //
        //
        //! list of dependent conservations
        //
        //
        //______________________________________________________________________
        class Canon : public Object, public Conservations
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Canon(const Conservation &first); //!< setup with first conservation
            virtual ~Canon() noexcept;                 //!< cleanup

            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool accepts(const Conservation &) const noexcept; //!< true if sharing a species
            bool accepts(const Canon &)        const noexcept; //!< true if sharing a species
            void compile();                                    //!< make repo and build aux-level indices
            void viz(OutputStream &fp, size_t &indx) const;    //!< display conservation, increase indices

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SpRepo repo; //!< involved species
            Canon *      next; //!< for list
            Canon *      prev; //!< for list
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Canon);
        };

    }

}

#endif

