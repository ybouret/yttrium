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

namespace Yttrium
{
    namespace Chemical
    {
        void Plexus:: operator()(XMLog     &             xml,
                                 XWritable &             C0,
                                 const Initial::Axioms & axioms,
                                 Reactor::Proc * const   callback)
        {
            Plexus &self = *this;
            Initial::Design design(axioms,lib,cls);

            XArray      Cs(CopyOf,C0);
            XMatrix     Qr;
            design.build(xml,Cs,Qr,lib,cls);

            XArray C1(CopyOf,Cs);

            self(xml,C1,callback);

            lib.show(std::cerr << "C1=", "\t[", C1, "]", xreal_t::ToString ) << std::endl;

            std::cerr << "Cs=" << Cs << std::endl;
            std::cerr << "C1=" << C1 << std::endl;
            std::cerr << "Qr=" << Qr << std::endl;


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

