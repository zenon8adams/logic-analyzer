#ifndef STREAMING_WORKER_HPP
#define STREAMING_WORKER_HPP

#include "message.hpp"
#include "pcqueue.hpp"

class StreamingWorker : public Nan::AsyncProgressWorker
{
public:
    StreamingWorker( Nan::Callback *progress, Nan::Callback *callback, Nan::Callback *error);

	void close();

    void HandleOKCallback() override;

    void HandleErrorCallback() override;

    void HandleProgressCallback( const char *data, size_t size) override;

    PCQueue<Message> pipe_[ 2];
protected:
	bool isClosed();

    void send( const Nan::AsyncProgressWorker::ExecutionProgress& progress, const Message &message);

    void drainQueue();

    Nan::Callback *progress_,
                  *error_;

    bool closed_ = false;
};

#endif //STREAMING_WORKER_HPP
