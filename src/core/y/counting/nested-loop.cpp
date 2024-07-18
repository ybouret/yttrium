#include "y/counting/nested-loop.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/nullify.hpp"
#include "y/quantized.hpp"
#include "y/kemp/natural.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    typedef CxxArray<size_t,Memory::Dyadic> LoopArray;

    class NestedLoop:: Code : public Quantized, public LoopArray
    {
    public:


        //! (prod(i=0:d-1) (n-i))/d!
        static Cardinality Count(const size_t d, const size_t n)
        {
            if(d<=0||n<d) throw Specific::Exception(CallSign, "invalid dims=%lu and nmax=%lu", (unsigned long)d, (unsigned long)n);
            const apn count = apCount(d,n);
            return count.cast<Cardinality>(CallSign);
        }


        inline explicit Code(const size_t d, const size_t n) :
        Quantized(),
        LoopArray(d),
        dims( size() ),
        imax( dims, 0)
        {
            assert(d>=1);
            assert(n>=d);
            {
                const size_t delta = n-d;
                for(size_t i=1;i<=d;++i)
                    Coerce(imax[i]) = i+delta;
            }
            boot();
        }


        inline virtual ~Code() noexcept {}

        // initialize to first state
        inline void boot() noexcept {
            for(size_t i=size();i>0;--i) (*this)[i] = i;
        }

        // get next state
        inline bool next() noexcept
        {
            return move( size() );
        }


        const size_t    dims;
        const LoopArray imax;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);

        static inline apn apCount(const size_t d,
                                  const size_t n)
        {
            assert(d>=1);
            assert(n>=d);
            apn res = 1;
            for(size_t i=0;i<d;++i) {
                res *= n-i;
            }
            return res / apn::Factorial(d);
        }

        bool move(const size_t k) noexcept
        {
            size_t &i = (*this)[k];
            if(i<imax[k])
            {
                ++i;
                return true;
            }
            else
            {
                if(k<=1)
                {
                    return false;
                }
                else
                {
                    if(move(k-1))
                    {
                        LoopArray &self = *this;
                        for(size_t j=k;j<=dims;++j)
                            self[j] = self[j-1] + 1;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }


    };

    const char * const NestedLoop::CallSign = "NestedLoop";


    NestedLoop:: ~NestedLoop() noexcept
    {
        assert(0!=code);
        Nullify(code);
    }


    Cardinality NestedLoop:: CardinalityFor(const size_t d, const size_t n)
    {
        return Code::Count(d,n);
    }

    NestedLoop:: NestedLoop(const size_t d,
                            const size_t n) :
    Schedule( CardinalityFor(d,n) ),
    code( new Code(d,n) )
    {

        std::cerr << "sizeof(Code)=" << sizeof(Code) << std::endl;
    }

    const char * NestedLoop:: callSign() const noexcept
    {
        return CallSign;
    }

    size_t NestedLoop:: size() const noexcept
    {
        assert(0!=code);
        return code->dims;
    }

    const size_t & NestedLoop:: operator[](const size_t i) const noexcept
    {
        assert(0!=code);
        return (*code)[i];
    }

    void NestedLoop:: doBoot() noexcept
    {
        assert(0!=code);
        code->boot();
    }

    bool NestedLoop:: doNext() noexcept
    {
        assert(0!=code);
        return code->next();
    }



}
