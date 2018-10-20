#include "big_integer.h"

typedef int32_t word_t;
typedef int64_t dword_t;
typedef uint32_t uword_t;
typedef uint64_t udword_t;

const uword_t UWORD_MAX = UINT32_MAX;
const size_t CELL_SIZE = 32;

std::pair<std::vector<char>, word_t> decimal_to_binary(std::string u) {
    std::string *word = &u;
    std::vector<char> result;
    word_t sign = ((*word)[0] == '-') ? 1 : 0;
    if (sign == 1) u = u.substr(1, u.length() - 1);
    std::string next_step;
    while (!(*word).empty()) {
        result.push_back(static_cast<word_t>((*word)[(*word).size() - 1] - '0') % 2 == 1);
        word_t carry = 0;
        next_step = "";
        for (size_t i = 0; i < word->length(); i++) {
            char c = (*word)[i];
            auto new_c = static_cast<char>('0' + ((c + 10 * carry - '0') / 2));
            if ((i != 0) || (new_c != '0')) next_step += new_c;
            carry = static_cast<word_t>(c - '0') % 2;
        }
        *word = next_step;
    }
    return std::make_pair(result, sign);
}
std::string binary_to_decimal(std::pair<std::vector<char>, word_t> p)   {
    std::string word = "0";
    std::vector<char> &v = p.first;
    std::string next_step;
    for (size_t i = 0; i < v.size(); i++) {
        bool carry = v[v.size() - i - 1];
        next_step = "";
        for (size_t j = 0; j < word.length(); j++) {
            char c = word[word.length() - j - 1];
            word_t tcp = (c - '0') * 2 + carry;
            carry = tcp > 9;
            auto new_c = static_cast<char>(tcp % 10 + '0');
            next_step += new_c;
        }
        if (carry) next_step += "1";
        std::reverse(next_step.begin(), next_step.end());
        word = next_step;
    }
    if (p.second == 1) word = "-" + word;
    return word;
}

bool big_integer::check_for_zero() const {
    return (number.size() == 1) && (number[0] == 0);
}
bool big_integer::check_for_negative() const {
    return number.back() == -1;
}
bool big_integer::check_for_positive() const {
    return !number.empty() && number.back() == 0;
}

void big_integer::resize(size_t size) {
    while (number.size() < size) {
        int bob = check_for_negative() ? -1 : 0;
        number.push_back(bob);
    }
}
void big_integer::optimize() {
    if (number.empty()) return;
    if (number.back() > 0) {
        number.push_back(0);
    } else if (number.back() < -1) {
        number.push_back(-1);
    }
    while ((number.size() > 1) && (number.back() == number[number.size() - 2])) {
        number.pop_back();
    }
}

big_integer::big_integer() {
    number.push_back(0);
}
big_integer::big_integer(word_t value) {
    number.push_back(value);
    optimize();
}
big_integer::big_integer(std::string const &word) {
    auto p = decimal_to_binary(word);
    auto v = &(p.first);
    uword_t t = 1;
    number.push_back(0);
    for (size_t i = 0; i < (*v).size(); i++) {
        t = (i % CELL_SIZE == 0) ? 1 : t * 2;
        number.back() += static_cast<word_t>((*v)[i] * t);
        if (i % CELL_SIZE == CELL_SIZE - 1) number.push_back(0);
    }
    optimize();
    if (p.second == 1) *this = -(*this);
    optimize();
}
big_integer::big_integer(big_integer const &original) {
    number = original.number;
}
big_integer::~big_integer() {
    number.clear();
}

big_integer big_integer::operator+() const {
    return *this;
}
big_integer big_integer::operator-() const {
    big_integer my_bi = ~(*this);
    my_bi++;
    return my_bi;
}
big_integer big_integer::operator~() const {
    big_integer my_bi = big_integer();
    my_bi.resize(number.size());
    for (size_t i = 0; i < number.size(); i++) {
        my_bi.number[i] = ~static_cast<uword_t>(number[i]);
    }
    return my_bi;
}

big_integer &big_integer::operator++() {
    *this += 1;
    return *this;
}
big_integer &big_integer::operator--() {
    *this -= 1;
    return *this;
}
const big_integer big_integer::operator++(int) {
    big_integer my_bi = *this;
    ++*this;
    return my_bi;
}
const big_integer big_integer::operator--(int) {
    big_integer my_bi = *this;
    --*this;
    return my_bi;
}

big_integer &big_integer::operator=(big_integer const &arg) {
    this->number = arg.number;
    return *this;
}
big_integer &big_integer::operator+=(big_integer const &arg) {
    this->add_ms(arg, 0, 0);
    return *this;

}
big_integer &big_integer::operator-=(big_integer const &arg) {
    this->subtract_ms(arg, 0, 0);
    return *this;
}
big_integer &big_integer::operator*=(big_integer const &arg) {
    big_integer first = abs(*this), second = abs(arg);
    word_t sign_result = sign(*this) * sign(arg);
    number.clear();
    number.push_back(0);
    for (size_t j = 0; j < second.number.size(); j++) {
        udword_t carry = 0;
        big_integer my_bi;
        my_bi.number.resize(first.number.size() + 1, 0);
        for (size_t i = 0; i < first.number.size(); i++){
            auto product = static_cast<udword_t>(static_cast<uword_t>(first.number[i]));
            product *= static_cast<udword_t>(static_cast<uword_t>(second.number[j]));
            product += carry;
            my_bi.number[i] = static_cast<word_t>(product);
            carry = product >> CELL_SIZE;
        }
        my_bi.number.push_back(static_cast<word_t>(carry));
        my_bi.optimize();
        this->add_ms(my_bi, j, -static_cast<word_t>(j));
    }
    if (sign_result == -1) *this = -(*this);
    return *this;
}
big_integer &big_integer::operator/=(big_integer const &arg) {
    if (check_for_zero()) return *this;
    big_integer first = abs(*this), second = abs(arg);
    bool result_negative = this->check_for_negative() ^ arg.check_for_negative();

    *this = div_ms(first, second);
    if (result_negative) *this = -(*this);
    return *this;
}
big_integer &big_integer::operator%=(big_integer const &arg) {
    big_integer d = *this / arg;
    *this = *this - arg * d;
    return *this;
}
big_integer &big_integer::operator&=(big_integer const &arg) {
    resize(arg.number.size());
    for (size_t i = 0; i < arg.number.size(); i++){
        number[i] &= arg.number[i];
    }
    optimize();
    return *this;
}
big_integer &big_integer::operator|=(big_integer const &arg) {
    resize(arg.number.size());
    for (size_t i = 0; i < arg.number.size(); i++){
        number[i] |= arg.number[i];
    }
    optimize();
    return *this;
}
big_integer &big_integer::operator^=(big_integer const &arg) {
    resize(arg.number.size());
    for (size_t i = 0; i < arg.number.size(); i++){
        number[i] ^= arg.number[i];
    }
    optimize();
    return *this;
}
big_integer &big_integer::operator<<=(word_t arg) {
    if (arg < 0) return (*this >>= std::abs(arg));
    size_t big = arg / CELL_SIZE, small = arg % CELL_SIZE;
    word_t carry = 0;

    for (size_t i = 0; i < number.size(); i++) {
        word_t w;
        if (small == 0) {
            w = 0;
        } else {
            w = static_cast<uword_t>(number[i]) >> (CELL_SIZE - small);
        }
        auto x = static_cast<udword_t>(static_cast<dword_t>(number[i]));
        x <<= small;
        number[i] = static_cast<word_t>(static_cast<dword_t>(x));
        if (small != 0) number[i] += carry;
        carry = w;
    }
    optimize();

    big_integer my_bi;
    if (big > 0) {
        my_bi.number.resize(big, 0);
    } else {
        my_bi.number.clear();
    }
    for (size_t i = 0; i < number.size(); i++) {
        my_bi.number.push_back(number[i]);
    }
    *this = my_bi;
    return *this;
}
big_integer &big_integer::operator>>=(word_t arg) {
    if (arg < 0) return (*this <<= std::abs(arg));
    size_t big = arg / CELL_SIZE, small = arg % CELL_SIZE;

    big_integer my_bi;
    my_bi.number.clear();
    for (size_t i = big; i < number.size(); i++) {
        my_bi.number.push_back(number[i]);
    }
    if (my_bi.number.empty()) my_bi.number.push_back(number.back());
    *this = my_bi;

    for (size_t i = 0; i < number.size() - 1; i++) {
        word_t w = number[i + 1] % (1u << small);
        if (small == 0) {
            w = 0;
        } else {
            w <<= CELL_SIZE - small;
        }
        number[i] = static_cast<word_t>(static_cast<uword_t>(number[i]) >> small);
        number[i] += w;
    }
    optimize();
    return *this;
}

big_integer operator+(big_integer first, big_integer const &second) {
    return first += second;
}
big_integer operator-(big_integer first, big_integer const &second) {
    return first -= second;
}
big_integer operator*(big_integer first, big_integer const &second) {
    return first *= second;
}
big_integer operator/(big_integer first, big_integer const &second) {
    return first /= second;
}
big_integer operator%(big_integer first, big_integer const &second) {
    return first %= second;
}
big_integer operator&(big_integer first, big_integer const &second) {
    return first &= second;
}
big_integer operator|(big_integer first, big_integer const &second) {
    return first |= second;
}
big_integer operator^(big_integer first, big_integer const &second) {
    return first ^= second;
}
big_integer operator>>(big_integer first, word_t second) {
    return first >>= second;
}
big_integer operator<<(big_integer first, word_t second) {
    return first <<= second;
}

bool operator==(big_integer const &first, big_integer const &second) {
    big_integer my_bi = first - second;
    return my_bi.check_for_zero();
}
bool operator!=(big_integer const &first, big_integer const &second) {
    big_integer my_bi = first - second;
    return !my_bi.check_for_zero();
}
bool operator<(big_integer const &first, big_integer const &second) {
    big_integer my_bi = first - second;
    return my_bi.check_for_negative();
}
bool operator>(big_integer const &first, big_integer const &second) {
    big_integer my_bi = second - first;
    return my_bi.check_for_negative();
}
bool operator<=(big_integer const &first, big_integer const &second) {
    big_integer my_bi = second - first;
    return !my_bi.check_for_negative();
}
bool operator>=(big_integer const &first, big_integer const &second) {
    big_integer my_bi = first - second;
    return !my_bi.check_for_negative();
}

std::istream& operator>>(std::istream& in, big_integer &some_bi){
    std::string word;
    in >> word;
    some_bi = big_integer(word);
    return in;
}
std::ostream& operator<<(std::ostream& out, big_integer some_bi) {
    return out << to_string(some_bi);
}

std::string to_string(big_integer const &arg) {
    word_t x = 0;
    if (arg.check_for_negative()) x++;
    big_integer const minus_some_bi(-arg);
    big_integer const *my_bi = (x == 0) ? &arg : &minus_some_bi;
    std::vector<char> v;
    for (size_t i = 0; i < my_bi->number.size(); i++) {
        word_t u = my_bi->number[i];
        for (size_t j = 0; j < CELL_SIZE; j++) {
            v.push_back(u % 2 != 0);
            u >>= 1;
        }
    }
    return binary_to_decimal(std::make_pair(v, x));
}
word_t sign(big_integer const &arg) {
    if (arg.check_for_negative()) {
        return -1;
    } else if (arg.check_for_positive()) {
        return 1;
    } else {
        return 0;
    }
}
big_integer abs(big_integer const &arg) {
    if (arg.check_for_negative()) {
        return -arg;
    } else {
        return +arg;
    }
}
big_integer pow(big_integer const &first, word_t second) {
    if (second == 0) return 1;
    if (first == 1) return 1;
    if (first == -1) return 1 - big_integer(2 * (second % 2));
    if (second % 2 == 1) {
        big_integer my_bi = pow(first, second - 1);
        my_bi *= first;
        return my_bi;
    } else {
        big_integer my_bi = pow(first, second >> 1);
        my_bi *= my_bi;
        return my_bi;
    }
}

std::string big_integer::to_binary() const {
    std::string result;
    for (size_t i = 0; i < number.size(); i++) {
        result += std::bitset<CELL_SIZE>(number[number.size() - i - 1]).to_string();
        if (i != number.size() - 1) result += ".";
    }
    return result;
}
big_integer big_integer::from_binary(std::vector<char> &v) {
    big_integer res;
    res.number.clear();
    word_t x = 0;
    for (size_t i = 0; i < v.size(); i++) {
        word_t y = 0;
        if (v[i] != 0) y++;
        y <<= (i % CELL_SIZE);
        x += y;
        if (i % CELL_SIZE == CELL_SIZE - 1 || i == v.size() - 1) {
            res.number.push_back(x);
            x = 0;
        }
    }
    res.number.push_back(0);
    res.optimize();
    return res;
}

big_integer big_integer::div_ms(big_integer &first, big_integer &second) {
    if (first.check_for_zero() || first < second) return 0;
    if (second == 1) return first;

    big_integer my_bi = second, neg = 0;
    size_t h = (first.number.size() - second.number.size()) * CELL_SIZE, k = 0;
    my_bi <<= h;
    if (first < my_bi) {
        my_bi >>= CELL_SIZE;
        h -= CELL_SIZE;
    }
    while (first >= my_bi) {
        my_bi <<= 1;
        h++;
    }
    if (first < my_bi) {
        my_bi >>= 1;
        h--;
    }
    std::array<big_integer, CELL_SIZE> shifts;
    std::array<word_t, CELL_SIZE> clean{};
    clean.fill(0);
    h++;
    for (size_t i = 0; i < std::min(h, CELL_SIZE); i++) {
        shifts[i] = my_bi >> i;
    }
    big_integer *p = &my_bi;
    std::vector<char> vec_res;
    while (h > 0) {
        h--;
        if (first.number.size() < p->number.size() - clean[k]) {
            vec_res.push_back(0);
        } else {
            first.subtract_ms(*p, h / CELL_SIZE, clean[k]);
            if (first.check_for_positive()) {
                vec_res.push_back(1);
            } else {
                first.add_ms(*p, h / CELL_SIZE, clean[k]);
                vec_res.push_back(0);
            }
        }
        clean[k]++;
        k = (k + 1) % CELL_SIZE;
        p = &shifts[k];
    }
    std::reverse(vec_res.begin(), vec_res.end());
    return from_binary(vec_res);
}
big_integer big_integer::add_ms(big_integer const &arg, size_t x, word_t y) {
    size_t s = arg.number.size() - y;
    resize(s);
    bool carry = false;
    for (size_t i = x; i < number.size(); i++){
        word_t addend = (i < s) ? arg.number[i + y] : 0;
        auto sum = static_cast<word_t>(static_cast<dword_t>(number[i]) + addend + carry);
        carry = static_cast<uword_t>(sum) < static_cast<uword_t>(addend);
        carry |= static_cast<uword_t>(sum) < static_cast<uword_t>(number[i]);
        carry &= addend != static_cast<word_t>(UWORD_MAX) || i != s - 1;
        number[i] = sum;
    }
    optimize();
    return *this;
}
big_integer big_integer::subtract_ms(big_integer const &arg, size_t x, word_t y) {
    size_t s = arg.number.size() - y;
    resize(s);
    bool carry = false;
    for (size_t i = x; i < number.size(); i++){
        word_t subtrahend = (i < s) ? arg.number[i + y] : 0;
        auto difference = static_cast<word_t>(static_cast<dword_t>(number[i]) - subtrahend - carry);
        carry = static_cast<uword_t>(number[i]) < static_cast<uword_t>(subtrahend);
        carry |= static_cast<uword_t>(number[i]) < static_cast<uword_t>(difference);
        carry &= subtrahend != static_cast<word_t>(UWORD_MAX) || i != s - 1;
        number[i] = difference;
    }
    optimize();
    return *this;
}