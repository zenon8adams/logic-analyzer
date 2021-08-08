#include <string>
#include "binary-counter.hpp"

Logic::BinaryCounter::BinaryCounter( std::string ini )
    : bitString(std::move( ini ))
{
}

Logic::BinaryCounter::BinaryCounter( std::size_t size, char bit )
    :bitString(std::string( size, bit ) )
{
}

Logic::BinaryCounter& Logic::BinaryCounter::Inc()
{
    if( bitString.empty() )
                bitString += '0';

    size_t end = bitString.size() - 1;
    int val = bitString[end] - '0',
            inc = val + 1,
            carry = inc / 2;
    bitString[end] = inc % 2 + '0';

    if( !carry )
        return *this;

    for(; --end != size_t(-1); )
    {
        val = bitString[end] - '0' + carry;
        bitString[end] = val % 2 + '0';
        carry = val / 2;
        if( !carry )
            break;
    }
    if( carry )
        bitString.insert( bitString.begin(), carry + '0' );

    return *this;
}

Logic::BinaryCounter& Logic::BinaryCounter::Dec()
{
    if( bitString.empty() )
                bitString = "0";
    bitString = BinSub(bitString,
                       LeftZeroPad( "1",bitString.size()-1 ) );
    return *this;
}

std::string Logic::BinaryCounter::value() const
{
    return bitString;
}

std::string Logic::BinaryCounter::BinSub( std::string lterm, std::string rterm )
{
    std::string result;
    char value;
    int borrow = 0;
    for( size_t n = lterm.size()-1; n != size_t (-1); --n )
    {
        if( lterm[n] - borrow >= rterm[n] )
        {
            value = (char)(lterm[n] - rterm[n] - borrow + '0');
            borrow = 0;
        }else
        {
            borrow = borrow != 0 ? borrow : 2;
            value = (char)(lterm[n] - rterm[n] + borrow + '0');
            borrow -= borrow == 2;
        }
        result.insert( 0, 1, value );
    }
    return result;
}

std::string Logic::BinaryCounter::LeftZeroPad( const std::string& term, std::size_t quantity )
{
    return std::string( quantity, '0' ) + term;
}

// JS interface.

extern "C" {

    Logic::BinaryCounter *createCounter_s( const char *ini )
    {
        return new Logic::BinaryCounter( ini);
    }

    Logic::BinaryCounter *createCounter( std::size_t size, char bit )
    {
        return new Logic::BinaryCounter( std::string( size, bit));
    }

    Logic::BinaryCounter *Inc( Logic::BinaryCounter *counter )
    {
        return &counter->Inc();
    }

    Logic::BinaryCounter *Dec( Logic::BinaryCounter *counter )
    {
        return &counter->Dec();
    }

    void destroyCounterObject( Logic::BinaryCounter *obj )
    {
        delete obj;
    }
}