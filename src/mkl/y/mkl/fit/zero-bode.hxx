
namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {



            template <>
            ZeroBode<real_t>:: ZeroBode(const size_t depth) :
            ZeroBodeInfo( depth <= 1 ? 1 : depth),
            x(size),
            y(size),
            lu(size)
            {
            }

            template <>
            ZeroBode<real_t>:: ~ZeroBode() noexcept
            {
            }

            template <>
            void ZeroBode<real_t>:: free() noexcept
            {
                Coerce(x).free();
                Coerce(y).free();
            }

            template <>
            void  ZeroBode<real_t>:: feed(const real_t xx, const real_t yy)
            {
                assert(x.size() == y.size());
                assert(x.size() <= size);

                {     Coerce(x) >> xx; }
                try { Coerce(y) >> yy; }
                catch(...) {
                    Coerce(x).popHead();
                    assert(x.size() == y.size());
                    throw;
                }
            }

            template <>
            real_t ZeroBode<real_t>::inferred()
            {
                static const real_t zero(0);
                static const real_t one(1);

                assert(x.size() == y.size());
                const size_t n = x.size();

                switch(n)
                {
                    case 0: return zero;
                    case 1: return y.head();
                    default:
                        break;
                }

                Matrix<real_t>               mu(n,n);
                CxxArray<real_t,MemoryModel> cf(n,zero);

                Scope::ConstIterator ix = x.begin();
                Scope::ConstIterator iy = y.begin();
                for(size_t i=1;i<=n;++i,++ix,++iy)
                {
                    cf[i]     = *iy;
                    mu[i][1]  = one;
                    {
                        const real_t xx = *ix;
                        real_t       xp = xx;
                        for(size_t j=2;j<=n;++j) mu[i][j] = (xp*=xx);
                    }
                }
                
                if(!lu.build(mu)) singular();
                lu.solve(mu,cf);

                std::cerr << "cf=" << cf << std::endl;

                return cf[1];

            }

            template <>
            void ZeroBode<real_t>:: save(const Core::String<char> &fileName) const
            {
                Libc::OutputFile fp(fileName);
                for( Scope::ConstIterator ix = x.begin(), iy = y.begin(); ix != x.end(); ++ix, ++iy)
                {
                    fp("%.15g %.15g\n", double(*ix), double(*iy) );
                }
            }

        }

    }

}
