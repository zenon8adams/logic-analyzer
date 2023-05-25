#ifndef LOGICAL_EXPRESSION_HPP
#define LOGICAL_EXPRESSION_HPP

#include "config.hpp"
#include <memory>
#include <string>

#define DECLARE_BINARY(objName)                                                \
  class DLL_LOCAL objName : public BinaryOp                                    \
  {                                                                            \
  public:                                                                      \
	explicit DLL_LOCAL objName(std::shared_ptr<LogicalExp> l,                  \
	                           std::shared_ptr<LogicalExp> r);                 \
	DLL_LOCAL ~objName() override = default;                                   \
	DLL_LOCAL bool Eval() override;                                            \
	DLL_LOCAL std::wstring Name() const override;                              \
  };

enum Operator
{
  AND = 0x2227,
  XOR = 0x2295,
  OR = 0x2228,
  NOT = 0x00AC,
  IF = 0x21D2,
  IFF = 0x21D4,
  ODIM = 0x0028,
  CDIM = 0x0029,
  SPACE = 0x0020
};

namespace Logic
{
class Context;

class DLL_LOCAL LogicalExp
{
public:
  DLL_LOCAL LogicalExp() = default;
  virtual ~LogicalExp() = default;
  virtual bool Eval() = 0;
  [[nodiscard]] virtual std::wstring Name() const = 0;
  [[nodiscard]] DLL_LOCAL virtual std::wstring ToWString() const { return {}; }
};

class DLL_LOCAL VariableExp : public LogicalExp
{
public:
  DLL_LOCAL explicit VariableExp(std::wstring vn);
  DLL_LOCAL ~VariableExp() override = default;
  DLL_LOCAL bool Eval() override;
  [[nodiscard]] DLL_LOCAL std::wstring Name() const override { return varName; }
  [[nodiscard]] DLL_LOCAL std::wstring ToWString() const override
  {
	return varName;
  }

private:
  std::wstring varName;
};

class DLL_LOCAL BinaryOp : public LogicalExp
{
public:
  DLL_LOCAL BinaryOp(std::shared_ptr<LogicalExp> l,
                     std::shared_ptr<LogicalExp> r);
  DLL_LOCAL ~BinaryOp() override = default;
  [[nodiscard]] DLL_LOCAL std::wstring ToWString() const override;

protected:
  std::shared_ptr<LogicalExp> left, right;
};

class DLL_LOCAL NotExp : public LogicalExp
{
public:
  explicit DLL_LOCAL NotExp(std::shared_ptr<LogicalExp> e);
  DLL_LOCAL ~NotExp() override = default;
  DLL_LOCAL bool Eval() override;
  [[nodiscard]] DLL_LOCAL std::wstring Name() const override;
  [[nodiscard]] DLL_LOCAL std::wstring ToWString() const override;

private:
  std::shared_ptr<LogicalExp> expr;
};

DECLARE_BINARY(OrExp)
DECLARE_BINARY(AndExp)
DECLARE_BINARY(XorExp)
DECLARE_BINARY(IFExp)
DECLARE_BINARY(IFFExp)

} // namespace Logic

#endif