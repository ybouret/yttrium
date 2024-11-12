
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
            //__________________________________________________________________
            //
            //
            // Definitions for Compound
            //
            //__________________________________________________________________
            typedef Small::BareLightList<const Rule> Manifest; //!< alias
            typedef Manifest::NodeType               RNode;    //!< alias

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
                // Interface
                //
                //______________________________________________________________
                virtual void vizLink(OutputStream &fp) const;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! add existing rule to manifest
                void add(const Rule &);

                //! alias to add()
                Compound & operator<<(const Rule &);

                //! make name for manifest with given separator
                static String MakeNameFor(const Manifest &, const char);

            protected:
                

                //! check that rule 'id' is not empty in calling 'fn'
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
