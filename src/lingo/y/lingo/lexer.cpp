
#include "y/lingo/lexer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Lexer:: ~Lexer() noexcept
        {
            scanners.release();
            (void)liberate();
        }

        void Lexer:: initialize()
        {
            withhold();
            const ScanPtr ps = this;
            mustInsert(ps);
        }

        void Lexer:: mustInsert(const ScanPtr &ps)
        {
            if(!scanners.insert(ps))
                throw Specific::Exception(name->c_str(),"unexpected multiple '%s'", ps->name->c_str());
        }

        

        void Lexer:: restart() noexcept
        {
            lexemes.release();
            history.free();
            scanner = this;
        }

        void Lexer:: call(const Caption &label)
        {
            assert(0!=scanner);
            ScanPtr *pps = scanners.search( *label );
            if(0==pps)
                throw Specific::Exception(name->c_str(),"no [%s] to be called from [%s]", label->c_str(), scanner->name->c_str());

            history << *scanner;
            scanner = & **pps;
        }

        void Lexer:: back()
        {
            assert(0!=scanner);
            if(history.size<=0) throw Specific::Exception(name->c_str(),"cannot go back from [%s]", scanner->name->c_str() );
            scanner = & history.pullTail();
        }

    }

}


