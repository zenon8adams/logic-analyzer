#include "streaming-worker.hpp"
#include <iostream>

StreamingWorker::StreamingWorker(Nan::Callback *progress,
                                 Nan::Callback *callback, Nan::Callback *error)
    : Nan::AsyncProgressWorker(callback), progress_(progress), error_(error)
{
}

void StreamingWorker::close() { closed_ = true; }

void StreamingWorker::HandleOKCallback()
{
  drainQueue();
  Nan::Call(*callback, 0, nullptr);
}

void StreamingWorker::HandleErrorCallback() { HandleError(); }

void StreamingWorker::HandleProgressCallback(const char *, size_t)
{
  drainQueue();
}

[[maybe_unused]] bool StreamingWorker::isClosed() const { return closed_; }

void StreamingWorker::send(
    const Nan::AsyncProgressWorker::ExecutionProgress& progress,
    const Message& message)
{
  pipe_[0].write(message);
  progress.Send(reinterpret_cast<const char *>(&pipe_[0]), sizeof(pipe_[0]));
}

void StreamingWorker::drainQueue()
{
  if (progress_)
  {
	Nan::HandleScope scope;

	std::deque<Message> contents;
	pipe_[0].readAll(contents);
	for (Message& message : contents)
	{
	  v8::Local<v8::Value> argv[] = {
	      Nan::New<v8::String>(message.name.c_str()).ToLocalChecked(),
	      Nan::New<v8::String>(message.data.c_str()).ToLocalChecked(),
	  };

	  Nan::Call(*progress_, 2, argv);
	}
  }
}

void StreamingWorker::HandleError()
{
  if (error_)
  {
	v8::Local<v8::Value> argv = v8::Exception::Error(
	    Nan::New<v8::String>(ErrorMessage()).ToLocalChecked());
	error_->Call(1, &argv, async_resource);
  }
}
