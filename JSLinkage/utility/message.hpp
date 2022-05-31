//
// Created by zener-diode on 11/01/2022.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

class Message
{
public:
    std::string name,
        data;

    Message( std::string name, std::string data)
        : name( std::move( name)), data( std::move( data))
    {
    }
};

#endif //MESSAGE_HPP
