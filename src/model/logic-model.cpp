#include <unordered_map>
#include "model/logic-model.hpp"

Logic::LogicModel::LogicModel( std::vector<std::wstring>  postfix ): token(std::move( postfix ))
{
    buildModel();
}

void Logic::LogicModel::buildModel()
{

#define VARIABLE    1

#define SOLVE( Type )                                        \
    right = tmp.back();                                      \
    tmp.pop_back();                                          \
    left = tmp.back();                                       \
    tmp.pop_back();                                          \
    tmp.push_back( std::make_shared<Type>( left, right ) );  \
    str = tmp.back()->ToWString();                           \
    if( seen[ str ] )                                        \
        break;                                               \
    seen[ str ] = tmp.back();                                \
    model.push_back( tmp.back() );                           \
    break;

    size_t varSection{}, negSection{};
    std::wstring str;
    std::vector<std::shared_ptr<LogicalExp>> tmp;
    std::shared_ptr<LogicalExp> left, right;
    std::unordered_map<std::wstring, std::shared_ptr<LogicalExp>> seen;

    for( int opr = getOp( str ); opr != WEOF; opr = getOp( str ) )
    {
        switch( opr )
        {
            case NOT:
                right = tmp.back();
                tmp.pop_back();
                tmp.push_back( std::make_shared<NotExp>( right ) );
                str = tmp.back()->ToWString();
                if( seen[ str ] )
                    break;
                seen[ str ] = tmp.back();
                if( seen[ str.substr( 1) ] )
                    model.insert( model.begin() + varSection + negSection++,  tmp.back() );
                else
                    model.push_back( tmp.back() );
                break;
            case AND:
            SOLVE( AndExp )
            case XOR:
            SOLVE( XorExp )
            case OR:
            SOLVE( OrExp )
            case IF:
            SOLVE( IFExp )
            case IFF:
            SOLVE( IFFExp )
            case VARIABLE:
                if( seen[ str ] )
                {
                    tmp.emplace_back( seen[ str ]  );
                    break;
                }
                else
                    tmp.emplace_back( new VariableExp( str ) );

                seen[ str ] = tmp.back();
                model.insert( model.cbegin() + varSection++, tmp.back() );
                break;
            default:
                break;
        }
    }
    nVars = varSection;
}

int Logic::LogicModel::getOp( std::wstring& str )
{
    if( idx >= token.size() )
        return WEOF;
    str = token[ idx++ ];
    if( isalpha( str[0] ) )
        return VARIABLE;
    return str[0];
}

extern "C" {

    Logic::LogicModel *makeLogicModel( wchar_t **array, size_t size )
    {
        std::vector<std::wstring> m_array( size );
        for( auto& entry : m_array )
            entry = *array++;
        return new Logic::LogicModel( m_array );
    }

    void model( Logic::LogicModel *lmodel,
            LogicalExpCollection* obj_coll )
    {
        *obj_coll = lmodel->Model();
    }

    size_t numberOfVariable( Logic::LogicModel *lexp )
    {
        return lexp->NumberOfVariable();
    }

    void destroyLogicModelObject( Logic::LogicModel *lmodel )
    {
        delete lmodel;
    }

}