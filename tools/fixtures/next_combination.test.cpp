#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <string>
#include <vector>

// NEXT_COMBINATION_IMPLEMENTATION

struct Tagged {
    int value;
    int identity;
    friend bool operator<(const Tagged& a, const Tagged& b) { return a.value < b.value; }
};

void require(bool ok, const char* message) {
    if (!ok) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

// テスト用の別方式:
// 添字の部分集合を再帰的に列挙し、値として同じものを set で除く。
void subsets(const std::vector<int>& a, std::size_t pos, int left,
             std::vector<int>& chosen,
             std::set<std::vector<int>>& result) {
    if (left == 0) {
        result.insert(chosen);
        return;
    }
    if (pos == a.size()) {
        return;
    }

    subsets(a, pos + 1, left, chosen, result);
    chosen.push_back(a[pos]);
    subsets(a, pos + 1, left - 1, chosen, result);
    chosen.pop_back();
}

void check(const std::vector<int>& input, int k) {
    std::set<std::vector<int>> expected_set;
    std::vector<int> chosen;
    subsets(input, 0, k, chosen, expected_set);

    std::vector<std::vector<int>> expected(
        expected_set.begin(), expected_set.end()
    );
    std::vector<std::vector<int>> actual;
    auto values = input;

    do {
        require(std::is_sorted(values.begin(), values.begin() + k),
                "selected order");
        require(std::is_sorted(values.begin() + k, values.end()),
                "unselected order");

        auto all = values;
        std::sort(all.begin(), all.end());
        require(all == input, "multiset preserved");

        actual.emplace_back(values.begin(), values.begin() + k);
        require(actual.size() <= expected.size(), "too many combinations");
    } while (next_combination(values.begin(), values.end(), k));

    require(values == input, "reset after false");
    require(actual == expected, "combinations differ from oracle");
}

// 0, 1, 2 からなる昇順の入力をすべて生成する。
void enumerate_inputs(std::vector<int>& a, int remaining, int minimum) {
    if (remaining == 0) {
        for (int k = 0; k <= static_cast<int>(a.size()); ++k) {
            check(a, k);
        }
        return;
    }

    for (int x = minimum; x < 3; ++x) {
        a.push_back(x);
        enumerate_inputs(a, remaining - 1, x);
        a.pop_back();
    }
}

int main() {
    for (int n = 0; n <= 8; ++n) {
        std::vector<int> distinct;
        for (int i = 0; i < n; ++i) {
            distinct.push_back(i);
        }
        for (int k = 0; k <= n; ++k) {
            check(distinct, k);
        }

        std::vector<int> input;
        enumerate_inputs(input, n, 0);
    }

    std::vector<int> v{1, 1, 2};
    require(next_combination(v.begin(), v.end(), 2),
            "duplicate example first");
    require((v == std::vector<int>{1, 2, 1}),
            "duplicate example next");
    require(!next_combination(v.begin(), v.end(), 2),
            "duplicate example end");
    require((v == std::vector<int>{1, 1, 2}),
            "duplicate example reset");

    std::string s = "aabc";
    const std::vector<std::string> expected{"aa", "ab", "ac", "bc"};
    std::vector<std::string> got;

    do {
        got.push_back(s.substr(0, 2));
    } while (next_combination(s.begin(), s.end(), 2));

    require(got == expected && s == "aabc", "string");

    // The implementation must move, rather than copy, stored values.
    std::vector<std::unique_ptr<int>> pointers;
    for (int x : {1, 2, 3}) pointers.push_back(std::make_unique<int>(x));
    // unique_ptr's ordering is by address, so arrange it by that ordering.
    std::sort(pointers.begin(), pointers.end());
    auto* first = pointers[0].get();
    auto* second = pointers[1].get();
    auto* third = pointers[2].get();
    require(next_combination(pointers.begin(), pointers.end(), 2), "move-only next");
    require(pointers[0].get() == first && pointers[1].get() == third, "move-only values");
    require(next_combination(pointers.begin(), pointers.end(), 2), "move-only last");
    require(!next_combination(pointers.begin(), pointers.end(), 2), "move-only end");
    require(pointers[0].get() == first && pointers[1].get() == second &&
            pointers[2].get() == third, "move-only reset");

    std::vector<Tagged> tagged{{1, 0}, {1, 1}, {2, 2}, {2, 3}};
    std::vector<std::vector<int>> tagged_combinations;
    do {
        tagged_combinations.push_back({tagged[0].value, tagged[1].value});
        require(std::is_sorted(tagged.begin(), tagged.begin() + 2), "tagged selected");
        require(std::is_sorted(tagged.begin() + 2, tagged.end()), "tagged unselected");
    } while (next_combination(tagged.begin(), tagged.end(), 2));
    require((tagged_combinations == std::vector<std::vector<int>>{{1, 1}, {1, 2}, {2, 2}}),
            "comparison-equivalent values");
    require(std::is_sorted(tagged.begin(), tagged.end()), "tagged reset");
    std::set<int> identities;
    for (const auto& item : tagged) identities.insert(item.identity);
    require(identities == std::set<int>({0, 1, 2, 3}), "tagged identity preservation");

    std::vector<bool> bits{false, false, true};
    require(next_combination(bits.begin(), bits.end(), 2), "proxy iterator next");
    require(bits == std::vector<bool>({false, true, false}), "proxy iterator values");
    require(!next_combination(bits.begin(), bits.end(), 2), "proxy iterator end");
    require(bits == std::vector<bool>({false, false, true}), "proxy iterator reset");

    std::cout << "All tests passed\n";
}
