
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


        Lexical::Scanner & Lexer:: query(const Caption &label)
        {
            {
                ScanPtr *pps = scanners.search( *label );
                if(0!=pps) return **pps;
            }
            
            ScanPtr ps = new Scanner(label,*this);
            mustInsert(ps);
            return *ps;
        }
    }

}


