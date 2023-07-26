//! \file

#ifndef Y_Data_Small_Supply_Included
#define Y_Data_Small_Supply_Included 1

#include "y/type/releasable.hpp"
#include "y/type/destruct.hpp"
#include "y/data/list.hpp"
#include "y/data/pool.hpp"

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


        protected:
            //__________________________________________________________________
            //
            //
            // helper zeroed block of memory
            //
            //__________________________________________________________________
            void *zacquire(const size_t blockSize);                           //!< helper, acquire [blockSize]
            void  zrelease(void *blockAddr, const size_t blockSize) noexcept; //!< helper, release [blockSize] at blockAddr

            //__________________________________________________________________
            //
            //
            // managing blocks of memory
            //
            //__________________________________________________________________
            virtual void   *getFlat()                = 0; //!< specialized data block acquire
            virtual void    putFlat(void *) noexcept = 0; //!< specialized data block release

            //__________________________________________________________________
            //
            //
            // NODE building helpers
            //
            //__________________________________________________________________

            //! protocol to create a new node from its arguments
            template <typename NODE> inline
            NODE * newNode(typename NODE::ParamType args)
            {
                NODE *node = static_cast<NODE *>(getFlat());
                try { return new (node) NODE(args); }
                catch(...)  { putFlat(node); throw; }
            }

            //! protocol to duplicate a node
            template <typename NODE> inline
            NODE * dupNode(const NODE &source)
            {
                NODE *node = static_cast<NODE *>(getFlat());
                try { return new (node) NODE(source); }
                catch(...)  { putFlat(node); throw; }
            }


            
        public:
            //__________________________________________________________________
            //
            //
            // public interface
            //
            //__________________________________________________________________
            
            virtual size_t  stowage() const noexcept = 0; //!< flat nodes availability

            //! protocol to destruct a live node
            template <typename NODE> inline
            void destroy(NODE *node) noexcept { putFlat( Destructed(node) ); }

            //! helper to destroy of full live list
            template <typename NODE> inline
            void destroy(ListOf<NODE> &linked) noexcept {
                while(linked.size>0) destroy( linked.popTail() );
            }

            //! helper to destroy a full live pool
            template <typename NODE> inline
            void destroy(PoolOf<NODE> &linked) noexcept {
                while(linked.size>0) destroy( linked.query() );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Supply);
        };
    }

}

#endif

