//
// Created by zener-diode on 11/01/2022.
//

#ifndef PCQUEUE_HPP
#define PCQUEUE_HPP


#include <deque>
#include <mutex>
#include <condition_variable>
template <typename Data>
class PCQueue
{
public:
    void write( Data data)
    {
        while( true)
        {
            std::unique_lock<std::mutex> locker( mu_);
            buffer_.push_back( data);
            locker.unlock();
            cond_.notify_all();

            return;
        }
    }

    Data __attribute__((unused)) read()
    {
        while( true)
        {
            std::unique_lock<std::mutex> locker( mu_);
            cond_.wait( locker, [ this]() {
                return buffer_.size() > 0;
            });
            Data back = buffer_.front();
            buffer_.pop_front();
            locker.unlock();
            cond_.notify_all();

            return back;
        }
    }

    void readAll( std::deque<Data> &target)
    {
        std::unique_lock<std::mutex> locker( mu_);
        std::copy( buffer_.cbegin(), buffer_.cend(),
                   std::back_inserter( target)
        );
        buffer_.clear();
        locker.unlock();
    }

    PCQueue() = default;

private:
    std::mutex mu_;
    std::condition_variable cond_;
    std::deque<Data> buffer_;
};


#endif //PCQUEUE_HPP
