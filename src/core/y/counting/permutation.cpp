#include "y/counting/permutation.hpp"
#include "y/system/exception.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/counting/c/perm.h"

namespace Yttrium
{

    apn Permutation:: Cardinal(const size_t n)
    {
        return apn::Factorial(n);
    }

    const char * const Permutation::CallSign = "Permutation";

    static inline size_t getCard(const size_t n)
    {
        if(n<1) throw Specific::Exception(Permutation::CallSign,"n<1");
        const apn np = Permutation::Cardinal(n);
        return np.cast<size_t>(Permutation::CallSign);
    }

    class Permutation:: Code : public Object, public Memory::Wad<size_t,Memory::Pooled>
    {
    public:
        typedef Memory::Wad<size_t,Memory::Pooled> WadType;

        inline explicit Code(const size_t n) :
        Object(),
        WadType(n),
        indx( static_cast<size_t *>(workspace) - 1),
        perm()
        {
            Y_Perm_Init(&perm,n);
            Y_Perm_Boot(&perm,indx);
        }


        inline virtual ~Code() noexcept {}

        inline void boot() noexcept { Y_Perm_Boot(&perm,indx); }
        inline bool next() noexcept { return 0 != Y_Perm_Next(&perm,indx); }



        size_t * const indx;
        Y_Perm         perm;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };

    Permutation:: Permutation(const size_t n) :
    Schedule( getCard(n)  ),
    code( new Code(n) )
    {
    }

    Permutation:: ~Permutation() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }

    void Permutation:: doBoot() noexcept
    {
        code->boot();
    }

    bool Permutation:: doNext() noexcept
    {
        return code->next();
    }


    size_t Permutation:: size() const noexcept
    {
        return code->perm.n;
    }

    size_t Permutation:: capacity() const noexcept
    {
        return code->perm.n;
    }

    const size_t & Permutation:: operator[](const size_t i) const noexcept
    {
        assert(i>0);
        assert(i<=code->perm.n);
        return code->indx[i];
    }


}
