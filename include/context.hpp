#ifndef __CONTEXT_HPP
#define __CONTEXT_HPP

#include <string>
#include <map>
#include "config.hpp"

namespace Logic
{
    class DLL_LOCAL Context
    {
    public:
        DLL_LOCAL void Lookup( const std::wstring& str, bool& result ) const;
        static DLL_LOCAL void Assign( const std::wstring& s, bool b );

    private:
        static std::map<std::wstring, bool> varTable;
    };

}

#endif