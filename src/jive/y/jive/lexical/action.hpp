
//! \file

#ifndef Y_Lexical_Action_Included
#define Y_Lexical_Action_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/lexical/unit.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {
            //__________________________________________________________________
            //
            //
            // generic callback to process detected token
            //
            //__________________________________________________________________
            typedef uint32_t                            Message;  //!< alias
            typedef Functor<Message,TL1(const Token &)> Callback; //!< alias

            //! predefined messages
            enum MessageID
            {
                LX_EMIT = 0x01,
                LX_DROP = 0x02,
                LX_ENDL = 0x04,
                LX_CNTL = 0x08
            };


            //__________________________________________________________________
            //
            //
            //! Action to be taken upon detected token
            //
            //__________________________________________________________________
            class Action : public Entity
            {
            public:
                typedef ArkPtr<String,Action> Pointer; //!< alias

                template <typename LABEL>
                explicit Action(const LABEL    & id,
                                const Motif    & pp,
                                const Callback & cb) :
                Entity(id), motif(pp), doing(cb) {}      //!< setup
                virtual ~Action() noexcept;              //!< cleanup

                Motif            motif; //!< motif to be match
                Callback         doing; //!< callback to be done upon match

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Action);

            };

        }

    }

}

#endif
