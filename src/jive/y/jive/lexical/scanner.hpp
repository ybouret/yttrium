//! \file

#ifndef Y_Lexical_Scanner_Included
#define Y_Lexical_Scanner_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/lexical/unit.hpp"
#include "y/jive/entity.hpp"
#include "y/functor.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {

            typedef Functor<unsigned,TL1(const Token &)> Action;

            class Callback : public Entity
            {
            public:
                typedef ArkPtr<String,Callback> Pointer;

                template <typename LABEL>
                explicit Callback(const LABEL  & l,
                                  const Motif  & m,
                                  const Action & a) :
                Entity(l), motif(m), action(a) {}      //!< setup
                virtual ~Callback() noexcept;          //!< cleanup

                Motif            motif;
                Action           action;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Callback);

            };


            class Scanner : public Entity
            {
            public:
                typedef Callback::Pointer Deed;

                //! setup with identifier
                template <typename LABEL>
                explicit Scanner(const LABEL &usr) :
                Entity(usr),
                plan(),
                dict( new Dictionary() )
                {
                }


                template <typename ID, typename RX, typename HOST, typename METHOD>
                inline void operator()(const ID &id, const RX &rx, HOST &host, METHOD meth)
                {
                    const Action action( &host, meth );
                    const Motif  motif( RegExp::Compile(rx, & *dict) );
                    const Deed   deed( new Callback(id,motif,action) );
                }

                unsigned produce(const Token &)
                {
                    return 0;
                }


                //! cleanup
                virtual ~Scanner() noexcept;

                SuffixSet<String,Deed> plan;
                ArcPtr<Dictionary>     dict;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                void append(const Deed &deed);
                
            };
        }
        
    }
}

#endif
