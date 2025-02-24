//! \file


#ifndef Y_Osprey_Content_Included
#define Y_Osprey_Content_Included 1


#include "y/osprey/iproxy.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        class Residue;

        
        //______________________________________________________________________
        //
        //
        //
        //! Content of QFamily, ordered
        //
        //
        //______________________________________________________________________
        class Content : public IProxy
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Content(const IBank &  bank,
                             const size_t   indx); //!< setup with first index
            explicit Content(const Content &root,
                             const size_t   indx); //!< setup from root + index
            Content(const Content &);              //!< duplicate
            virtual ~Content() noexcept;           //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! append new indx
            Content & operator<<(const size_t indx);

            //! steal residue
            Content & operator<<(Residue &);


            

        private:
            Y_DISABLE_ASSIGN(Content);
            static SignType Compare(const INode *lhs, const INode *rhs) noexcept;
        };

    }

}

#endif

