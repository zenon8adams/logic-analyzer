#include "postfix-gen.hpp"
#include "utility.hpp"

Logic::PostfixGenerator::PostfixGenerator(const std::wstring& input)
    : strm(input)
{
  generate();
}

void Logic::PostfixGenerator::consumeWs()
{
  while (strm.get() == SPACE)
	;
  strm.unget();
}

std::wstring Logic::PostfixGenerator::getVariable()
{
  std::wstring str;
  for (auto c = strm.get(); std::isalnum(static_cast<int>(c)); c = strm.get())
	str += Util::WSRep(static_cast<wchar_t>(c));
  strm.unget();
  return str;
}

bool Logic::PostfixGenerator::isOperator(wchar_t c)
{
  return Util::orAll(c, AND, XOR, OR, NOT, IF, IFF, ODIM);
}

bool Logic::PostfixGenerator::mayBeSpecialOp(wchar_t c)
{
  return replOps.find(c) != replOps.cend();
}

void Logic::PostfixGenerator::generate()
{
  consumeWs();
  for (auto c = static_cast<wchar_t>(strm.get()); c != WEOF;
       c = static_cast<wchar_t>(strm.get()))
  {
	if (c == CDIM)
	{
	  if (unwindStackUntil([](int ch) { return ch != ODIM; }))
		oprStack.pop_back();
	}
	else if (isOperator(c) || mayBeSpecialOp(c))
	{

	  if (mayBeSpecialOp(c))
	  {
		auto op = replOps.find(c);
		if (op == replOps.cend())
		  throw std::logic_error(": Invalid symbol '" +
		                         std::string(1, static_cast<char>(c)) + '\'');

		auto remaining = op->second.first.size() - 1;
		std::wstring toComp(1, c);
		toComp.reserve(remaining);
		if (toComp.capacity() > 1)
		{
		  toComp.resize(remaining + 1);
		  strm.readsome(&toComp[1], static_cast<std::ptrdiff_t>(remaining));
		}
		if (Util::ToU8String(toComp) != op->second.first)
		  throw Util::error::syntax_error(": Invalid operator '" +
		                                  Util::ToU8String(toComp) + '\'');
		c = op->second.second;
	  }
	  if (!oprStack.empty() && c != ODIM && !postfix.empty())
		reOrder(c);
	  else
		oprStack.push_back(c);
	}
	else if (std::isalnum(c))
	{
	  auto str = Util::WSRep(c);
	  auto remaining = getVariable();
	  if (!remaining.empty())
		str.append(remaining);
	  postfix.push_back(str);
	}
	else if (std::isspace(c))
	  consumeWs();
	else
	{
	  throw Util::error::syntax_error(Util::Join(
	      "Unknown token `", Util::ToU8String(Util::WSRep(c)), "`"));
	}
  }
  unwindStackUntil([](int ch) { return ch != WEOF; });
}

void Logic::PostfixGenerator::reOrder(int c)
{
  while (!oprStack.empty() && oprStack.back() != ODIM &&
         hasHigherPrecedence(oprStack.back(), c))
  {
	postfix.push_back(Util::WSRep(oprStack.back()));
	oprStack.pop_back();
  }
  oprStack.push_back(c);
}

bool Logic::PostfixGenerator::hasHigherPrecedence(int l, int r)
{
  return precedenceTable.find(l)->second >= precedenceTable.find(r)->second;
}

bool Logic::PostfixGenerator::unwindStackUntil(
    const std::function<bool(int)>& op)
{
  while (!oprStack.empty() && op(oprStack.back()))
  {
	postfix.emplace_back(1, oprStack.back());
	oprStack.pop_back();
  }
  return !oprStack.empty();
}

const std::unordered_map<int, int> Logic::PostfixGenerator::precedenceTable = {
    {NOT, 5}, {AND, 4}, {OR, 3}, {XOR, 3}, {IF, 2}, {IFF, 1}};

const std::unordered_map<int, std::pair<std::string, Operator>>
    Logic::PostfixGenerator::replOps = {
        {'~', {"~", NOT}}, {'!', {"!", NOT}},  {'<', {"<=>", IFF}},
        {'=', {"=>", IF}}, {'-', {"->", IF}},  {'^', {"^", XOR}},
        {'@', {"@", XOR}}, {'&', {"&", AND}},  {'/', {"/\\", AND}},
        {'|', {"|", OR}},  {'\\', {"\\/", OR}}};

extern "C"
{

  MAYBE_UNUSED Logic::PostfixGenerator *
  makePostfixGenerator(const wchar_t *input)
  {
	return new Logic::PostfixGenerator(input);
  }

  wchar_t **postfixForm(Logic::PostfixGenerator *gen, size_t *len)
  {
	std::vector<std::wstring> pfx = gen->postfixForm();
	auto **pfx_array =
	    static_cast<wchar_t **>(calloc(sizeof(wchar_t *) + 1, pfx.size()));

	if (pfx_array)
	{
	  for (size_t i = 0; i < pfx.size(); ++i)
	  {
		pfx_array[i] = static_cast<wchar_t *>(malloc(pfx[i].size()));
		wcscpy(pfx_array[i], pfx[i].data());
	  }
	  *len = pfx.size();
	  return pfx_array;
	}
	return nullptr;
  }

  void destroyPostfixObject(Logic::PostfixGenerator *obj) { delete obj; }
}