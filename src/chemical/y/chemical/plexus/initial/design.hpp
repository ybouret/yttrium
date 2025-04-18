
//! \file


#ifndef Y_Chemical_Initial_Design_Included
#define Y_Chemical_Initial_Design_Included 1

#include "y/chemical/plexus/initial/axiom.hpp"
#include "y/chemical/library.hpp"
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            class Design : public Quantized, public Entity, public Proxy< ListOf<Axiom> >
            {
            public:
                static const char * const          CallSign; //!< "Initial::Design"
                typedef Proxy< ListOf<Axiom> >     BaseType;
                typedef ArkPtr<String,Design>      Pointer;

                template <typename UUID>
                explicit Design(const UUID &uuid) :
                Entity( new String(uuid) ),
                BaseType(),
                my()
                {
                }

                template <typename UUID>
                explicit Design(const UUID &uuid, const Design &root) :
                Entity( new String(uuid) ),
                BaseType(),
                my(root.my)
                {
                }

                virtual ~Design() noexcept;

                void add(Axiom * const) noexcept;
                void build(XMLog          &xml,
                           XWritable      &C0,
                           const Library  &lib,
                           const Clusters &cls) const;





            private:
                Y_DISABLE_COPY_AND_ASSIGN(Design);
                Y_PROXY_DECL();
                ListOfCloneable<Axiom> my;
                

            };

          

        }

    }

}

#endif

