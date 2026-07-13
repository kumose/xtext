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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <xtext/parser.h>

#include <string_view>

#include <doctest.h>

TEST_CASE("ParseInteger Basic") {
    int32_t v32;
    CHECK(xtext::parse_integer<int32_t>("12345", &v32));
    CHECK(v32 == 12345);

    CHECK(xtext::parse_integer<int32_t>("-12345", &v32));
    CHECK(v32 == -12345);

    int8_t v8;
    CHECK_FALSE(xtext::parse_integer<int8_t>("200", &v8));
}

TEST_CASE("ParseInteger Prefix") {
    std::string_view remained;
    int32_t val;

    CHECK(xtext::parse_integer_prefix<int32_t>("100KB", &val, &remained));
    CHECK(val == 100);
    CHECK(remained == "KB");

    CHECK_FALSE(xtext::parse_integer<int32_t>("100KB", &val));
}

TEST_CASE("ParseHexInteger") {
    uint32_t val;

    CHECK(xtext::parse_hex_integer<uint32_t>("0x1A", &val));
    CHECK(val == 0x1A);

    CHECK(xtext::parse_hex_integer<uint32_t>("FF", &val));
    CHECK(val == 255);
}

TEST_CASE("ParseBinAndOct") {
    uint32_t val;

    CHECK(xtext::parse_bin_integer<uint32_t>("0b1010", &val));
    CHECK(val == 10);

    CHECK(xtext::parse_oct_integer<uint32_t>("0755", &val));
    CHECK(val == 0755);
}

TEST_CASE("ParseFloat Basic") {
    double vd;
    CHECK(xtext::parse_float<double>("3.14159", &vd));
    CHECK(vd == doctest::Approx(3.14159));

    float vf;
    CHECK(xtext::parse_float<float>("1.23e-4", &vf));
    CHECK(vf == doctest::Approx(0.000123f));
}

TEST_CASE("ParseBool") {
    bool val;

    CHECK(xtext::parse_bool("true", &val));
    CHECK(val == true);
    CHECK(xtext::parse_bool("YES", &val));
    CHECK(val == true);
    CHECK(xtext::parse_bool("1", &val));
    CHECK(val == true);
    CHECK(xtext::parse_bool("t", &val));
    CHECK(val == true);

    CHECK(xtext::parse_bool("false", &val));
    CHECK(val == false);
    CHECK(xtext::parse_bool("no", &val));
    CHECK(val == false);
    CHECK(xtext::parse_bool("0", &val));
    CHECK(val == false);
    CHECK(xtext::parse_bool("f", &val));
    CHECK(val == false);

    CHECK_FALSE(xtext::parse_bool("maybe", &val));
}

TEST_CASE("ParseUnit") {
    int64_t vi;
    CHECK(xtext::parse_integer_unit<int64_t>("10K", &vi));
    CHECK(vi == 10000);
    CHECK(xtext::parse_integer_unit<int64_t>("2MB", &vi));
    CHECK(vi == 2000000);

    double vd;
    CHECK(xtext::parse_float_unit<double>("1.5G", &vd));
    CHECK(vd == doctest::Approx(1.5e9));

    int64_t vh;
    CHECK(xtext::parse_hex_integer_unit<int64_t>("0x10K", &vh));
    CHECK(vh == 16000);
}
