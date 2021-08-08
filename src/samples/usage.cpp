#include <iostream>
#include <deque>
#include <fstream>
#include "utility.hpp"

int main( int argc, char *argv[] )
{
    std::string input;
    std::cout << "KEY: XOR( ^, @ ), OR( |, \\/ ), AND( &, /\\ ), NOT( ~, ! ), IF( =>, -> ), IFF( <=> )\n";
    std::cout << "Enter a logical expression: ";
    std::getline( std::cin, input );
    std::ofstream fileHandle{"out.txt", std::ios::out | std::ios::trunc };
    try
    {
        Logic::PostfixGenerator gen( Util::ToWString( input ) );
        auto logicModel = Logic::LogicModel{ gen.postfixForm() };
        Logic::TableModel dispModel{ logicModel };
        dispModel.AddOutputChannel( stdout );
        dispModel.AddOutputChannels({ fileHandle } );
        dispModel.SetDisplayModeAscending(false );
        dispModel.Display();
        fileHandle.close();
    }
    catch( const std::exception& e )
    {
        std::cerr << "ERROR " << e.what() <<'\n';
        fileHandle.close();
    }
    catch( ... )
    {
        std::cerr << "UNKNOWN ERROR\n";
        fileHandle.close();
    }
}