
#include "y/jive/pattern/matcher.hpp"
#include "y/jive/pattern/regexp.hpp"
#include "y/jive/source.hpp"

namespace Yttrium
{

    namespace Jive
    {
        

        Matcher:: ~Matcher() noexcept
        {
        }

        void Matcher:: initialize()
        {
            motif->query( Coerce(firstChars) );
        }

        Matcher:: Matcher(Pattern *p) noexcept :
        motif(p), firstChars()
        {
            initialize();
        }

        Matcher:: Matcher(const char *rx, const Dictionary *dict) :
        motif( RegExp::Compile(rx,dict) ), firstChars()
        {
            initialize();
        }

        Matcher:: Matcher(const String &rx, const Dictionary *dict) :
        motif( RegExp::Compile(rx,dict) ), firstChars()
        {
            initialize();
        }

        Matcher:: Matcher(const Matcher &other) :
        motif( other.motif->clone() ),
        firstChars( other.firstChars )
        {
        }

        void Matcher:: cleanUp() noexcept
        {
            Coerce(*motif).reset();
        }

        Token * Matcher:: exactly(Source &source)
        {
            Pattern &self = Coerce(*motif);
            self.reset();
            assert(0==self.size);

            if(self.takes(source))
            {
                if(source.ready())
                {
                    // still got some char!
                    source.put(self);
                    return 0;
                }
                else
                {
                    return &self;
                }
            }
            else
            {
                assert(0==self.size);
                return 0;
            }


        }

        Token * Matcher:: somehow(Source &source)
        {
            Pattern &self = Coerce(*motif);
            self.reset();
            assert(0==self.size);

        PROBE:
            if(!source.ready())            return   0; // End Of Source
            if(!firstChars[source.peek()]) 
            {
                source.skip();
                goto PROBE; // couldn't match
            }

            assert(source.cached()>0);
            if(self.takes(source))       return &self;

            assert(0==self.size);
            assert(source.cached()>0);
            
            source.skip();
            goto PROBE;

        }



    }

}
