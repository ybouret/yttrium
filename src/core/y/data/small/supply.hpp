
//! \file

#ifndef Y_Data_Small_Supply_Included
#define Y_Data_Small_Supply_Included 1

#include "y/type/releasable.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    namespace Small
    {

        class Supply : public Releasable
        {
        protected:
            explicit Supply() noexcept;

        public:
            virtual ~Supply() noexcept;

        protected:
            virtual void   *getFlat()                = 0;
            virtual void    putFlat(void *) noexcept = 0;
        public:
            virtual size_t  stowage() const noexcept = 0;

            template <typename NODE> inline
            void destroy(NODE *node) noexcept
            {
                putFlat( Destructed(node) );
            }

            template <typename NODE, typename ARGS> inline
            NODE * produce(ARGS args)
            {
                NODE *node = getFlat();
                try {
                    return new (node) NODE(args);
                }
                catch(...) { putFlat(node); throw; }
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Supply);
        };
    }

}

#endif

