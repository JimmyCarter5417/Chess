#ifndef MEMO_H
#define MEMO_H

#include <vector>
#include <memory>
#include <assert.h>

using std::vector;
using std::shared_ptr;

template <typename T>
class Memo
{
public:
    void clear()
    {
        memo_.clear();
    }

    void push(shared_ptr<T> ptr)
    {
        memo_.push_back(ptr);
    }

    void pop()
    {
        return memo_.pop_back();
    }

    shared_ptr<T> top()
    {
        return memo_.back();
    }

    shared_ptr<T> bottom()
    {
        return memo_.front();
    }

    shared_ptr<T> at(unsigned int index)
    {
        return memo_.at(index);
    }

    unsigned int size()
    {
        return memo_.size();
    }

    bool empty()
    {
        return memo_.empty();
    }

private:
    vector<shared_ptr<T>> memo_;
};

#endif // MEMO_H
