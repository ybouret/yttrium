
namespace Yttrium
{
    namespace Core
    {

        template <>
        class Tokenizer<CH>:: Code : public Object
        {
        public:
            inline virtual ~Code() noexcept {}

            inline explicit Code(const CH *entry, const size_t count) noexcept :
            curr(entry),
            stop(entry),
            last(entry+count)
            {
                assert(Good(entry,count));
            }


            inline bool next(const CH * const sep, const size_t num) noexcept
            {
                assert(Good(sep,num));

                // skip separators
                while(stop<last && in(sep,num,*stop) ) ++stop;
                assert(stop==last || notIn(sep,num,*stop) );
                if( (curr = stop) >= last ) return false;

                // move stop
                while(stop<last && notIn(sep,num,*stop) ) ++stop;

                return true;
            }

            const CH *       curr;
            const CH *       stop;
            const CH * const last;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            static inline bool in(const CH * const sep, const size_t num, const CH c) noexcept
            {
                assert(Good(sep,num));
                for(size_t i=0;i<num;++i)
                {
                    if (c==sep[i]) return true;
                }
                return false;
            }

            static inline bool notIn(const CH * const sep, const size_t num, const CH c) noexcept
            {
                assert(Good(sep,num));
                for(size_t i=0;i<num;++i)
                {
                    if (c==sep[i]) return false;
                }
                return true;
            }



        };

        template <> Tokenizer<CH>:: Tokenizer(const CH *entry, const size_t count) :
        code( new Code(entry,count) )
        {

        }

        template <> Tokenizer<CH>:: Tokenizer(const CH *entry) :
        code( new Code(entry,StringLength(entry)) )
        {

        }


        template <> Tokenizer<CH>:: Tokenizer(const String<CH> &s) :
        code( new Code(s(),s.size()) )
        {

        }

        
        template <> Tokenizer<CH>:: ~Tokenizer() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        template <>
        const CH * Tokenizer<CH>:: token() const noexcept
        {
            assert(0!=code);
            assert(0!=code->curr);
            return code->curr;
        }

        template <>
        size_t Tokenizer<CH>:: units() const noexcept
        {
            assert(0!=code);
            assert(0!=code->curr);
            assert(0!=code->stop);
            assert(code->stop>=code->curr);
            return static_cast<size_t>(code->stop-code->curr);
        }

        template <>
        bool Tokenizer<CH>::  next(const CH * const sep, const size_t num) noexcept
        {
            assert(0!=code);
            return code->next(sep,num);
        }


        template <>
        bool Tokenizer<CH>::  next(const CH * const sep) noexcept
        {
            assert(0!=code);
            return code->next(sep,StringLength(sep));
        }

        template <>
        bool Tokenizer<CH>::  next(const CH c) noexcept
        {
            assert(0!=code);
            return code->next(&c,1);
        }

        template <>
        bool Tokenizer<CH>::  next(const String<CH> &s) noexcept
        {
            assert(0!=code);
            return code->next(s(),s.size());
        }
    }
}
