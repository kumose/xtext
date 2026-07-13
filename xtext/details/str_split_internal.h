// Copyright (C) 2026 Kumo inc. and its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <xtext/macros.h>

namespace xtext::strings_internal {

    class ConvertibleToStringView {
    public:
        ConvertibleToStringView(const char *s) : value_(s) {}
        ConvertibleToStringView(char *s) : value_(s) {}
        ConvertibleToStringView(std::string_view s) : value_(s) {}
        ConvertibleToStringView(const std::string &s) : value_(s) {}

        ConvertibleToStringView(std::string &&) = delete;
        ConvertibleToStringView(const std::string &&) = delete;

        std::string_view value() const { return value_; }

    private:
        std::string_view value_;
    };

    template <typename Delimiter, typename Predicate, typename StringType>
    class Splitter;

    template <typename Delimiter, typename Predicate>
    class SplitIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::string_view;
        using difference_type = ptrdiff_t;
        using pointer = const std::string_view *;
        using reference = const std::string_view &;

        SplitIterator() = default;
        SplitIterator(const Splitter<Delimiter, Predicate, std::string_view> *splitter,
                      std::string_view text)
                : SplitIterator(splitter, text, Predicate()) {}

        SplitIterator(const Splitter<Delimiter, Predicate, std::string> *splitter,
                      std::string_view text)
                : SplitIterator(splitter, text, Predicate()) {}

        std::string_view operator*() const { return curr_; }
        SplitIterator &operator++() {
            DoSplit();
            return *this;
        }
        SplitIterator operator++(int) {
            auto old = *this;
            ++*this;
            return old;
        }
        bool operator==(const SplitIterator &other) const {
            return curr_.data() == other.curr_.data() &&
                   curr_.size() == other.curr_.size();
        }
        bool operator!=(const SplitIterator &other) const { return !(*this == other); }

    private:
        SplitIterator(const Splitter<Delimiter, Predicate, std::string_view> *splitter,
                      std::string_view text, Predicate predicate)
                : splitter_(splitter), predicate_(std::move(predicate)) {
            curr_ = std::string_view(text.data(), 0);
            pos_ = 0;
            DoSplit();
        }
        SplitIterator(const Splitter<Delimiter, Predicate, std::string> *splitter,
                      std::string_view text, Predicate predicate)
                : splitter_(splitter), predicate_(std::move(predicate)) {
            curr_ = std::string_view(text.data(), 0);
            pos_ = 0;
            DoSplit();
        }

        void DoSplit();

        const Splitter<Delimiter, Predicate, std::string_view> *splitter_ = nullptr;
        std::string_view curr_;
        size_t pos_ = 0;
        Predicate predicate_;
    };

    template <typename Delimiter, typename Predicate, typename StringType>
    class Splitter {
    public:
        using iterator = SplitIterator<Delimiter, Predicate>;
        using const_iterator = iterator;
        using value_type = typename iterator::value_type;

        Splitter(ConvertibleToStringView text, Delimiter d, Predicate p)
                : text_(std::move(text)), delimiter_(std::move(d)),
                  predicate_(std::move(p)) {}

        size_t EstimateLength() const { return text_.value().size() / 1 + 1; }

        iterator begin() const {
            return iterator(this, text_.value());
        }
        iterator end() const { return {}; }

    private:
        ConvertibleToStringView text_;
        Delimiter delimiter_;
        Predicate predicate_;

        friend class SplitIterator<Delimiter, Predicate>;
    };

    template <typename Delimiter, typename Predicate>
    void SplitIterator<Delimiter, Predicate>::DoSplit() {
        if (pos_ == std::string_view::npos) {
            curr_ = std::string_view();
            return;
        }
        auto text = splitter_->text_.value();
        auto d = splitter_->delimiter_;
        auto found = d.Find(text, pos_);
        if (found.data() == nullptr) {
            found = text.substr(text.size());
        }
        if (found.data() == text.data() + text.size()) {
            curr_ = text.substr(pos_);
            pos_ = std::string_view::npos;
        } else {
            curr_ = text.substr(pos_, found.data() - (text.data() + pos_));
            pos_ = found.data() - text.data() + found.size();
        }
        while (!predicate_(curr_)) {
            if (pos_ == std::string_view::npos) {
                curr_ = std::string_view();
                return;
            }
            text = splitter_->text_.value();
            d = splitter_->delimiter_;
            found = d.Find(text, pos_);
            if (found.data() == text.data() + text.size() ||
                found.data() == nullptr) {
                curr_ = text.substr(pos_);
                pos_ = std::string_view::npos;
            } else {
                curr_ = text.substr(pos_, found.data() - (text.data() + pos_));
                pos_ = found.data() - text.data() + found.size();
            }
        }
    }

    template <typename Delimiter, typename Predicate>
    SplitIterator<Delimiter, Predicate> begin(
            const Splitter<Delimiter, Predicate, std::string_view> &splitter) {
        return splitter.begin();
    }

    template <typename Delimiter, typename Predicate>
    SplitIterator<Delimiter, Predicate> end(
            const Splitter<Delimiter, Predicate, std::string_view> &splitter) {
        return splitter.end();
    }

    template <typename Delimiter, typename Predicate>
    SplitIterator<Delimiter, Predicate> begin(
            const Splitter<Delimiter, Predicate, std::string> &splitter) {
        return splitter.begin();
    }

    template <typename Delimiter, typename Predicate>
    SplitIterator<Delimiter, Predicate> end(
            const Splitter<Delimiter, Predicate, std::string> &splitter) {
        return splitter.end();
    }

}  // namespace xtext::strings_internal
