#include <utility.hpp>
#include "context.hpp"

void Logic::Context::Lookup( const std::wstring& str, bool& result ) const
{
    auto value = varTable.find( str );
    if( value != varTable.cend() )
        result = value->second;
}
void Logic::Context::Assign( const std::wstring& s, bool b )
{
    varTable[ s ] = b;
}

std::map<std::wstring, bool> Logic::Context::varTable;

void Assign( const wchar_t *str, bool result)
{
    Logic::Context::Assign( str, result);
}