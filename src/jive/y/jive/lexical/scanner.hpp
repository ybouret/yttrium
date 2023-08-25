//! \file

#ifndef Y_Lexical_Scanner_Included
#define Y_Lexical_Scanner_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/lexical/unit.hpp"
#include "y/functor.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {


            class Action : public Object, public Counted, public Tag
            {
            public:
                enum Type
                {
                    Produce,
                    Control
                };

                typedef Functor<Type,TL1(const Token &)> CallBack;
                typedef ArkPtr<String,Action>            Pointer;

                virtual ~Action() noexcept {}

                template <typename TAG, typename OBJECT, typename METHOD_POINTER>
                static inline Action *Create(TAG             &t,
                                             Pattern *        pattern,
                                             OBJECT          &host,
                                             METHOD_POINTER  &method)
                {
                    const Motif    m(pattern);
                    const CallBack c(&host,method);
                    return new Action(t,m,c);
                }


                Motif    motif;
                CallBack instr;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Action);
                template <typename TAG> inline
                explicit Action(TAG &            t,
                                const Motif    & m,
                                const CallBack & c) :
                Object(),
                Counted(),
                Tag(t),
                motif(m),
                instr(c)
                {
                }

            };

            class Scanner : public Tag
            {
            public:
                template <typename TAG>
                explicit Scanner(TAG &usr) : Tag(usr)
                {

                }

                virtual ~Scanner() noexcept;




            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                SuffixSet<String,Action::Pointer> adb;
            };
        }
        
    }
}

#endif
