#ifndef __UTILITY_HPP
#define __UTILITY_HPP

#include <cstring>
#include "utility.hpp"

std::wstring Util::WSRep(int c )
{
    return std::wstring( 1, c );
}

std::string Util::Decode(unsigned val )
{
    std::string sRep;
    if (val < 0x80) {
        sRep += (char)(val);
    } else if (val < 0x800) {
        sRep += (char)(0xC0U | (val >> 6U));
        sRep += (char)(0x80U | (val & 0x3FU));
    } else if (val < 0x10000)
    {
        sRep += (char)(0xE0U |  (val >> 12U));
        sRep += (char)(0x80U | ((val >>  6U) & 0x3FU));
        sRep += (char)(0x80U |  (val         & 0x3FU));
    } else if (val < 0x200000)
    {
        sRep += (char)(0xF0U |  (val >> 18U));
        sRep += (char)(0x80U | ((val >> 12U) & 0x3FU));
        sRep += (char)(0x80U | ((val >>  6U) & 0x3FU));
        sRep += (char)(0x80U |  (val         & 0x3FU));
    }else if( val < 0x4000000 )
    {
        sRep += (char)( 0xF8U | (  val >> 24U           ));
        sRep += (char)( 0x80U | (( val >> 18U ) & 0x3FU ));
        sRep += (char)( 0x80U | (( val >> 12U ) & 0x3FU ));
        sRep += (char)( 0x80U | (( val >> 6U )  & 0x3FU ));
        sRep += (char)( 0x80U | (  val          & 0x3FU ));
    }else if( val < 0x80000000 )
    {
        sRep += (char)( 0xFCU |  ( val >> 30U           ));
        sRep += (char)( 0xF8U | (( val >> 24U ) & 0x3FU ));
        sRep += (char)( 0x80U | (( val >> 18U ) & 0x3FU ));
        sRep += (char)( 0x80U | (( val >> 12U ) & 0x3FU ));
        sRep += (char)( 0x80U | (( val >> 6U )  & 0x3FU ));
        sRep += (char)( 0x80U |  ( val          & 0x3FU ));
    }
    return sRep;
}

size_t Util::nByte( unsigned char c )
{
    for( size_t n = 2; n <= 6; ++n )
    {
       bool val = (c >> n) ^ 0xFFU >> n;
       if( !val )
           return 8 - n;
    }
    return 1;
}

int Util::Join(std::string str, size_t idx, int count )
{
    if( count == 1 )
        return str[idx];

    int copy = count;
    while( copy >= 1 )
        str[idx + copy--] &= 0x3FU;

    str[ idx ] &= 0xFFU >> (count + 1);
    size_t i = ( count << 2U ) + ( count << 1 );
    int value = 0;
    while( count >= 0 )
    {
         value += str[ idx++ ] << i;
        --count;
        i -= 6;
    }
    return value;
}

DLL_EXPORT std::wstring Util::ToWString( const std::string& str )
{
    std::wstring wsRep;
    for( size_t i = 0uL; i < str.size(); )
    {
        size_t byteCount = nByte( str[i] );
        wsRep += Join( str, i, byteCount-1 );
        i += byteCount;
    }
    return wsRep;
}

std::string Util::ToU8String(const std::wstring& ws )
{
    std::string s;
    for( auto&& elm : ws )
        s += Decode( elm );
    return s;
}

extern "C" {

    DLL_EXPORT wchar_t *toWString( const char *str )
    {
        std::wstring wsRep = Util::ToWString( str );
        auto *pstr = (wchar_t *)calloc( sizeof(wchar_t), wsRep.size() );
        wmemcpy( pstr, wsRep.data(), wsRep.size() );
        return pstr;
    }

    DLL_EXPORT void freeArray( wchar_t **array, size_t size )
    {
        for ( size_t i = 0; i < size; ++i )
            free( array[ i] );
        free( array );
        array = nullptr;
    }

    DLL_EXPORT void freeString( wchar_t *c )
    {
        free( c );
    }

    FILE *tty0()
    {
        return stdout;
    }

}

#endif
