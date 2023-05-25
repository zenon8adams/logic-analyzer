#ifndef STREAMING_WORKER_HPP
#define STREAMING_WORKER_HPP

#include "config.hpp"
#include "message.hpp"
#include "pcqueue.hpp"
#include <nan.h>
#include <node/node.h>

class StreamingWorker : public Nan::AsyncProgressWorker
{
public:
  StreamingWorker(Nan::Callback *progress, Nan::Callback *callback,
                  Nan::Callback *error);

  void close();

  void HandleOKCallback() override;

  void HandleErrorCallback() override;

  void HandleError();
  void HandleProgressCallback(const char *data, size_t size) override;

  ~StreamingWorker() override = default;

  PCQueue<Message> pipe_[2];

protected:
  MAYBE_UNUSED [[nodiscard]] bool isClosed() const;

  void send(const Nan::AsyncProgressWorker::ExecutionProgress& progress,
            const Message& message);

  void drainQueue();

  Nan::Callback *progress_{nullptr}, *error_{nullptr};

  bool closed_ = false;
};

#endif // STREAMING_WORKER_HPP
