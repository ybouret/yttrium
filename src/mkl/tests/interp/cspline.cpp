#include "y/mkl/algebra/cyclic.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/v2d.hpp"
#include "y/container/cxx/array.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class CyclicSpline : public Writable< V2D<T> >
        {
        public:
            typedef V2D<T> Vertex;

            explicit CyclicSpline(const size_t n);
            virtual ~CyclicSpline() noexcept;

            
        private:
            class Code;
            Code *code;
            Y_DISABLE_COPY_AND_ASSIGN(CyclicSpline);
        };



        template <typename T>
        class CyclicSpline<T>::Code : public Object
        {
        public:
            typedef Memory::Dyadic         Model;
            typedef CxxArray<Vertex,Model> Vertices;
            typedef CxxArray<T,Model>      Tableau;

            inline explicit Code(const size_t n) :
            Object(),
            N(n),
            P(n),
            d2P(n),
            rhs(n)
            {
            }

            const size_t N;
            Vertices     P;
            Vertices     d2P;
            Tableau      rhs;
            Cyclic<T>    mu;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        template <typename T>
        CyclicSpline<T>:: CyclicSpline(const size_t n) : code( new Code(n) )
        {
        }

        template <typename T>
        CyclicSpline<T>:: ~CyclicSpline() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }


    }
}


using namespace Yttrium;




Y_UTEST(interp_cspline)
{
    
}
Y_UDONE()



