//! \file

#ifndef Y_Data_Small_Supply_Included
#define Y_Data_Small_Supply_Included 1

#include "y/type/releasable.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    namespace Small
    {

        //______________________________________________________________________
        //
        //
        //
        //! Interface to NODEs supply
        //
        //
        //______________________________________________________________________
        class Supply : public Releasable
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit Supply() noexcept; //!< setup

        public:
            virtual ~Supply() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
        protected:
            void *zacquire(const size_t blockSize);                           //!< helper, acquire [blockSize]
            void  zrelease(void *blockAddr, const size_t blockSize) noexcept; //!< helper, release [blockSize] at blockAddr

            virtual void   *getFlat()                = 0; //!< specialized data block acquire
            virtual void    putFlat(void *) noexcept = 0; //!< specialized data block release

            //! protocol to create a new node from its arguments
            template <typename NODE> inline
            NODE * newNode(typename NODE::ParamType args)
            {
                NODE *node = static_cast<NODE *>(getFlat());
                try { return new (node) NODE(args); }
                catch(...)  { putFlat(node); throw; }
            }
            
        public:
            virtual size_t  stowage() const noexcept = 0; //!< flat nodes availability

            //! protocol to destruct a live node
            template <typename NODE> inline
            void destroy(NODE *node) noexcept { putFlat( Destructed(node) ); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Supply);
        };
    }

}

#endif

