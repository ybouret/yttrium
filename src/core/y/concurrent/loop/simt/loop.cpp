
#include "y/concurrent/loop/simt/loop.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
        Loop *       SIMT_Loop:: operator->()       noexcept {  assert(0!=loop); return loop; }
        const Loop * SIMT_Loop:: operator->() const noexcept {  assert(0!=loop); return loop; }

        

        SIMT_Loop:: ~SIMT_Loop() noexcept { Coerce(loop) = 0; }
        SIMT_Loop::  SIMT_Loop(const SharedLoop &sl) noexcept :
        loop( & Coerce(*sl) )
        {
        }
    }

}


