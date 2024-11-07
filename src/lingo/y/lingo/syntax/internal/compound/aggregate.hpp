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

            class Aggregate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('X', 'A', 'G', 'G');

                template <typename NAME> inline
                explicit Aggregate(const NAME & _name) :
                Compound(_name,UUID)
                {
                    Y_Lingo_Syntax_Rule(Aggregate);
                }

                virtual ~Aggregate() noexcept;

                Aggregate & operator += (const Rule &);

                virtual void viz(OutputStream &fp)        const;
                virtual bool robust()            const noexcept;
                virtual bool accepts(Y_Lingo_Syntax_Args) const;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };


        }

    }

}

#endif

