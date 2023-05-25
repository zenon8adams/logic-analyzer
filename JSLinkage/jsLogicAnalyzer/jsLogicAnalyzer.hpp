#ifndef LOGICANALYZER_JSLOGICANALYZER_HPP
#define LOGICANALYZER_JSLOGICANALYZER_HPP

#include "../utility/streaming-worker.hpp"
#include <nan.h>
#include <node/node.h>

class JsLogicAnalyzer : public StreamingWorker
{
public:
  JsLogicAnalyzer(Nan::Callback *progress, Nan::Callback *complete,
                  Nan::Callback *error, const v8::Local<v8::Object>& options);

  void
  Execute(const Nan::AsyncProgressWorker::ExecutionProgress& progress) override;

  template <typename Value>
  std::string serialize(const std::string& label,
                        const std::vector<Value>& results);

private:
  std::string rule;
  bool is_ascending_order;
};

#endif // LOGICANALYZER_JSLOGICANALYZER_HPP
