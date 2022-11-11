#ifndef HOMEWORK_GS_VECTOR_H
#define HOMEWORK_GS_VECTOR_H

#pragma once

#include "include.h"

namespace GS {
    template<typename T>
    struct vector {
        vector() noexcept;
        vector(vector const &other);

        vector &operator=(vector const &other);

        template<typename InputIterator>
        vector(InputIterator first, InputIterator last);

        ~vector();

        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last);

        using value_type = T;
        typedef T *iterator;
        typedef const T *const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        T &operator[](size_t index);
        T const &operator[](size_t index) const;

        T &front() noexcept;
        T const &front() const noexcept;
        T &back() noexcept;
        T const &back() const noexcept;

        void push_back(T const &value);
        void pop_back();

        iterator begin();
        const_iterator begin() const noexcept;
        iterator end();
        const_iterator end() const noexcept;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const noexcept;
        reverse_iterator rend();
        const_reverse_iterator rend() const noexcept;

        T *data();
        T const *data() const noexcept;

        [[nodiscard]] bool empty() const noexcept;

        [[nodiscard]] size_t size() const noexcept;
        [[nodiscard]] size_t capacity() const noexcept;

        void clear() noexcept;
        void reserve(size_t n);
        void resize(size_t n);
        void shrink_to_fit();

        iterator insert(const_iterator pos, T const &val);
        iterator insert( T const &val, const_iterator pos_1, const_iterator pos_2);

        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);

        template<typename V>
        friend void swap(vector<V> &a, vector<V> &b);
        template<typename V>
        friend bool operator==(vector<V> const &a, vector<V> const &b) noexcept;
        template<typename V>
        friend bool operator!=(vector<V> const &a, vector<V> const &b) noexcept;
        template<typename V>
        friend bool operator<(vector<V> const &a, vector<V> const &b) noexcept;
        template<typename V>
        friend bool operator>(vector<V> const &a, vector<V> const &b) noexcept;
        template<typename V>
        friend bool operator<=(vector<V> const &a, vector<V> const &b) noexcept;
        template<typename V>
        friend bool operator>=(vector<V> const &a, vector<V> const &b) noexcept;

        T& at( size_t n ) {
            if (n >= size()) throw std::out_of_range("Invalid index");
            return reinterpret_cast<char &>(buf[n]);
        }

        T& at( size_t n ) const {
            if (n >= size()) throw std::out_of_range("Invalid index");
            return reinterpret_cast<char &>(buf[n]);
        }

    private:
        static size_t const SIZE_I = 0;
        static size_t const CAP_I = 1;
        static size_t const REF_I = 2;
        bool single;
        union {
            T val;
            size_t *buf;
        };

        size_t get_buf_size(size_t n) { return sizeof(size_t) * 3 + sizeof(T) * n; }

        T const *buf_start(size_t *buf) const { return reinterpret_cast<T *>(buf + 3); }
        T *buf_start(size_t *buf) { return reinterpret_cast<T *>(buf + 3); }
        T const *buf_start() const { return buf_start(buf); }
        T *buf_start() { return buf_start(buf); }

        bool has_buf() { return !single && !empty(); }

        void make_unique() {
            if (!has_buf() || buf[REF_I] == 1) {
                return;
            }
            size_t *temp = new_buf_with_size(capacity());
            --buf[REF_I];
            buf = temp;
        }

        size_t *new_buf_with_size(size_t buf_size) {
            auto *new_buf = static_cast<size_t *>(operator new(get_buf_size(buf_size)));
            size_t new_size = std::min(buf_size, size());
            new_buf[SIZE_I] = new_size;
            new_buf[CAP_I] = buf_size;
            new_buf[REF_I] = 1;
            size_t i = 0;
            try {
                if (single) {
                    new(buf_start(new_buf)) T(val);
                } else {
                    for (; i < new_size; ++i) {
                        new(buf_start(new_buf) + i) T(*(buf_start() + i));
                    }
                }
                return new_buf;
            } catch (...) {
                for (size_t j = 0; j < i; ++j) {
                    (*(buf_start(new_buf) + j)).~T();
                }
                operator delete(new_buf);
                throw;
            }
        }

    };
}

#endif
