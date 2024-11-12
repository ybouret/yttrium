//! \file

#ifndef Y_Lingo_Syntax_Aggregate_Included
#define Y_Lingo_Syntax_Aggregate_Included 1

#include "y/lingo/syntax/internal/compound.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            //
            //! Aggregate of existing rules
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
                static const uint32_t UUID = Y_FOURCC('X', 'A', 'G', 'G'); //!< identifier

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with name
                template <typename NAME> inline
                explicit Aggregate(const NAME & _name) :
                Compound(_name,UUID)
                {
                    Y_Lingo_Syntax_Rule(Aggregate);
                }

                //! cleanup
                virtual ~Aggregate() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                Aggregate & operator += (const Rule &); //!< alias to add()

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                //! at lease one is robust, checking redundancy
                virtual bool         robust()                     const; 
                virtual bool         accepts(Y_Lingo_Syntax_Args) const; //!< must accept all
                virtual void         vizMark(OutputStream &)      const; //!< graphViz code

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };


        }

    }

}

#endif

