
#include "y/chemical/plexus/initial/design.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            const char * const Design:: CallSign = "Initial::Design";

            Y_PROXY_IMPL(Design,my)


            Design:: ~Design() noexcept
            {

            }

            void Design:: add(Axiom * const axiom) noexcept
            {
                assert(0!=axiom);
                assert(!my.owns(axiom));
                my.pushTail(axiom);
            }

            static  inline apq Dot(const Readable<apq> &a, const Readable<apq> &b)
            {
                assert(a.size()==b.size());
                apq res = 0;
                for(size_t i=a.size();i>0;--i)
                {
                    res += a[i] * b[i];
                }
                return res;
            }

            
            void Design:: build(XMLog &xml,XWritable &C0, const Library &lib) const
            {

                const size_t                 m    = lib->size();
                const size_t                 n    = my.size;

                Y_XML_SECTION_OPT(xml,CallSign," |library|=" << m << " |axiom|=" << n);
                if(m<=0) {
                    Y_XML_COMMENT(xml, "no registered species");
                    return;
                }

                if(n<=0)
                {
                    Y_XML_COMMENT(xml, "no registered axiom");
                    return;
                }

                lib.ldz(C0);
                Matrix<xreal_t> B3(m,n);
                XArray       rhs(n);
                {
                    Y_XML_COMMENT(xml, "compiling " << n << " axiom" << Plural::s(n));
                    Matrix<apq>  B(n,m);
                    {
                        size_t i=1;
                        for(const Axiom *axiom=my.head;axiom;axiom=axiom->next,++i)
                        {
                            Writable<apq> &B_i = B[i];
                            for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                            {
                                const Species &sp = **it;
                                sp(B_i,TopLevel) = axiom->weight(sp);
                            }
                            rhs[i] = axiom->amount;
                            Y_XMLOG(xml, std::setw(8) << rhs[i].str() << " @" << B_i);
                        }

                    }

                    Y_XML_COMMENT(xml, "computing pseudo-inverse");
                    {
                        Matrix<apq> BB(n,n);
                        for(size_t i=1;i<=n;++i)
                        {
                            for(size_t j=1;j<=i;++j)
                            {
                                BB[i][j] = BB[j][i] = Dot(B[i],B[j]);
                            }
                        }
                        Y_XMLOG(xml, "B     = " << B);
                        Y_XMLOG(xml, "GramB = " << BB);
                        Matrix<apq> iBB(n,n);
                        {
                            MKL::LU<apq> lu(n);
                            if(!lu.build(BB))
                                throw Specific::Exception(CallSign,"singular description");
                            lu.invert(BB,iBB);
                        }
                        Y_XMLOG(xml, "iBB   = " << iBB);
                        Y_XML_COMMENT(xml, "finalizing dispatch matrix");
                        const Matrix<apq> BT(TransposeOf,B);
                        for(size_t j=1;j<=m;++j)
                        {
                            for(size_t i=1;i<=n;++i)
                            {
                                const apq q = Dot(BT[j],iBB[i]);
                                B3[j][i] = q.to<real_t>();
                            }
                        }
                    }
                }
                Y_XMLOG(xml,"mat=" << B3);
                Y_XMLOG(xml,"rhs=" << rhs);

                {
                    XAdd  xadd;
                    for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                    {
                        const Species &sp = **it;
                        sp(C0,TopLevel) = xadd.dot(sp(B3,TopLevel),rhs);
                    }
                }

                lib.show(std::cerr << "Cini=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;


            }

        }

    }

}
