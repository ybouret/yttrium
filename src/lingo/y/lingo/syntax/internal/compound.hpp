
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
                explicit Compound(const NAME   &  _name,
                                  const uint32_t  _uuid,
                                  Grammar * const _host) :
                Internal(_name,_uuid,_host),
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

                Compound & operator<<(const Rule     &);         //!< add a rule
                Compound & operator<<(const Manifest &);         //!< add a whole manifest
                Compound & operator<<(const String &      expr); //!< with hosted grammar/parser
                Compound & operator<<(const char * const  expr); //!< with hosted grammar/parser
                Compound & operator<<(const char          expr); //!< with hosted grammar/parser

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
                Manifest        manifest; //!< list of existing rules
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
                virtual ConstInterface & surrogate() const noexcept;
                void                     noGrammarFor(const Caption &) const;

            };
        }

    }

}

#endif
