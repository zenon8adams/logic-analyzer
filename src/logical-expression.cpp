#include "logical-expression.hpp"
#include "utility.hpp"

#define DEFINE_BINARY( objName, opr, chrType ) \
    \
Logic::objName::objName( std::shared_ptr<Logic::LogicalExp> l, std::shared_ptr<Logic::LogicalExp> r ) \
: Logic::BinaryOp( std::move( l), std::move( r) ) \
{   \
}   \
    \
bool Logic::objName::Eval( Logic::Context& c ) \
{   \
    return opr; \
} \
    \
std::wstring Logic::objName::Name() const \
{   \
    return std::wstring( 1, chrType ); \
} \


Logic::VariableExp::VariableExp( std::wstring vn )
    : Logic::LogicalExp(), varName( std::move( vn))
{
}

bool Logic::VariableExp::Eval( Logic::Context& c )
{
    bool dVal{};
    c.Lookup( varName, dVal );
    return dVal;
}

Logic::BinaryOp::BinaryOp(  std::shared_ptr<LogicalExp>l, std::shared_ptr<LogicalExp>r )
        : left(std::move( l )), right(std::move( r ))
{
}

std::wstring Logic::BinaryOp::ToWString() const
{
    std::wstring sRep{ L"(" };
    sRep += left->ToWString();
    sRep += L' ';
    sRep += Name();
    sRep += L' ';
    sRep += right->ToWString();
    sRep += L')';
    return sRep;
}

Logic::NotExp::NotExp( std::shared_ptr<LogicalExp> e )
    : expr( std::move( e))
{
}

bool Logic::NotExp::Eval( Logic::Context& c )
{
    return !expr->Eval(c );
}

std::wstring Logic::NotExp::Name() const
{
    return Util::WSRep( NOT );
}

std::wstring Logic::NotExp::ToWString() const
{
    std::wstring sRep{ Name() };
    sRep += expr->ToWString();
    return sRep;
}

DEFINE_BINARY( OrExp, left->Eval( c ) |  right->Eval( c ), OR )
DEFINE_BINARY( AndExp,  left->Eval( c ) &  right->Eval( c ), AND )
DEFINE_BINARY( XorExp,  left->Eval( c ) ^ right->Eval( c ), XOR )
DEFINE_BINARY( IFExp, !(left->Eval(c) && !right->Eval(c)), IF )
DEFINE_BINARY( IFFExp, !(left->Eval( c ) ^  right->Eval( c )), IFF )