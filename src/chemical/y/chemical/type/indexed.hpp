
//! \file


#ifndef Y_Chemical_Indexed_Included
#define Y_Chemical_Indexed_Included 1

#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        enum Level
        {
            TopLevel,
            SubLevel,
            AuxLevel
        };

        //______________________________________________________________________
        //
        //
        //
        //! Base class for indexed entities
        //
        //
        //______________________________________________________________________
        class Indexed : public Entity
        {
        public:
            static const size_t MaxLevel = AuxLevel+1;


            explicit Indexed(const String *, const size_t i) noexcept;
            virtual ~Indexed() noexcept;


            const size_t indx[MaxLevel];
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
            void setup(const size_t i) noexcept;
        };

    }

}

#endif

