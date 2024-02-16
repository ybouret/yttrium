
namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {



            template <>
            ZeroBode<real_t>:: ZeroBode(const size_t depth) :
            ZeroBodeInfo( ),
            Snake<real_t>( depth <= 1 ? 1 : depth ),
            lu( size() )
            {
            }

            template <>
            ZeroBode<real_t>:: ~ZeroBode() noexcept
            {
            }





            template <>
            real_t ZeroBode<real_t>::inferred()
            {
                static const real_t zero(0);
                static const real_t one(1);

                const size_t n = size();


                switch(n)
                {
                    case 0: return zero;
                    case 1: return head();
                    default:
                        break;
                }

                Matrix<real_t>               mu(n,n);
                CxxArray<real_t,MemoryModel> cf(n,zero);

                Scope::Iterator it = begin();
                for(size_t i=1;i<=n;++i,++it)
                {
                    cf[i]     = *it;
                    mu[i][1]  = one;
                    {
                        const real_t xx = i;
                        real_t       xp = xx;
                        for(size_t j=2;j<=n;++j) mu[i][j] = (xp*=xx);
                    }
                }
                
                if(!lu.build(mu)) singular();
                lu.solve(mu,cf);

                {
                    std::cerr << cf[1];
                    for(size_t i=2;i<=n;++i)
                    {
                        std::cerr << "+(" << cf[i] << ")*x**" << i;
                    }
                    std::cerr << std::endl;
                }


                return cf[1];

            }

            template <>
            void ZeroBode<real_t>:: save(const String &fileName) const
            {
                Libc::OutputFile fp(fileName);

                unsigned i=1;
                for(ConstIterator it=begin();it != end();++it,++i)
                {
                    fp("%u  %.15g\n", i, double( *it ));
                }

            }



        }

    }

}
