#include "y/chemical/plexus.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Plexus:: CallSign = "Plexus";

        static inline
        const Library & PlexusCheck(const Library &lib)
        {
            if( !lib.latched ) throw Specific::Exception(Plexus::CallSign, "%s must be latched", Library::CallSign);
            return lib;
        }

        static inline
        Equilibria & PlexusCheck(Equilibria &eqs)
        {
            if( eqs.latched ) throw Specific::Exception(Plexus::CallSign, "%s must be unlatched", Equilibria::CallSign);
            return eqs;
        }


        Plexus:: Plexus(XMLog         &  xml,
                        const Library & _lib,
                        Equilibria    & _eqs,
                        const xreal_t   t0) :
        lib( PlexusCheck(_lib) ),
        eqs( PlexusCheck(_eqs) ),
        cls( xml, _eqs, lib, t0 ),
        eqz(cls),
        rxn(cls)
        {

        }

        Plexus:: ~Plexus() noexcept
        {
        }

        void Plexus:: operator()(XMLog     &xml,
                                 XWritable &C0,
                                 Reactor::Proc * const callback)
        {
            eqz(xml,C0);
            lib.show(std::cerr << "Ceqz=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
            rxn(xml,C0,callback);
            lib.show(std::cerr << "Crxn=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        }

    }

}


#include "y/chemical/plexus/initial/design.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        static inline void saveC(OutputStream &fp, const unsigned count, const XReadable &C)
        {
            fp("%u",count);
            for(size_t i=1;i<=C.size();++i)
                fp(" %.15g", double(C[i]));
            fp << '\n';
        }

        void Plexus:: operator()(XMLog     &             xml,
                                 XWritable &             C0,
                                 const Initial::Axioms & axioms,
                                 Reactor::Proc * const   callback)
        {

            Y_XML_SECTION(xml, "Plexus::Initial");

            // construct design from axions
            Plexus &        self = *this;
            const size_t    M    = lib->size();
            Initial::Design design(axioms,self.lib,self.cls);
            const xreal_t   zero;

            // initialize Cs and Qr
            XArray      Cs(M,zero);
            XMatrix     Qr;
            design.build(xml,Cs,Qr,lib,cls);

            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                const Cluster &cluster = *cl;
                iMatrix        Nu(cluster.N,M);
                const EList   &primary = cluster.order[1]; assert(Nu.rows==primary.size);
                {
                    size_t i=1;
                    for(const ENode *en = primary.head; en; en=en->next,++i )
                    {
                        const Equilibrium &eq = **en;

                        eq.fillTopology(Nu[i],TopLevel);
                        std::cerr << "with " << eq.name << " @ " << Nu[i] << std::endl;
                    }
                }

                std::cerr << "Nu=" << Nu << std::endl;
            }


            throw Exception("emergency stop before solving");

            // create first solved solution
            XArray C1(CopyOf,Cs); assert(M==C1.size());
            XArray C2(M,0);
            self(xml,C1,callback);

            unsigned   count=0;
            OutputFile fp("init.dat");

            saveC(fp,count,C1);

            lib.show(std::cerr << "Cs=", "\t[", Cs, "]", xreal_t::ToString ) << std::endl;
            lib.show(std::cerr << "C1=", "\t[", C1, "]", xreal_t::ToString ) << std::endl;


            std::cerr << "Cs=" << Cs << std::endl;
            std::cerr << "C1=" << C1 << std::endl;
            std::cerr << "Qr=" << Qr << std::endl;


            for(size_t iter=0;iter<100;++iter)
            {
                for(size_t i=M;i>0;--i)
                {
                    xadd.free();
                    xadd << Cs[i];
                    for(size_t j=M;j>0;--j) xadd << Qr[i][j] * C1[j];
                    C2[i] = xadd.sum();
                }
                self(xml,C2,callback);
                std::cerr << "Cs=" << Cs << std::endl;
                std::cerr << "C1=" << C1 << std::endl;
                std::cerr << "Qr=" << Qr << std::endl;
                std::cerr << "C2=" << C2 << std::endl;

                bool converged = true;
                for(size_t i=M;i>0;--i)
                {
                    const xreal_t dc = C1[i]-C2[i];
                    C1[i] = C2[i];
                    if( dc.abs().mantissa > 0 ) converged = false;
                }
                std::cerr << "converged=" << converged << std::endl;
                ++count;
                saveC(fp,count,C1);

            }

            throw Exception("emergency stop before solving");

        }


        xreal_t Plexus:: mix(XMLog           &xml,
                             XWritable       &Cm,
                             const XReadable &Ca, const xreal_t Va,
                             const XReadable &Cb, const xreal_t Vb,
                             Reactor::Proc * const callback)
        {
            const xreal_t Vnew = Va+Vb; assert(Vnew>0.0);
            size_t i=lib->size();
            for(Library::ConstIterator it=lib->begin();i>0;--i,++it)
            {
                const Species &sp = **it;
                sp(Cm,TopLevel) = (Va * sp(Ca,TopLevel) + Vb * sp(Cb,TopLevel))/Vnew;
            }
            (*this)(xml,Cm,callback);
            return Vnew;
        }

    }

}

