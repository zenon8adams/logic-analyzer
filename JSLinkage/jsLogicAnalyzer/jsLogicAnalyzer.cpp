
#include "jsLogicAnalyzer.hpp"
#include "../utility/builder/json-builder.hpp"
#include "../utility/streaming-worker-adapter.hpp"
#include "config.hpp"
#include <postfix-gen.hpp>
#include <utility.hpp>

JsLogicAnalyzer::JsLogicAnalyzer(Nan::Callback *progress,
                                 Nan::Callback *complete, Nan::Callback *error,
                                 const v8::Local<v8::Object>& options)
    : StreamingWorker(progress, complete, error), is_ascending_order(true)
{
  if (options->IsUndefined() || !options->IsObject())
  {
	SetErrorMessage("Invalid option type!");
	exit(EXIT_FAILURE);
  }

  v8::Local<v8::Value> input =
      Nan::Get(options, Nan::New<v8::String>("input").ToLocalChecked())
          .ToLocalChecked();
  if (input->IsString())
  {
	Nan::Utf8String imm_str(input);
	rule = *imm_str;
  }
  else
  {
	SetErrorMessage("Unable to parse input!");
	exit(EXIT_FAILURE);
  }

  auto imm_ascending =
      Nan::Get(options, Nan::New<v8::String>("ascending").ToLocalChecked())
          .ToLocalChecked();
  if (!imm_ascending.IsEmpty())
	is_ascending_order = Nan::To<bool>(imm_ascending).FromJust();
}

void JsLogicAnalyzer::Execute(
    const Nan::AsyncProgressWorker::ExecutionProgress& progress)
{
  try
  {
	Logic::PostfixGenerator gen(Util::ToWString(rule));
	auto logicModel = Logic::LogicModel{gen.postfixForm()};
	Logic::TableModel dispModel{logicModel};
	auto headers = dispModel.GetHeaders();
	send(progress, Message("packet", serialize("headers", headers)));

	auto model = logicModel.Model();
	auto width = logicModel.NumberOfVariable();
	auto counter = createCounter(width, is_ascending_order ? '0' : '1');
	std::vector<char> rows(model.size());

	for (size_t i = 1U << width; i > 0; --i)
	{
	  auto *value = Value(counter);

	  for (size_t j = 0u; j < width; ++j)
		Assign(model[j]->ToWString().c_str(), value[j] - '0');
	  for (size_t k = 0; k < model.size(); ++k)
		rows[k] = model[k]->Eval() ? 'T' : 'F';
	  send(progress, Message("packet", serialize("rows", rows)));
	  is_ascending_order ? Inc(counter) : Dec(counter);

	  free(static_cast<char *>(value));
	}
  }
  catch (const std::exception& e)
  {
	SetErrorMessage(e.what());
  }
  catch (...)
  {
	SetErrorMessage("Unknown error occurred!");
  }
}

template <typename Value>
std::string
JsLogicAnalyzer::serialize(MAYBE_UNUSED const std::string& label,
                           MAYBE_UNUSED const std::vector<Value>& results)
{
  JsonBuilder builder;
  builder.startList(label);
  for (auto&& each : results)
  {
	if constexpr (std::is_same_v<char, Value>)
	  builder.add(std::string(1, each));
	else if constexpr (std::is_same_v<Logic::TableModel::HeaderType, Value>)
	  builder.add(each.header);
  }

  return builder.endList().final();
}

DLL_EXPORT StreamingWorker *createWorker(Nan::Callback *progress,
                                         Nan::Callback *complete,
                                         Nan::Callback *error,
                                         const v8::Local<v8::Object>& options)
{
  return new JsLogicAnalyzer(progress, complete, error, options);
}

NODE_MODULE(logic_analyzer, StreamingWorkerAdapter::init)
