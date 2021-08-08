#ifndef __TABLE_MODEL_HPP
#define __TABLE_MODEL_HPP

#include <sstream>
#include "logic-model.hpp"
#include "config.hpp"
#define BUFSIZE 1024

namespace Logic
{
    class DLL_EXPORT TableModel
    {
    public:

        explicit TableModel( LogicModel ml );
        ~TableModel();
        void AddOutputChannels( std::initializer_list<std::reference_wrapper<std::ostream>> os );
        void AddOutputChannel( FILE *channel );
        void SetDisplayModeAscending(bool asc );

        void Display() const;

    private:

        struct HeaderData
        {
            std::string header;

            std::size_t width;

            static const std::size_t padding = 4,
                    gap      = 6;

            HeaderData( std::string h, size_t w );
        };

        static size_t SpacingOf( const std::wstring& text );

        std::string Boxed( char value, size_t index ) const;

        void DisplayHeading() const;

        void BuildHeaders();

        void Flush() const;

        LogicModel logicModel;

        std::vector<HeaderData> headers;

        mutable std::vector<std::reference_wrapper<std::ostream>> strms;

        mutable std::stringstream _default;
        // For compatibility with C streams.
        mutable FILE **channel = nullptr;
        size_t n_channel;

        bool ascending{};

    };

}

extern "C" {

DLL_EXPORT Logic::TableModel *makeTableModel( Logic::LogicModel *lmodel );
DLL_EXPORT void addOutputChannel( Logic::TableModel *tmodel, FILE *fp );
DLL_EXPORT void setDisplayModeAscending( Logic::TableModel *tmodel, bool asc );
DLL_EXPORT void display( Logic::TableModel *tmodel );
DLL_EXPORT void destroyTableModelObject( Logic::TableModel *tmodel );

}

#endif