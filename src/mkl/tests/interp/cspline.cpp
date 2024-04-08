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

            void update();


            virtual const char * callSign() const noexcept { return "CyclicSpline"; }
            virtual size_t       size() const throw() { return code->N; }

            const V2D<T> & operator[](const size_t i) const noexcept
            {
                return code->P[i];
            }

            V2D<T> & operator[](const size_t i)   noexcept
            {
                return code->P[i];
            }

            const Readable< V2D<T> > & accel() const noexcept
            {
                return code->d2P;
            }

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
            Nm1(N-1),
            P(n),
            d2P(n),
            rhs(n),
            cof(n),
            mu(n)
            {
                for(size_t i=N;i>0;--i)
                {
                    mu.b[i] = 4;
                    mu.a[i] = mu.c[i] = 1;
                }

                std::cerr << "mu=" << mu << std::endl;
            }

            inline virtual ~Code() noexcept {}

            inline T compute(const Vertex &prev,
                             const Vertex &curr,
                             const Vertex &next,
                             const size_t  dim) noexcept
            {

                const T mid = curr[dim];
                return (prev[dim]-mid) + (next[dim]-mid);
            }

            inline void update()
            {
                const T six = 6;
                for(size_t dim=1;dim<=2;++dim)
                {
                    rhs[1] = six * compute(P[N],P[1],P[2],dim);
                    for(size_t i=Nm1;i>1;--i)
                    {
                        rhs[i] = six * compute(P[i-1],P[i],P[i+1],dim);
                    }
                    rhs[N] = six * compute(P[Nm1],P[N],P[1],dim);

                    mu.solve(cof,rhs);
                    for(size_t i=N;i>0;--i)
                    {
                        d2P[i][dim] = cof[i];
                    }
                }
            }

            const size_t N;
            const size_t Nm1;
            Vertices     P;
            Vertices     d2P;
            Tableau      rhs;
            Tableau      cof;
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

        template <typename T>
        void CyclicSpline<T>:: update()
        {
            assert(0!=code);
            code->update();
        }


    }
}


using namespace Yttrium;




Y_UTEST(interp_cspline)
{
    MKL::CyclicSpline<double> S(3);

    S[1] = V2D<double>(1,0);
    S[2] = V2D<double>(0,1);
    S[3] = V2D<double>(-1,0);

    S.update();
    std::cerr << "S=" << S << std::endl;
    std::cerr << "accel=" << S.accel() << std::endl;
    
}
Y_UDONE()



