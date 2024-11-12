//! \file

#ifndef Y_Lingo_Syntax_Wildcard_Included
#define Y_Lingo_Syntax_Wildcard_Included 1

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
            //
            //! Wildcard using an existing ROBUST rule
            //
            //
            //__________________________________________________________________
            class Wildcard : public Internal
            {
            protected:
                static const char Constructor[]; //!< "constructor"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                //! setup with name, uuid and existing rule
                template <typename NAME> inline
                explicit Wildcard(const NAME   & _name,
                                  const uint32_t _uuid,
                                  const Rule   & _rule) :
                Internal(_name,_uuid),
                rule(_rule)
                {
                    checkRobustness(Constructor);
                }

            public:
                //! cleanup
                virtual ~Wildcard() noexcept;


                virtual void vizLink(OutputStream &fp) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Rule & rule; //!< existing ROBUST rule

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
                
            protected:
                void checkRobustness(const char * const) const; //!< check robustness
            };
        }

    }

}

#endif


