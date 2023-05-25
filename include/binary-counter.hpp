#ifndef BINARY_COUNTER_HPP
#define BINARY_COUNTER_HPP

#include "config.hpp"
#include <string>

namespace Logic
{
class DLL_LOCAL BinaryCounter
{
public:
  explicit DLL_LOCAL BinaryCounter(std::string ini);
  [[maybe_unused]] DLL_LOCAL BinaryCounter(std::size_t size, char bit);
  DLL_LOCAL BinaryCounter& Inc();
  DLL_LOCAL BinaryCounter& Dec();
  [[nodiscard]] DLL_LOCAL std::string value() const;

private:
  std::string bitString;

  static DLL_LOCAL std::string BinSub(std::string left_term,
                                      std::string right_term);
  static DLL_LOCAL std::string LeftZeroPad(const std::string& term,
                                           std::size_t quantity);
};
} // namespace Logic

extern "C"
{
  MAYBE_UNUSED DLL_EXPORT Logic::BinaryCounter *
  createCounter_s(const char *ini);
  DLL_EXPORT Logic::BinaryCounter *createCounter(std::size_t size, char bit);
  DLL_EXPORT Logic::BinaryCounter *Inc(Logic::BinaryCounter *counter);
  DLL_EXPORT Logic::BinaryCounter *Dec(Logic::BinaryCounter *counter);
  DLL_EXPORT char *Value(Logic::BinaryCounter *counter);
  MAYBE_UNUSED DLL_EXPORT void destroyCounterObject(Logic::BinaryCounter *obj);
}

#endif