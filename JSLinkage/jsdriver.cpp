#include <iostream>
#include <deque>
#include <fstream>
#include "utility.hpp"
#include <napi.h>

void run(const Napi::CallbackInfo& info)
{

	Napi::String input = info[ 0].ToString();
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

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports[ "run"] = Napi::Function::New(env, run);
	return exports;
}

NODE_API_MODULE( trial, Init)
