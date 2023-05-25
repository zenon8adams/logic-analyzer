#include "model/table-model.hpp"
#include "binary-counter.hpp"
#include "context.hpp"
#include "utility.hpp"
#include <algorithm>
#include <iostream>

Logic::TableModel::TableModel(LogicModel ml)
    : logicModel(std::move(ml)), n_channel(0)
{
  BuildHeaders();
}

Logic::TableModel::~TableModel()
{
  if (channel)
	free(channel);
}

void Logic::TableModel::AddOutputChannels(
    std::initializer_list<std::reference_wrapper<std::ostream>> os)
{
  strms.insert(strms.end(), os.begin(), os.end());
}

void Logic::TableModel::AddOutputChannel(FILE *fp)
{
  if (channel == nullptr)
  {
	channel = static_cast<FILE **>(calloc(sizeof(FILE *), BUFSIZ));
	n_channel = 0;
  }
  channel[n_channel++] = fp;
}

void Logic::TableModel::SetDisplayModeAscending(bool asc) { ascending = asc; }

void Logic::TableModel::Display() const
{
  auto model = logicModel.Model();
  auto width = logicModel.NumberOfVariable();
  BinaryCounter counter{width, ascending ? '0' : '1'};

  DisplayHeading();

  for (size_t i = 1U << width; i > 0; --i)
  {
	auto value = counter.value();
	for (size_t j = 0u; j < width; ++j)
	  Logic::Context::Assign(model[j]->ToWString(), value[j] - '0');
	for (size_t k = 0; k < model.size(); ++k)
	{
	  char c = model[k]->Eval() ? 'T' : 'F';
	  _default << Boxed(c, k);
	  if (k != model.size() - 1)
		_default << std::string(HeaderData::gap, ' ');
	}
	_default << '\n';
	ascending ? counter.Inc() : counter.Dec();
  }
  Flush();
}

Logic::TableModel::HeaderData::HeaderData(std::string h, size_t w)
    : header(std::move(h)), width(w)
{
}

size_t Logic::TableModel::SpacingOf(const std::wstring& text)
{
  size_t spacing = 0;
  for (auto&& ch : text)
	spacing += ch > 0xFF ? 2 : 1;
  return spacing;
}

std::string Logic::TableModel::Boxed(char value, size_t index) const
{
  auto format{headers[index]};
  std::string sRep = std::string(HeaderData::padding + format.width - 1, ' ');
  sRep.insert(sRep.begin() + static_cast<std::ptrdiff_t>(sRep.size() / 2),
              value);
  return sRep;
}

void Logic::TableModel::DisplayHeading() const
{
  if (headers.empty())
	return;
  auto centered = [](auto&& heading)
  {
	return std::string(HeaderData::padding / 2, ' ') + heading +
	       std::string(HeaderData::padding / 2, ' ');
  };
  std::for_each(headers.cbegin(), --headers.cend(),
                [&, this](auto&& heading) {
	              _default << centered(heading.header)
	                       << std::string(HeaderData::gap, ' ');
                });
  _default << centered((--headers.cend())->header) << '\n';
}

void Logic::TableModel::BuildHeaders() const
{
  auto model = logicModel.Model();
  std::for_each(model.cbegin(), model.cend(),
                [this](auto&& heading)
                {
	              auto wsRep = heading->ToWString();
	              headers.emplace_back(Util::ToU8String(wsRep),
	                                   SpacingOf(wsRep));
                });
}

void Logic::TableModel::Flush() const
{
  std::string text = _default.str();
  std::for_each(strms.begin(), strms.end(),
                [&text](auto& strm) { strm.get() << text; });

  for (size_t i = 0; i < n_channel; ++i)
	fwrite(text.data(), sizeof(char), text.size(), channel[i]);

  _default.clear();
}

std::vector<Logic::TableModel::HeaderData> Logic::TableModel::GetHeaders() const
{
  if (headers.empty())
	BuildHeaders();

  return headers;
}

extern "C"
{

  Logic::TableModel *makeTableModel(Logic::LogicModel *logic_model)
  {
	return new Logic::TableModel(*logic_model);
  }

  void addOutputChannel(Logic::TableModel *table_model, FILE *fpp)
  {
	table_model->AddOutputChannel(fpp);
  }

  MAYBE_UNUSED void setDisplayModeAscending(Logic::TableModel *table_model,
                                            bool asc)
  {
	table_model->SetDisplayModeAscending(asc);
  }

  void display(Logic::TableModel *table_model) { table_model->Display(); }

  void destroyTableModelObject(Logic::TableModel *table_model)
  {
	delete table_model;
  }
}