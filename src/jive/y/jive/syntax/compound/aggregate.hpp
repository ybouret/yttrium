
//! \file

#ifndef Y_Jive_Syntax_Aggregate_Included
#define Y_Jive_Syntax_Aggregate_Included 1

#include "y/jive/syntax/compound.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //
            //! Aggregation of Rule(s)
            //
            //
            //__________________________________________________________________
            class Aggregate : public Compound
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const uint32_t     UUID = Y_FOURCC('A','G','G',':'); //!< alias
                static const char * const CallSign;                         //!< "Aggregate"

                //! possible properties
                enum Property
                {
                    Permanent, //!< permanent aggregate, design of grammar
                    Surrogate, //!< fusion content if single node
                    Transient  //!< never appears, used only for grouping
                };



                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! forward name and uuid
                template <typename ID>
                explicit Aggregate(const ID &id, const Property ppty) :
                Compound(id,UUID), property(ppty)
                {
                    I_am<Aggregate>(this);
                }

                //! cleanup
                virtual ~Aggregate() noexcept;

                //______________________________________________________________
                //
                //
                // Method
                //
                //______________________________________________________________

                //! accept all rules on manifest
                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const;

                //! GraphViz
                virtual void vizCore(OutputStream &) const;

                //! frail if ALL manifest is frail
                virtual bool isFrail() const noexcept;

                //! forward to manifest
                Aggregate & operator<<(const Rule &);

                //! helper
                Aggregate & operator +=(const Rule &);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Property property; //!< behavior

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };

        }

    }

}

#endif
