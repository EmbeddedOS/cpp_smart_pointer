#include <atomic>

namespace lava
{
    class __shared_count
    {
        using atomic_word = std::atomic_int;
        atomic_word _use_count;

    public:
        __shared_count() : _use_count{1} {}

        void inc() noexcept
        {
            this->_use_count++;
        }

        void dec() noexcept
        {
            if (this->_use_count > 0)
            {
                this->_use_count--;
            }
        }

        long get() const noexcept
        {
            return this->_use_count;
        }
    };

    template <typename T>
    class shared_ptr
    {
    private:
        T *_ptr;
        __shared_count *_counter;

    public:
        shared_ptr() : _ptr{nullptr}, _counter{nullptr} {}
        shared_ptr(T *ptr) : _ptr{ptr}
        {
            if (ptr != nullptr)
            {
                this->_counter = new __shared_count;
            }
        }

        shared_ptr(shared_ptr<T> &other) : _ptr{other._ptr},
                                           _counter{other._counter}
        {
            if (_ptr != nullptr)
            {
                this->_counter->inc();
            }
        }

        shared_ptr(shared_ptr<T> &&other) : _ptr{other._ptr},
                                            _counter{other._counter}

        {
            if (_ptr != nullptr)
            {
                other->_ptr = nullptr;
                other._counter = nullptr;
            }
        }

        shared_ptr<T> &operator=(const shared_ptr<T> &other) noexcept
        {
            reset();
            if (other._ptr != nullptr)
            {
                this->_ptr = other._ptr;
                this->_counter = other._counter;
                this->_counter->inc();
            }

            return *this;
        }

        shared_ptr<T> &operator=(const shared_ptr<T> &&other) noexcept
        {
            reset();
            if (other._ptr != nullptr)
            {
                this->_ptr = other._ptr;
                this->_counter = other._counter;
                other._ptr = nullptr;
                other._counter = nullptr;
            }

            return *this;
        }

        T &operator*() noexcept
        {
            return *this->_ptr;
        }

        T *operator->() noexcept
        {
            return this->_ptr;
        }

        explicit operator bool() const noexcept
        {
            return this->_ptr != nullptr;
        }

        void reset() noexcept
        {
            this->reset(nullptr);
        }

        void reset(T *ptr)
        {
            if (this->_ptr != nullptr)
            {
                this->_counter->dec();
                if (this->_counter->get() == 0)
                {
                    delete this->_ptr;
                    delete this->_counter;
                }

                this->_ptr = nullptr;
                this->_counter = nullptr;
            }

            if (ptr != nullptr)
            {
                this->_ptr = ptr;
                this->_counter = new __shared_count;
            }
        }

        void swap(shared_ptr<T> &other) noexcept
        {
            std::swap(this->_ptr, other._ptr);
            std::swap(this->_counter, other._counter);
        }

        long use_count() const noexcept
        {
            if (this->_ptr != nullptr)
            {
                return this->_counter->get();
            }

            return 0;
        }

        ~shared_ptr()
        {
            reset();
        }
    };
}
