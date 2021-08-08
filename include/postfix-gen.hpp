#ifndef __POSTFIX_GEN_HPP
#define __POSTFIX_GEN_HPP


#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>
#include "logical-expression.hpp"

namespace Logic
{
    class DLL_EXPORT PostfixGenerator
    {
    public:
        explicit PostfixGenerator(const std::wstring& input );
        auto postfixForm() const { return postfix; }

    private:
        void consumeWs();
        std::wstring getVariable();
        static bool isOperator(int c );
        static bool mayBeSpecialOp(int c );
        void generate();
        void reOrder( int c );
        static bool hasHigherPrecedence(int l, int r );
        bool unwindStackUntil(const std::function<bool(int)>& op );

    private:
        std::wstringstream strm;
        std::vector<int> oprStack;
        std::vector<std::wstring> postfix;
        static const std::unordered_map<int, int> precedenceTable;
        static const std::unordered_map<int, std::pair<std::string, Operator>> replOps;
    };
}

extern "C" {

    DLL_EXPORT Logic::PostfixGenerator *makePostfixGenerator( const wchar_t *input );
    DLL_EXPORT wchar_t **postfixForm( Logic::PostfixGenerator *gen, size_t *len );
    DLL_EXPORT void destroyPostfixObject( Logic::PostfixGenerator *obj );

}

#endif