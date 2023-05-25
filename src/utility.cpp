#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "utility.hpp"

std::wstring Util::WSRep(wchar_t c) { return {1, c}; }

std::string Util::Decode(unsigned val)
{
  std::string sRep;
  if (val < 0x80)
  {
	sRep += static_cast<char>(val);
  }
  else if (val < 0x800)
  {
	sRep += static_cast<char>(0xC0U | (val >> 6U));
	sRep += static_cast<char>(0x80U | (val & 0x3FU));
  }
  else if (val < 0x10000)
  {
	sRep += static_cast<char>(0xE0U | (val >> 12U));
	sRep += static_cast<char>(0x80U | ((val >> 6U) & 0x3FU));
	sRep += static_cast<char>(0x80U | (val & 0x3FU));
  }
  else if (val < 0x200000)
  {
	sRep += static_cast<char>(0xF0U | (val >> 18U));
	sRep += static_cast<char>(0x80U | ((val >> 12U) & 0x3FU));
	sRep += static_cast<char>(0x80U | ((val >> 6U) & 0x3FU));
	sRep += static_cast<char>(0x80U | (val & 0x3FU));
  }
  else if (val < 0x4000000)
  {
	sRep += static_cast<char>(0xF8U | (val >> 24U));
	sRep += static_cast<char>(0x80U | ((val >> 18U) & 0x3FU));
	sRep += static_cast<char>(0x80U | ((val >> 12U) & 0x3FU));
	sRep += static_cast<char>(0x80U | ((val >> 6U) & 0x3FU));
	sRep += static_cast<char>(0x80U | (val & 0x3FU));
  }
  else if (val < 0x80000000)
  {
	sRep += static_cast<char>(0xFCU | (val >> 30U));
	sRep += static_cast<char>(0xF8U | ((val >> 24U) & 0x3FU));
	sRep += static_cast<char>(0x80U | ((val >> 18U) & 0x3FU));
	sRep += static_cast<char>(0x80U | ((val >> 12U) & 0x3FU));
	sRep += static_cast<char>(0x80U | ((val >> 6U) & 0x3FU));
	sRep += static_cast<char>(0x80U | (val & 0x3FU));
  }
  return sRep;
}

size_t Util::nByte(unsigned char c)
{
  for (size_t n = 2; n <= 6; ++n)
  {
	bool val = (c >> n) ^ 0xFFU >> n;
	if (!val)
	  return 8 - n;
  }
  return 1;
}

MAYBE_UNUSED int Util::Join(std::string str, size_t idx, int count)
{
  if (count == 1)
	return str[idx];

  int copy = count;
  while (copy >= 1)
	str[idx + static_cast<size_t>(copy--)] &= 0x3FU;

  unsigned char shift = 0xFF >> (count + 1);
  str[idx] = static_cast<char>(static_cast<unsigned char>(str[idx]) & shift);
  auto i =
      static_cast<size_t>((count) << 2U) + (static_cast<size_t>(count) << 1);
  int value = 0;
  while (count >= 0)
  {
	value += str[idx++] << i;
	--count;
	i -= 6;
  }
  return value;
}

DLL_EXPORT std::wstring Util::ToWString(std::string str)
{
  std::wstring wsRep;
  for (size_t i = 0uL; i < str.size();)
  {
	size_t byteCount = nByte(static_cast<unsigned char>(str[i]));
	wsRep +=
	    static_cast<wchar_t>(Join(str, i, static_cast<int>(byteCount) - 1));
	i += byteCount;
  }
  return wsRep;
}

std::string Util::ToU8String(std::wstring_view ws)
{
  std::string s;
  for (auto elm : ws)
	s += Decode(static_cast<unsigned int>(elm));
  return s;
}

extern "C"
{

  DLL_EXPORT wchar_t *toWString(const char *str)
  {
	std::wstring wsRep = Util::ToWString(str);
	auto *pstr =
	    static_cast<wchar_t *>(calloc(sizeof(wchar_t) + 1, wsRep.size()));
	wcscpy(pstr, wsRep.data());
	return pstr;
  }

  MAYBE_UNUSED DLL_EXPORT void freeArray(wchar_t **array, size_t size)
  {
	for (size_t i = 0; i < size; ++i)
	  free(array[i]);
	free(array);
  }

  MAYBE_UNUSED DLL_EXPORT void freeString(wchar_t *c) { free(c); }

  FILE *tty0() { return stdout; }
}

#endif
