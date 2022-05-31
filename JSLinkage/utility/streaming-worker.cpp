#include <node/node.h>
#include <nan.h>
#include <iostream>
#include "message.hpp"
#include "pcqueue.hpp"
#include "streaming-worker.hpp"

StreamingWorker::StreamingWorker( Nan::Callback *progress, Nan::Callback *callback, Nan::Callback *error)
: Nan::AsyncProgressWorker( callback), progress_( progress), error_( error)
{
}

void StreamingWorker::close()
{
    closed_ = true;
}

void StreamingWorker::HandleOKCallback()
{
    drainQueue();
    Nan::Call( *callback, 0, nullptr);
}

void StreamingWorker::HandleErrorCallback()
{
    v8::Local<v8::Value> argv[] = {
        v8::Exception::Error( Nan::New<v8::String>( ErrorMessage()).ToLocalChecked())
    };

    Nan::Call( *error_, 1, argv);
}

void StreamingWorker::HandleProgressCallback( const char *, size_t)
{
    drainQueue();
}


bool StreamingWorker::isClosed()
{
    return closed_;
}

void StreamingWorker::send( const Nan::AsyncProgressWorker::ExecutionProgress& progress, const Message &message)
{
    pipe_[ 0].write( message);
    progress.Send(
            reinterpret_cast<const char *>( &pipe_[ 0]),
            sizeof( pipe_[ 0])
    );
}

void StreamingWorker::drainQueue()
{
    Nan::HandleScope scope;

    std::deque<Message> contents;
    pipe_[ 0].readAll(contents);
    for( Message &message : contents)
    {
        v8::Local<v8::Value> argv[] = {
                Nan::New<v8::String>( message.name.c_str()).ToLocalChecked(),
                Nan::New<v8::String>( message.data.c_str()).ToLocalChecked(),
        };

        Nan::Call( *progress_, 2, argv);
    }
}

