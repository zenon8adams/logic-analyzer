#include "streaming-worker-adapter.hpp"
#include "message.hpp"
#include "streaming-worker.hpp"

NAN_MODULE_INIT(StreamingWorkerAdapter::init)
{
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<v8::String>("StreamingWorker").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "close", closeInput);
  Nan::SetPrototypeMethod(tpl, "sendToAddon", sendToAddon);

  constructor().Reset(
      tpl->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());

  Nan::Set(target, Nan::New<v8::String>("StreamingWorker").ToLocalChecked(),
           tpl->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
}

NAN_METHOD(StreamingWorkerAdapter::closeInput)
{
  auto *obj = Nan::ObjectWrap::Unwrap<StreamingWorkerAdapter>(info.Holder());
  obj->worker_->close();
}

NAN_METHOD(StreamingWorkerAdapter::sendToAddon)
{
  Nan::Utf8String name(info[0]), data(info[1]);

  auto *obj = Nan::ObjectWrap::Unwrap<StreamingWorkerAdapter>(info.Holder());
  obj->worker_->pipe_[1].write(Message(*name, *data));
}

StreamingWorkerAdapter::StreamingWorkerAdapter(StreamingWorker *worker)
    : worker_(worker)
{
}

NAN_METHOD(StreamingWorkerAdapter::New)
{
  if (info.IsConstructCall())
  {
	auto *progress(new Nan::Callback(info[0].As<v8::Function>())),
	    *complete(new Nan::Callback(info[1].As<v8::Function>())),
	    *error(new Nan::Callback(info[2].As<v8::Function>()));

	v8::Local<v8::Object> options = info[3].As<v8::Object>();

	auto *obj = new StreamingWorkerAdapter(
	    createWorker(progress, complete, error, options));

	obj->Wrap(info.This());
	info.GetReturnValue().Set(info.This());

	Nan::AsyncQueueWorker(obj->worker_);
  }
  else
  {
	const int argc = 4;
	v8::Local<v8::Value> argv[] = {
	    info[0],
	    info[1],
	    info[2],
	    info[3],
	};

	v8::Local<v8::Function> cons = Nan::New(constructor());
	info.GetReturnValue().Set(
	    cons->NewInstance(Nan::GetCurrentContext(), argc, argv)
	        .ToLocalChecked());
  }
}

inline Nan::Persistent<v8::Function>& StreamingWorkerAdapter::constructor()
{
  static Nan::Persistent<v8::Function> _constructor;

  return _constructor;
}
