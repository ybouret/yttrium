#include "y/lingo/source.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/nullify.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Lingo
    {

        class Source::Code : public Object
        {
        public:
            inline explicit Code(Module *h) noexcept :
            Object(), handle(h), cache()
            {
            }

            virtual ~Code() noexcept {}

            inline Char *get() { return cache.size>0 ? cache.popHead() : handle->get(); }

            inline bool ready()
            {
                if(cache.size>0)
                    return true;

                Char *ch = handle->get();
                if(!ch)
                    return false;
                
                cache.pushHead(ch);
                return true;
            }



            AutoPtr<Module> handle;
            Token           cache;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };



        Source:: Source(Module *h) :
        code(0)
        {
            if(0==h) throw Specific::Exception("Jive::Source","NULL module");
            AutoPtr<Module> guard(h);
            code = new Code(h);
            guard.relax();
        }


        Source:: ~Source() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }


        Char * Source:: get()
        {
            assert(0!=code);
            return code->get();
        }

        bool Source:: getch(Token &token)
        {
            assert(0!=code);
            assert(0==token.size);
            Char * ch = code->get();
            if(ch) { token << ch; return true; }
            else                  return false;
        }

        void Source:: put(Char *chr) noexcept
        {
            assert(0!=chr);
            assert(0!=code);
            code->cache.pushHead(chr);
        }

        void Source:: put(Token &token) noexcept
        {
            assert(0!=code);
            code->cache.mergeHead(token);
        }

        bool  Source:: ready()
        {
            assert(0!=code);
            return code->ready();
        }

        uint8_t Source:: peek() const noexcept
        {
            assert(0!=code);
            assert(code->cache.size>0);
            return **(code->cache.head);
        }

        size_t Source:: cached() const noexcept
        {
            assert(0!=code);
            return code->cache.size;
        }

        void Source:: skip() noexcept
        {
            assert(code!=0);
            assert(code->cache.size>0);
            delete code->cache.popHead();
        }

        void Source:: skip(size_t n) noexcept
        {
            assert(code!=0);
            assert(code->cache.size>=n);
            while(n-- > 0)
                delete code->cache.popHead();
        }



        void Source:: dup(const Token &token)
        {
            Token cpy(token);
            put(cpy);
        }


        void  Source:: newLine() noexcept
        {
            assert(0!=code);
            code->handle->newLine();
        }

        
        static inline bool isBlank(const Char *ch) noexcept
        {
            assert(0!=ch);
            static const char sep[] = " \t\r\n";
            return 0 != strchr(sep,**ch);
        }


        bool Source:: guess(Token &token)
        {
            token.release();
            while(true)
            {
                Char *ch = get();
                if(!ch) break;
                if(isBlank(ch))
                {
                    delete ch;
                    continue;
                }
                else
                {
                    token.pushTail(ch);
                    break;
                }
            }

            while(true)
            {
                Char *ch = get();
                if(!ch) break;
                if(isBlank(ch))
                {
                    put(ch);
                    break;
                }
                else
                {
                    token.pushTail(ch);
                    continue;
                }
            }

            
            return token.size > 0;
        }

        
    }

}
