#ifndef MYSTACK_H
#define MYSTACK_H

#include <vector>
#include <stack>

template <typename T>
class MyStack
{
public:
    void clear()
    {
        data_.clear();
    }

    void push(const T& t)
    {
        data_.push_back(t);
    }

    void pop()
    {
        return data_.pop_back();
    }

    T top() const
    {
        return data_.back();
    }

    T bottom() const
    {
        return data_.front();
    }

    T at(unsigned int index) const
    {
        return data_.at(index);
    }

    unsigned int size() const
    {
        return data_.size();
    }

    bool empty() const
    {
        return data_.empty();
    }

    void swap(MyStack& other)
    {
        data_.swap(other.data_);
    }

private:
    std::vector<T> data_;
};

#endif // MYSTACK_H
