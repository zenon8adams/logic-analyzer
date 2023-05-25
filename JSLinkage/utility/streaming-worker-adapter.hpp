#ifndef STREAMING_WORKER_WRAPPER_HPP
#define STREAMING_WORKER_WRAPPER_HPP

#include "streaming-worker.hpp"
#include <config.hpp>
#include <nan.h>

DLL_EXPORT StreamingWorker *createWorker(Nan::Callback *progress,
                                         Nan::Callback *complete,
                                         Nan::Callback *error,
                                         const v8::Local<v8::Object>& options);

class StreamingWorkerAdapter : public Nan::ObjectWrap
{
public:
  static NAN_MODULE_INIT(init);

  static NAN_METHOD(closeInput);

  static NAN_METHOD(sendToAddon);

private:
  explicit StreamingWorkerAdapter(StreamingWorker *worker);

  static NAN_METHOD(New);

  static inline Nan::Persistent<v8::Function>& constructor();

  StreamingWorker *worker_;
};

#endif // STREAMING_WORKER_WRAPPER_HPP
