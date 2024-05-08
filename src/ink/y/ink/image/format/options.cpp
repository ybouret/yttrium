
#include "y/ink/image/format/options.hpp"
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
        class Options:: Code : public Oversized, public FmtOptsDB
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

        Options:: ~Options() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Options:: Options() : code( new Code() )
        {
        }

        Options:: Options(const Options &other) :
        code( new Code( *other.code ) )
        {
        }

        Options & Options:: operator<<(const String &opts)
        {
            assert(0!=code);
            code->load(opts);
            return *this;
        }


        Options & Options:: operator<<(const char *opts)
        {
            const String _(opts);
            return (*this) << _;
        }

        const String * Options:: query(const String &key) const noexcept
        {
            assert(0!=code);
            return code->search(key);
        }

        const String * Options:: query(const char   *key) const
        {
            const String _(key);
            return query(_);
        }

        const String * Options:: query(const char   key) const
        {
            const String _(key);
            return query(_);
        }

        String & Options:: fetch(const String &key)
        {
            assert(0!=code);
            (void) code->insert(key,code->empty);
            return *(code->search(key));
        }

        String & Options:: fetch(const char * const key)
        {
            const String _(key); return fetch(_);
        }


        String & Options:: fetch(const char key)
        {
            const String _(key); return fetch(_);
        }

        const String & Options:: empty() const noexcept
        {
            assert(0!=code);
            return code->empty;
        }

        std::ostream & operator<<(std::ostream &os, const Options &opt)
        {
            assert(0!=opt.code);
            return (os << (*opt.code) );
        }

    }

}
