#ifndef TABLE_MODEL_HPP
#define TABLE_MODEL_HPP

#include "config.hpp"
#include "logic-model.hpp"
#include <sstream>

namespace Logic
{
class DLL_EXPORT TableModel
{
  struct HeaderData;

public:
  using HeaderType MAYBE_UNUSED = HeaderData;

  explicit TableModel(LogicModel ml);
  ~TableModel();
  void AddOutputChannels(
      std::initializer_list<std::reference_wrapper<std::ostream>> os);
  void AddOutputChannel(FILE *channel);
  void SetDisplayModeAscending(bool asc);
  std::vector<HeaderData> GetHeaders() const;
  void Display() const;

private:
  struct HeaderData
  {
	std::string header;
	std::size_t width;
	static constexpr std::size_t padding = 4, gap = 6;

	HeaderData(std::string h, size_t w);
  };

  static size_t SpacingOf(const std::wstring& text);

  std::string Boxed(char value, size_t index) const;

  void DisplayHeading() const;

  void BuildHeaders() const;

  void Flush() const;

  LogicModel logicModel;

  mutable std::vector<HeaderData> headers;
  mutable std::vector<std::reference_wrapper<std::ostream>> strms;
  mutable std::stringstream _default;
  // For compatibility with C streams.
  mutable FILE **channel = nullptr;
  size_t n_channel;

  bool ascending{};
};

} // namespace Logic

extern "C"
{
  DLL_EXPORT Logic::TableModel *makeTableModel(Logic::LogicModel *logic_model);
  DLL_EXPORT void addOutputChannel(Logic::TableModel *table_model, FILE *fp);
  MAYBE_UNUSED DLL_EXPORT void
  setDisplayModeAscending(Logic::TableModel *table_model, bool asc);
  DLL_EXPORT void display(Logic::TableModel *table_model);
  DLL_EXPORT void destroyTableModelObject(Logic::TableModel *table_model);
}

#endif