
#include "y/chemical/reactive/equilibria/reactor.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        const char * const Reactor:: CallSign = "Chemical::Equilibria";
        
        Reactor:: ~Reactor() noexcept {}
        Reactor:: Reactor() :
        Equilibrium::Set(),
        Fragment(),
        K()
        {
        }

        void Reactor:: mustInsert(const Equilibrium::Handle &handle)
        {
            // check index
            const size_t nextIndex = size() + 1;
            if(nextIndex != handle->indx[TopLevel])
                throw Specific::Exception(CallSign,"invalid index for '%s'", handle->name.c_str());

            // syncrhonize constants
            {
                const xReal _0;
                K.adjust( nextIndex, _0);
            }

            // try to insert
            try
            {
                if( !insert(handle) )
                    throw Specific::Exception(CallSign, "multiple '%s'", handle->name.c_str());
                enroll( *handle );
            }
            catch(...)
            {
                K.popTail();
                assert( size() == K.size() );
                throw;
            }

            assert( size() == K.size() );
            K[ size() ] = handle->K(0);
        }



    }

}
