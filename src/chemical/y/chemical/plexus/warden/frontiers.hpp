
//! \file

#ifndef Y_Chemical_Warden_Frontiers_Included
#define Y_Chemical_Warden_Frontiers_Included 1

#include "y/chemical/plexus/warden/fund.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! frontiers to sort multiple requirements
        //
        //
        //______________________________________________________________________
        class Frontiers : public FList
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            explicit Frontiers(const Fund &) noexcept; //!< setup
            virtual ~Frontiers()             noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Frontiers);                //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool sorted() const noexcept;                    //!< check sorted, mostly to debug
            void operator()(const xreal_t, const Species &); //!< append a new requirement


        private:
            const SBank sbank; //!< shared copy
            Y_DISABLE_COPY_AND_ASSIGN(Frontiers);

            FNode *make(const xreal_t  xi,
                        const Species &sp);

        };
    }

}

#endif
