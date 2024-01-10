
#include "y/text/ascii/convert.hpp"
#include "y/system/exception.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/text/ops.hpp"
#include "y/string.hpp"
#include <cmath>

namespace Yttrium
{

    namespace ASCII
    {


        namespace
        {
#define WHERE (ctx?ctx:Core::Unknown)


            //
            // https://learn.microsoft.com/fr-fr/cpp/c-runtime-library/reference/atof-atof-l-wtof-wtof-l?view=msvc-170
            //

            template <typename T> static inline
            T MakeReal(const char * curr,
                       const size_t size,
                       const char * ctx)
            {
                static const T    oneTenth(0.1);
                static const T    ten(10);

                static const char fn[] = "ASCII::Convert::ToReal";
                assert(Good(curr,size));
                const char * const last = curr + size;

                //--------------------------------------------------------------
                // skip blanks
                //--------------------------------------------------------------
                while(curr<last && isspace(*curr)) ++curr;
                if(curr>=last)
                    throw Specific::Exception(fn,"empty content for '%s'", WHERE);

                //--------------------------------------------------------------
                // check for sign
                //--------------------------------------------------------------
                assert(curr<last);
                bool isNegative = false;
                switch(curr[0])
                {
                    case '+':
                        ++curr;
                        break;

                    case '-':
                        ++curr;
                        isNegative = true;
                        break;

                    default:
                        break;
                }

                //--------------------------------------------------------------
                // integer part
                //--------------------------------------------------------------
                if(curr>=last)
                    throw Specific::Exception(fn,"empty integer part for '%s'", WHERE);

                T res = 0;
                while(curr<last)
                {
                    const uint8_t c = *(curr++);

                    switch(c)
                    {
                        case '0': res *= ten;           break;
                        case '1': res *= ten; res += 1; break;
                        case '2': res *= ten; res += 2; break;
                        case '3': res *= ten; res += 3; break;
                        case '4': res *= ten; res += 4; break;
                        case '5': res *= ten; res += 5; break;
                        case '6': res *= ten; res += 6; break;
                        case '7': res *= ten; res += 7; break;
                        case '8': res *= ten; res += 8; break;
                        case '9': res *= ten; res += 9; break;
                        case '.':
                            goto FRACTIONAL_PART;

                        case 'e':
                        case 'E':
                        case 'd':
                        case 'D':
                            goto EXPONENT_PART;

                        default:
                            throw Specific::Exception(fn,"bad '%s' in integer part for '%s'", Printable::Char[c], WHERE);
                    }
                }
                goto RETURN;

                //--------------------------------------------------------------
                // fractional part
                //--------------------------------------------------------------
            FRACTIONAL_PART:
                {
                    T cf = oneTenth;
                    T fp = 0;
                    while(curr<last)
                    {
                        const uint8_t c = *(curr++);
                        switch(c)
                        {
                            case '0': break;
                            case '1': fp += cf;   break;
                            case '2': fp += 2*cf; break;
                            case '3': fp += 3*cf; break;
                            case '4': fp += 4*cf; break;
                            case '5': fp += 5*cf; break;
                            case '6': fp += 6*cf; break;
                            case '7': fp += 7*cf; break;
                            case '8': fp += 8*cf; break;
                            case '9': fp += 9*cf; break;

                            case 'e':
                            case 'E':
                            case 'd':
                            case 'D':
                                res += fp;
                                goto EXPONENT_PART;

                            default:
                                throw Specific::Exception(fn,"bad '%s' in fractional part for '%s'", Printable::Char[c], WHERE);
                        }
                        cf *= oneTenth;
                    }
                    res += fp;
                }
                goto RETURN;

                //--------------------------------------------------------------
                // exponent part
                //--------------------------------------------------------------
            EXPONENT_PART:
                {
                    if(curr>=last)
                        throw Specific::Exception(fn,"empty exponent content for '%s'", WHERE);

                    bool negativeExponent = false;
                    switch(*curr)
                    {
                        case '+': ++curr; break;
                        case '-': ++curr; negativeExponent = true; break;
                        default:
                            break;
                    }

                    if(curr>=last)
                        throw Specific::Exception(fn,"empty exponent part for '%s'", WHERE);

                    unsigned ex = 0;
                    while(curr<last)
                    {
                        const uint8_t c = *(curr++);
                        ex *= 10;
                        switch(c)
                        {
                            case '0':        break;
                            case '1': ++ex;  break;
                            case '2': ex+=2; break;
                            case '3': ex+=3; break;
                            case '4': ex+=4; break;
                            case '5': ex+=5; break;
                            case '6': ex+=6; break;
                            case '7': ex+=7; break;
                            case '8': ex+=8; break;
                            case '9': ex+=9; break;

                            default:
                                throw Specific::Exception(fn,"bad '%s' in exponent part for '%s'", Printable::Char[c], WHERE);
                        }
                    }
                    if(0!=ex)
                        res *= std::pow(negativeExponent?oneTenth:ten,ex);
                    
                }


            RETURN:
                return isNegative ? -res: res;

            }
        }

        template <>
        float Convert::ToReal<float>(const char *msg, const size_t len, const char *ctx)
        {
            return MakeReal<float>(msg,len,ctx);
        }

        template <>
        double Convert::ToReal<double>(const char *msg, const size_t len, const char *ctx)
        {
            return MakeReal<double>(msg,len,ctx);
        }

        template <>
        long double Convert::ToReal<long double>(const char *msg, const size_t len, const char *ctx)
        {
            return MakeReal<long double>(msg,len,ctx);
        }


        template <>
        float Convert::ToReal<float>(const char *msg, const char *ctx)
        {
            return MakeReal<float>(msg,StringLength(msg),ctx);
        }

        template <>
        double Convert::ToReal<double>(const char *msg, const char *ctx)
        {
            return MakeReal<double>(msg,StringLength(msg),ctx);
        }

        template <>
        long double Convert::ToReal<long double>(const char *msg, const char *ctx)
        {
            return MakeReal<long double>(msg,StringLength(msg),ctx);
        }


        template <>
        float Convert::ToReal<float>(const String &msg, const char *ctx)
        {
            return MakeReal<float>(msg.c_str(),msg.size(),ctx);
        }

        template <>
        double Convert::ToReal<double>(const String &msg, const char *ctx)
        {
            return MakeReal<double>(msg.c_str(),msg.size(),ctx);
        }

        template <>
        long double Convert::ToReal<long double>(const String &msg, const char *ctx)
        {
            return MakeReal<long double>(msg.c_str(),msg.size(),ctx);
        }

    }

}

