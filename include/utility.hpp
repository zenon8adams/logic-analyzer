#pragma once

#include "context.hpp"
#include "logical-expression.hpp"
#include "binary-counter.hpp"
#include "postfix-gen.hpp"
#include "model/logic-model.hpp"
#include "model/table-model.hpp"

namespace Util
{
    template <typename LTp_, typename ... RTp_>
    DLL_LOCAL bool orAll(const LTp_& l, const RTp_&... r )
    {
        return ( ... || ( l == r ) );
    }

    DLL_LOCAL std::wstring WSRep(int c );

    DLL_LOCAL std::string Decode(unsigned val );

    DLL_LOCAL size_t nByte( unsigned char c );

    DLL_LOCAL int Join( std::string str, size_t idx, int count );

    std::wstring ToWString( const std::string& str );

    DLL_LOCAL std::string ToU8String(const std::wstring& ws );
}

extern "C" {

    DLL_EXPORT wchar_t *toWString( const char *str );
    DLL_EXPORT void freeArray( wchar_t **array, size_t size );
    DLL_EXPORT void freeString( wchar_t *c );
    DLL_EXPORT FILE *tty0();

}