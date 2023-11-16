
#include "y/counting/partition.hpp"
#include "y/system/exception.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/counting/c/part.h"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/exception.hpp"

#include <cerrno>

namespace Yttrium
{

    const char * const Partition:: CallSign = "Partition";

    namespace
    {
        static inline void euler_sum(apn &sum, const List<apn> &l)
        {
            for(List<apn>::ConstIterator it=l.begin();it!=l.end();++it)
            {
                sum += *it;
            }
        }

        static inline void euler_update(Vector<apn> &p)
        {
            // initialize p to its new size
            apn  tmp = 0;
            p << tmp;
            const size_t n = p.size();


            List<apn>    add; // list of positive terms
            List<apn>    sub; // list of negative terms
            tmp = 1;
            for(size_t k=1;;++k)
            {
                const size_t k3  = k*3;
                const size_t km  = k*(k3-1)/2;
                const size_t kp  = k*(k3+1)/2;
                List<apn>   &dst = ( 0 == (k&1) ) ? sub : add;
                bool         brk = false;

                switch(Sign::Of(km,n) )
                {
                    case Negative: assert(km<n);  dst.pushTail(p[n-km]); break;
                    case __Zero__: assert(n==km); dst.pushTail(tmp);     break;
                    case Positive: brk = true; break;
                }

                switch(Sign::Of(kp,n) )
                {
                    case Negative: assert(kp<n);  dst.pushTail(p[n-kp]); break;
                    case __Zero__: assert(n==kp); dst.pushTail(tmp);     break;
                    case Positive:  break;
                }


                if(brk) break;
            }

            apn &lhs = p.tail(); euler_sum(lhs,add);
            apn  rhs = 0;        euler_sum(rhs,sub);
            if(rhs>=lhs) throw Libc::Exception(EINVAL,"invalid Euler's algorithm for partiton of %lu",(unsigned long)n);
            lhs-=rhs;
        }

        static inline apn euler(const size_t n)
        {
            if(n<=2) return n;
            Vector<apn> p(n,AsCapacity);
            apn tmp = 1; p << tmp;
            tmp=2;       p << tmp;
            for(size_t i=3;i<=n;++i)
                euler_update(p);
            return p.tail();
        }



        static inline size_t getCard(const size_t n)
        {
            if(n<1)   throw Specific::Exception(Partition::CallSign,"n<1");

            const apn np = Partition::Cardinal(n);
            return np.cast<size_t>(Partition::CallSign);
        }
    }

    apn Partition:: Cardinal(const size_t n)
    {
        return euler(n);
    }


    class Partition:: Code :
    public Object,
    public Memory::Wad<size_t,Memory::Pooled>
    {
    public:
        typedef Memory::Wad<size_t, Memory::Pooled> WadType;

        inline explicit Code(const size_t n) :
        Object(),
        WadType(n),
        data( lead() - 1),
        part()
        {
            assert(n>0);
            Y_Part_Init(&part,n);
            boot();
        }
        virtual ~Code() noexcept { }

        //! booting
        inline void boot() noexcept { Y_Part_Boot(&part,data); }

        //! get next config
        inline bool next() noexcept { return 0!= Y_Part_Next(&part,data); }



        size_t * const data;
        Y_Part         part;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };

    const char * Partition:: callSign() const noexcept
    {
        return CallSign;
    }

    Partition:: Partition(const size_t n) :
    Schedule( getCard(n) ),
    code( new Code(n) )
    {
        
    }

    Partition:: ~Partition() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }

    void Partition:: doBoot() noexcept
    {
        assert(0!=code);
        code->boot();
    }

    bool Partition:: doNext() noexcept
    {
        assert(0!=code);
        return code->next();
    }

    size_t Partition:: size() const noexcept
    {
        assert(0!=code);
        return code->part.k;
    }

    Partition::ConstType & Partition:: operator[](const size_t i) const noexcept
    {
        assert(0!=code);
        assert(i>=1);
        assert(i<=size());
        return code->data[i];
    }

    
    


}

