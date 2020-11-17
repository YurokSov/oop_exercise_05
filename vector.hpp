//
// Created by yurok on 13.11.2020.
//

#ifndef OOP_EXERCISE_05_VECTOR_HPP
#define OOP_EXERCISE_05_VECTOR_HPP

#include <exception>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <memory>

namespace lab {

    template <typename T>
    class Vector {
    public:
        using index_type = size_t;
        using value_type = T;

        Vector() : _length(0), _capacity(BASE_CAP) {
            _length = 0;
            _data = std::make_unique<value_type[]>(_capacity);
        };
        //explicit Vector(index_type size = 0, const value_type& value = value_type());
        Vector(const Vector&) = delete;
        Vector(Vector&&) = delete;
        ~Vector() = default;

        class VectorIterator {
        public:
            using difference_type = Vector::index_type;
            using value_type = Vector::value_type;
            using reference = Vector::value_type&;
            using pointer = Vector::value_type*;
            using iterator_category = std::forward_iterator_tag;

            VectorIterator(Vector& vector, difference_type index) : _vector(vector), _index(index) {}

            VectorIterator& operator++() {
                ++_index;
                return *this;
            }

            reference operator*() {
                return _vector[_index];
            }

            pointer operator->() {
                return &_vector[_index];
            }

            bool operator!=(const VectorIterator& other) {
                if (_index != other._index) {
                    return true;
                }
                if (&_vector != &(other._vector)) {
                    return true;
                }
                return false;
            }

            bool operator==(const VectorIterator& other) {
                if (_index != other._index) {
                    return false;
                }
                if (&_vector != &(other._vector)) {
                    return false;
                }
                return true;
            }

        private:
            Vector& _vector;
            difference_type _index;

            friend class Vector;
        };

        index_type& length() {
            return _length;
        };
        [[nodiscard]] const index_type& length() const {
            return _length;
        };

        value_type& at(index_type index) {
            if (index >= 0 && index < _capacity) {
                return _data[index];
            }
            else {
                throw std::out_of_range("Out of bounds");
            }
        };
        const value_type& at(index_type index) const {
            if (index >= 0 && index < _capacity) {
                return _data[index];
            }
            else {
                throw std::out_of_range("Out of bounds");
            }
        };

        value_type& operator[](index_type index) noexcept {
            return _data[index];
        };
        const value_type& operator[](index_type index) const noexcept {
            return _data[index];
        };

        index_type size() {
            return _length;
        }

        VectorIterator begin() {
            return VectorIterator(*this, 0);
        }

        VectorIterator end() {
            return VectorIterator(*this, size());
        }

        void reserve(index_type capacity) {
            if (capacity < _capacity) {
                return;
            }
            else {
                while (capacity < _capacity) {
                    index_type new_capacity = capacity * Vector::GROWTH_RATE;
                    if (new_capacity > capacity) {
                        capacity = new_capacity;
                    }
                    else {
                        break;
                    }
                }
                auto old_data = std::move(_data);
                try {
                    _data = std::make_unique<value_type[]>(capacity);
                }
                catch (std::bad_alloc& ex) {
                    std::cerr << "ERROR: " << ex.what() << std::endl;
                }
                for (index_type index = 0; index < _capacity; ++index) {
                    _data[index] = old_data[index];
                }
                _capacity = capacity;
            }
        }

        void push_back(value_type& value) {
            if (_length >= _capacity) {
                reserve(_capacity + 1);
            }
            if (_length >= _capacity) {
                throw std::logic_error("max capacity reached");
            }
            _data[_length] = value;
            ++_length;
        }

        void pop_back(value_type& value) {
            if (_length > 0) {
                --_length;
                value = _data[_length];
            }
            else {
                throw std::logic_error("pop on empty vector");
            }
        }

        VectorIterator erase(VectorIterator iter) {
            if (iter._index < 0 || iter._index > size()) {
                throw std::logic_error("bad insert");
            }
            index_type i = 0;
            for (; i < _length; ++i) {
                if (i == iter._index) {
                    break;
                }
            }
            for (; i < _length; ++i) {
                _data[i] = _data[i + 1];
            }
            --_length;
            return iter;
        }

        VectorIterator insert(VectorIterator iter, const value_type& value) {
            if (iter._index < 0 || iter._index > size()) {
                throw std::logic_error("bad insert");
            }
            if (_length == _capacity) {
                reserve(_length + 1);
            }
            for (index_type i = _length; i > 0; --i) {
                _data[i] = _data[i - 1];
                if (i == iter._index) {
                    _data[i] = value;
                    break;
                }
            }
            if (iter._index == 0) {
                _data[0] = value;
            }
            ++_length;
            return iter;
        }

    private:
        static index_type const BASE_CAP = 10;
        static index_type const GROWTH_RATE = 2;
        std::unique_ptr<value_type[]> _data;
        index_type _length;
        index_type _capacity;
    };

}

#endif //OOP_EXERCISE_05_VECTOR_HPP