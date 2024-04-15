
#include "y/ink/format/options.hpp"
#include "y/type/nullify.hpp"
#include "y/oversized.hpp"
#include "y/associative/suffix/map.hpp"
#include "y/string/tokenizer.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{
    namespace Ink
    {




        typedef SuffixMap<String,String> FmtOptsDB; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! database of options
        //
        //
        //______________________________________________________________________
        class FormatOptions:: Code : public Oversized, public FmtOptsDB
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Vector<String,Memory::Pooled> Strings;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Code()                 : Oversized(), FmtOptsDB(), empty()    {}
            inline explicit Code(const Code &code) : Identifiable(), Collection(), Oversized(), FmtOptsDB(code), empty() {}
            inline virtual ~Code() noexcept {}

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char *callSign() const noexcept { return "Ink::FormatOptions"; }

            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________
            inline void load(const String &opts)
            {
                //--------------------------------------------------------------
                // split options separated by ':'
                //--------------------------------------------------------------
                Strings options;
                Tokenizer::AppendTo(options,opts,':');

                for(size_t i=1;i<=options.size();++i)
                {
                    //----------------------------------------------------------
                    // split 'name=value'
                    //----------------------------------------------------------
                    const String &option = options[i];
                    Strings words;
                    Tokenizer::AppendTo(words,option,'=');
                    if(words.size()!=2) throw Specific::Exception(callSign(), "invalid '%s'", option.c_str());
                    for(size_t i=words.size();i>0;--i)
                        Algo::Crop(words[i], isblank);
                    const String &key   = words[1];
                    const String &value = words[2];
                    if( !insert(key, value) ) throw Specific::Exception(callSign(), "multiple option '%s'", key.c_str());
                }

            }

            const String empty;

        private:
            Y_DISABLE_ASSIGN(Object);
        };

        FormatOptions:: ~FormatOptions() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        FormatOptions:: FormatOptions() : code( new Code() )
        {
        }

        FormatOptions:: FormatOptions(const FormatOptions &other) :
        code( new Code( *other.code ) )
        {
        }

        FormatOptions & FormatOptions:: operator<<(const String &opts)
        {
            assert(0!=code);
            code->load(opts);
            return *this;
        }


        FormatOptions & FormatOptions:: operator<<(const char *opts)
        {
            const String _(opts);
            return (*this) << _;
        }

        const String * FormatOptions:: query(const String &key) const noexcept
        {
            assert(0!=code);
            return code->search(key);
        }

        const String * FormatOptions:: query(const char   *key) const
        {
            const String _(key);
            return query(_);
        }

        const String * FormatOptions:: query(const char   key) const
        {
            const String _(key);
            return query(_);
        }

        const String & FormatOptions:: empty() const noexcept
        {
            assert(0!=code);
            return code->empty;
        }

        std::ostream & operator<<(std::ostream &os, const FormatOptions &opt)
        {
            assert(0!=opt.code);
            return (os << (*opt.code) );
        }

    }

}
