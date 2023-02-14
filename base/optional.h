#ifndef BASE_OPTIONAL_H_
#define BASE_OPTIONAL_H_

#include <utility>

template <class T>
struct Optional {
    Optional() : valid_(false), value_(T()) {}

    Optional(const T &valueIn) : valid_(true), value_(valueIn) {}

    Optional(const Optional &other) : valid_(other.valid_), value_(other.value_) {}

    Optional &operator=(const Optional &other) {
        this->valid_ = other.valid_;
        this->value_ = other.value_;
        return *this;
    }

    Optional &operator=(const T &value) {
        value_ = value;
        valid_ = true;
        return *this;
    }

    Optional &operator=(T &&value) {
        value_ = std::move(value);
        valid_ = true;
        return *this;
    }

    void reset() { valid_ = false; }
    T &&release()
    {
        valid_ = false;
        return std::move(value_);
    }

    static Optional Invalid() { return Optional(); }

    bool valid() const { return valid_; }
    T &value() { return value_; }
    const T &value() const { return value_; }
    const T &ValueOr(const T &default_value) const { return valid_ ? value_ : default_value; }

    bool operator==(const Optional &other) const
    {
        return ((valid_ == other.valid_) && (!valid_ || (value_ == other.value_)));
    }

    bool operator!=(const Optional &other) const { return !(*this == other); }

    bool operator==(const T &value) const { return valid_ && (value_ == value); }

    bool operator!=(const T &value) const { return !(*this == value); }

  private:
    bool valid_;
    T value_;
};

#endif  // COMMON_OPTIONAL_H_
