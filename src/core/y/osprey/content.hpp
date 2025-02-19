
//! \file


#ifndef Y_Osprey_Content_Included
#define Y_Osprey_Content_Included 1


#include "y/osprey/iproxy.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        class Residue;

        class Content : public IProxy
        {
        public:
            explicit Content(const IBank &bank,
                             const size_t indx);

            explicit Content(const Content &root,
                             const size_t   indx);
            virtual ~Content() noexcept;

            //! append new indx
            Content & operator<<(const size_t indx);

            //! steal residue
            Content & operator<<(Residue &);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Content);
            static SignType Compare(const INode *lhs, const INode *rhs) noexcept;
        };

    }

}

#endif

