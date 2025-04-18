
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

                Y_XML_COMMENT(xml, "compiling " << n << " axiom" << Plural::s(n));
                lib.ldz(C0);
                Matrix<apq>  B(n,m);
                XArray       rhs(n);
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
                        //Y_XMLOG(xml, std::setw(8) << rhs[i].str() << " @" << B_i);
                    }
                }

                Y_XML_COMMENT(xml, "computing pseudo-inverse");
                Matrix<apq> BB(n,n);
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=i;++j)
                    {
                        BB[i][j] = BB[j][i] = Dot(B[i],B[j]);
                    }
                }
                MKL::LU<apq> lu(n);
                if(!lu.build(BB))
                    throw Specific::Exception(CallSign,"singular description");

                Y_XML_COMMENT(xml, "finalizing dispatch matrix");
                Matrix<apq> iBB(n,n);
                lu.invert(BB,iBB);
                const Matrix<apq> BT(TransposeOf,B);
                Matrix<apq> B3(m,n);
                for(size_t j=1;j<=m;++j)
                {
                    for(size_t i=1;i<=n;++i)
                    {
                        B3[j][i] = Dot(BT[j],BB[i]);
                    }
                }
                std::cerr << "B3="  << B3  << std::endl;
                std::cerr << "rhs=" << rhs << std::endl;
                for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                {
                    const Species &sp = **it;
                    //sp(B_i,TopLevel) = axiom->weight(sp);
                }
            }

        }

    }

}
