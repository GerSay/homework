#ifndef HOMEWORK_GS_STRING_H
#define HOMEWORK_GS_STRING_H

#pragma once

#include <vector>
#include "include.h"

namespace GS {
    class String {
        vector<char> m_chars;
        static String format(std::stringstream& is) {
            String s;
            is >> s;
            return s;
        }

        template<class... Args, class T>
        static String format(std::stringstream& is, T&& t, Args&&... things) { return is << t, format(is, std::forward<Args>(things)...); }

    public:
        using Iterator             = vector<char>::iterator;
        using ConstIterator        = vector<char>::const_iterator;
        using ReverseIterator      = vector<char>::reverse_iterator;
        using ConstReverseIterator = vector<char>::const_reverse_iterator;

        String();
        String(std::nullptr_t);
        String(const char* cstr);
        String(ConstIterator from, ConstIterator to);
        String(const String&) = default;
        String(String&&)      = default;

        explicit String(char c);

        String& operator=(const String&) = default;

        operator std::string() const;

        Iterator begin();
        ConstIterator begin() const;
        Iterator end();
        ConstIterator end() const;
        char& at(std::size_t i);
        char at(std::size_t i) const;

        bool empty() const noexcept;

        std::size_t size() const noexcept;
        std::size_t length() const noexcept;

        std::unique_ptr<char[]> to_c_string() const;
        std::string to_std_string() const;

        void clear() noexcept;

        void insert(ConstIterator iter, char c);
        void insert(ConstIterator iter, const String& s);
        void insert(ConstIterator iter, ConstIterator begin, ConstIterator end);

        void erase(ConstIterator iter);
        void erase(ConstIterator from, ConstIterator to);
        void erase(ConstIterator iter, std::size_t n);

        String substring(ConstIterator from, ConstIterator to) const;
        String substring(ConstIterator start, std::size_t n) const;

        Iterator find(char c);
        ConstIterator find(char c) const;
        Iterator find(char c, Iterator start);
        ConstIterator find(char c, ConstIterator start) const;

        Iterator find(const String&);
        ConstIterator find(const String&) const;
        Iterator find(const String&, Iterator start);
        ConstIterator find(const String&, ConstIterator start) const;

        bool contains(const String&) const;
        bool startswith(const String&) const;
        bool endswith(const String&) const;

        bool equals(const String&) const;
        bool operator==(const String&) const;
        bool operator!=(const String&) const;

        String& operator+=(const String&);
        String operator+(const String&) const;

        void replace(char to_replace, char replace_with);
        void replace(const String& to_replace, const String& replace_with);
        void replace(const String& to_replace, const String& replace_with, std::size_t n);

        vector <String> split(char delim, std::size_t expected_splits = 2) const;
        vector <String> split(const String& delim, std::size_t expected_splits = 2) const;

        void reserve(std::size_t size);
        std::size_t capacity() const;
        void shrink_to_fit() noexcept;

        char* data() noexcept;
        const char* data() const noexcept;

        friend std::ostream& operator<<(std::ostream&, const String&);
        friend std::istream& operator>>(std::istream& is, String& s);

        template<class... Args>
        static String format(Args&&... things) {
            std::stringstream s;
            return format(s, std::forward<Args>(things)...);
        }

        struct Format {
            enum Align : bool {
                Left,
                Right
            };

            enum Base {
                Oct = 8,
                Dec = 10,
                Hex = 16,
            };

            int precision { 6 };
            Base base { Base::Dec };
            Align alignment { Align::Left };
            int width { 0 };
            char fill { ' ' };

            friend std::ostream& operator<<(std::ostream&, const Format&);
        };
    };


    class ConstString {
        const char*       m_buffer;
        const std::size_t m_size;

        static constexpr std::size_t length(const char* str) { return *str ? 1 + length(str + 1) : 0; }


    public:
        constexpr ConstString(std::nullptr_t) = delete;
        constexpr ConstString(const char*&& buffer)
                : m_buffer(buffer)
                , m_size(length(buffer)) {}

        constexpr auto size() const { return m_size; }
        constexpr auto length() const { return m_size; }

        constexpr operator const char*() const { return m_buffer; }

        template<class T>
        constexpr bool operator==(const T str) const { return std::strcmp(m_buffer, str) == 0; }
        bool operator==(const String& str) const { return m_size != str.size() || std::strncmp(m_buffer, str.data(), m_size) == 0; }
    };

}

#endif
