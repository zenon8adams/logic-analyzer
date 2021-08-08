#ifndef __LOGIC_MODEL_HPP
#define __LOGIC_MODEL_HPP

#include <string>
#include <vector>
#include <memory>
#include "logical-expression.hpp"
#include "config.hpp"

namespace Logic
{

    class DLL_EXPORT LogicModel
    {
    public:

        explicit LogicModel( std::vector<std::wstring> postfix );

        [[nodiscard]]  auto Model() const{ return model; }

        [[nodiscard]] auto NumberOfVariable() const{ return nVars; }

    private:

        void buildModel();

        int getOp( std::wstring& str );

    private:

        std::vector<std::wstring> token;

        std::vector<std::shared_ptr<LogicalExp>> model;

        size_t idx{}, nVars{};
    };
}

extern "C" {

    using LogicalExpCollection = std::vector<std::shared_ptr<Logic::LogicalExp>>;

    DLL_EXPORT Logic::LogicModel *makeLogicModel( wchar_t ** array, size_t size );
    DLL_EXPORT void model( Logic::LogicModel *lmodel,
                          LogicalExpCollection *obj_coll );
    DLL_EXPORT size_t numberOfVariable( Logic::LogicModel *lexp );
    DLL_EXPORT void destroyLogicModelObject( Logic::LogicModel *lmodel );

}

#endif