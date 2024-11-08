
//! \file

#ifndef Y_Lingo_Syntax_Compound_Included
#define Y_Lingo_Syntax_Compound_Included 1

#include "y/lingo/syntax/internal.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            typedef Small::BareLightList<const Rule> Manifest;
            typedef Manifest::NodeType               RNode;

            //__________________________________________________________________
            //
            //
            //
            //! Compound
            //
            //
            //__________________________________________________________________
            class Compound : public Internal, public Proxy<const Manifest>
            {
            protected:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                //! setup with name, uuid and existing rule
                template <typename NAME> inline
                explicit Compound(const NAME   & _name,
                                  const uint32_t _uuid ) :
                Internal(_name,_uuid),
                manifest()
                {
                }



            public:
                virtual ~Compound() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! add existing rule to manifest
                void add(const Rule &);

                Compound & operator<<(const Rule &);
                

            protected:
                void vizLink(OutputStream &) const;
                void checkNotEmpty(const char * const fn,
                                   const char * const id) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Manifest manifest; //!< list of existing rules

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
                virtual ConstInterface & surrogate() const noexcept;

            };
        }

    }

}

#endif
