#include <BigNum.hpp>

#include <iterator>

namespace lab {

namespace {
/**
 * @brief Points to the max value BigNum array's cell can hold,
 *        same as basis in linear representation
 */
constexpr int NUM_BASE = 1000000000;

/**
 * @brief Points to number of digits in (NUM_BASE-1)
 */
constexpr char SECTION_DIGITS = 9;

}

BigNum::BigNum(std::string_view num_str) {
    for (int i = num_str.size() - 1; i >= 0; i -= SECTION_DIGITS) {
        std::string curr_section;
        if (i - SECTION_DIGITS + 1 < 0) {
            curr_section = num_str.substr(0, SECTION_DIGITS + (i - SECTION_DIGITS + 1));
        } else {
            curr_section = num_str.substr(i - SECTION_DIGITS + 1, SECTION_DIGITS);
        }
        _digits.push_back(std::stoi(curr_section));
    }
    while (!_digits.empty() && _digits.back() == 0) {
        _digits.pop_back();
    }
}

std::string to_string(const BigNum &num)
{
    std::string result;
    int leading_zeros = 0;

    for (int curr_pos = 0; curr_pos < num._digits.size(); ++curr_pos) {
        std::string temp = std::to_string(num._digits[curr_pos]);
        std::string offset;
        leading_zeros = 0;
        while (temp.size() + offset.size() != SECTION_DIGITS) {
            offset += '0';
            ++leading_zeros;
        }
        result = offset + temp + result;
    }

    result.erase(0, leading_zeros);
    return result;
}


const BigNum& BigNum::inf() {
    static const BigNum _inf = 99999999999999999999999999999999999999999999999999999999999999999999999999999999999999_bn;
    return _inf;
}

bool operator<(const BigNum &left, const BigNum &right) {
    if (left._digits.size() < right._digits.size()) return true;
    if (left._digits.size() > right._digits.size()) return false;
    for (int curr_pos = left._digits.size() - 1; curr_pos >= 0; --curr_pos) {
        if (left._digits[curr_pos] < right._digits[curr_pos]) return true;
        if (left._digits[curr_pos] > right._digits[curr_pos]) return false;
    }
    return false;
}

bool operator>(const BigNum &left, const BigNum &right) {
    return (right < left);
}

bool operator<=(const BigNum &left, const BigNum &right) {
    return !(right < left);
}

bool operator>=(const BigNum &left, const BigNum &right) {
    return !(left < right);
}

bool operator==(const BigNum &left, const BigNum &right) {
    return ((!(right < left)) && (!(left < right)));
}

bool operator!=(const BigNum &left, const BigNum &right) {
    return !(left == right);
}

BigNum operator+(const BigNum &left, const BigNum &right) {
    BigNum result;
    if (right._digits.size() > left._digits.size()) {
        result = right;
        int addition = 0;
        for (int curr_pos = 0; curr_pos < left._digits.size(); ++curr_pos) {
            result._digits[curr_pos] += left._digits[curr_pos] + addition;
            addition = result._digits[curr_pos] / NUM_BASE;
            result._digits[curr_pos] = result._digits[curr_pos] % NUM_BASE;
        }
    } else {
        result = left;
        int addition = 0;
        for (int curr_pos = 0; curr_pos < right._digits.size(); ++curr_pos) {
            long long temp = static_cast<long long>(result._digits[curr_pos])
                    + static_cast<long long>(right._digits[curr_pos]) + addition;
            result._digits[curr_pos] = temp % NUM_BASE;
            addition = temp / NUM_BASE;
            result._digits[curr_pos] = result._digits[curr_pos] % NUM_BASE;
        }
    }
    return result;
}

BigNum operator-(const BigNum &left, const BigNum &right) {
    BigNum result = left;
    for (int curr_pos = 0; curr_pos < right._digits.size(); ++curr_pos) {
        if (result._digits[curr_pos] < right._digits[curr_pos]) {
            result._digits[curr_pos] = result._digits[curr_pos] - right._digits[curr_pos] + NUM_BASE;
            result._digits[curr_pos + 1] -= 1;
        } else {
            result._digits[curr_pos] = result._digits[curr_pos] - right._digits[curr_pos];
        }
    }
    if (left._digits.size() > right._digits.size()) {
        if (result._digits[right._digits.size()] < 0) {
            result._digits[right._digits.size()] += NUM_BASE;
            result._digits[right._digits.size() + 1] -= 1;
        }
    }
    return result;
}

std::vector<char> toOneDigit(const BigNum &num) {
    std::vector<char> fnum;
    for (int curr_pos = 0; curr_pos < num._digits.size(); ++curr_pos) {
        int counter = num._digits[curr_pos];
        for (int i = 0; i < SECTION_DIGITS; ++i) {
            fnum.push_back(counter % 10);
            counter = counter / 10;
        }
    }
    while (!fnum.empty() && fnum.back() == 0) {
        fnum.pop_back();
    }
    return fnum;
}

BigNum from_string(std::string_view str)
{
    return BigNum(str);
}

BigNum toBigNum(std::vector<char> &num_digits) {
    std::string str_num;
    for (int i = num_digits.size() - 1; i >= 0; --i) {
        str_num += ('0' + num_digits[i]);
    }
    BigNum result(str_num);
    return result;
}

BigNum operator*(const BigNum &left, int right) {
    BigNum result = left;
    unsigned long long addition = 0;
    for (int curr_pos = 0; curr_pos < result._digits.size(); ++curr_pos) {
        unsigned long long temp = static_cast<long long>(result._digits[curr_pos]) * right + addition;
        result._digits[curr_pos] = temp % NUM_BASE;
        addition = temp / NUM_BASE;
    }
    if (addition >= NUM_BASE) {
        result._digits.push_back(addition % NUM_BASE);
        result._digits.push_back(addition / NUM_BASE);
    } else {
        result._digits.push_back(addition);
    }
    while (!result._digits.empty() && result._digits.back() == 0)
        result._digits.pop_back();
    return result;
}

/**
 * @brief Compares vector of digits with operator <
 * @param step points to the number of digits from beginning of left var to compare
 */
bool isLowerDigits(const std::vector<char> &left, const std::vector<char> &right, int step) {
    if ((step > right.size()) && (step <= left.size())) return false;
    if ((step < right.size()) || (left.size() < right.size())) return true;
    if (step > left.size()) return true;
    for (int i = right.size() - 1; i >= 0; --i) {
        if (left[left.size() + i - step] < right[i]) return true;
        if (left[left.size() + i - step] > right[i]) return false;
    }
    return false;
}

bool isLowerDigits(const std::vector<char> &left, const std::vector<char> &right, int step);

auto addDigits (const std::vector <char> &lhs, const std::vector <char> &rhs) {

    constexpr auto BASE = 10;
    std::vector <char> result;


    if (isLowerDigits(lhs, rhs, 0)) {
        result = rhs;
        int addition{};
        for (std::size_t i = 0; i < rhs.size(); ++i) {
            result[i] += (i >= lhs.size()) ? addition : lhs[i] + addition;
            addition = result[i] / BASE;
            result[i] %= BASE;
        }
    } else {
        result = lhs;
        int addition{};
        for (std::size_t i = 0; i < rhs.size(); ++i) {
            result[i] += (i >= rhs.size()) ? addition : rhs[i] + addition;
            addition = result[i] / BASE;
            result[i] %= BASE;
        }
    }
    return result;
}

/**
 * @brief Multiplies vector of digits and int
 */
std::vector<char> multiplyDigits(std::vector<char> &num1, int num2) {
    std::vector<char> result = num1;
    int addition = 0;
    for (int curr_pos = 0; curr_pos < result.size(); ++curr_pos) {
        long long temp = result[curr_pos] * num2 + addition;
        result[curr_pos] = temp % 10;
        addition = temp / 10;
    }
    if (addition != 0) {
        result.push_back(addition);
    }
    return result;
}

/**
 * @brief Returns power of basis to make two numbers almost equal
 * @param fnum number to reach
 * @param snum number to multiply
 * @return Maximum power of basis to reach bottom border of fnum
 */
int getMaxPow(std::vector<char> &fnum, std::vector<char> &snum)
{
    if (snum.size() > fnum.size()) {
        return -1;
    }
    int current_pow = fnum.size() - snum.size();
    for (int i = snum.size() - 1; i >= 0; --i) {
        if (snum[i] > fnum[i + current_pow]) {
            return current_pow - 1;
        }
        if (snum[i] < fnum[i + current_pow]) {
            return current_pow;
        }
    }
    return current_pow;
}

/// TODO fix +=10 after changing basis
/**
 * @brief Subtracts two numbers, where second one is multiplied on basis with given power.
 *        Result stores in first parameter
 * @param curr_pow power of basis
 */
void subtractPow(std::vector<char> &fnum, std::vector<char> &snum, int curr_pow)
{
    for (int i = 0; i < snum.size(); ++i) {
        if (fnum[i + curr_pow] < snum[i]) {
            fnum[i + curr_pow] += 10;
            fnum[i + curr_pow + 1] -= 1;
        }
        fnum[i + curr_pow] -= snum[i];
    }
}

/**
 * @brief Compares with operator<= two numbers, where second one is multiplied on basis with given power
 * @param curr_pow power of basis
 */
bool lessNumPow(std::vector<char> &fnum, std::vector<char> &snum, int curr_pow)
{
    if (curr_pow + snum.size() < fnum.size()) {
        return true;
    }
    for (int i = snum.size() - 1; i >= 0; --i) {
        if (fnum[i + curr_pow] < snum[i]) {
            return false;
        }
        if (fnum[i + curr_pow] > snum[i]) {
            return true;
        }
    }
    return true;
}

std::pair<BigNum, BigNum> extract(const BigNum &left, const BigNum &right) {
    if (left < right)
        return std::pair<BigNum, BigNum>(BigNum("0"), left);
    std::vector<char> fnum = toOneDigit(left);
    std::vector<char> snum = toOneDigit(right);
    std::vector<char> current = snum;
    std::vector<char> result;
    int curr_pow = getMaxPow(fnum, snum);
    int hight_rank = 1;
    while (true) {
        while (lessNumPow(fnum, current, curr_pow)) {
            ++hight_rank;
            current = multiplyDigits(snum, hight_rank);
        }
        current = multiplyDigits(snum, hight_rank - 1);
        subtractPow(fnum, current, curr_pow);
        result.push_back(hight_rank-1);
        int to_delete = current.size();
        while (!fnum.empty() && (fnum.back() == 0) && (to_delete > 0)) {
            fnum.pop_back();
            --to_delete;
        }
        while (!fnum.empty() && (fnum.back() == 0)) {
            fnum.pop_back();
            --curr_pow;
            result.push_back(0);
        }
        if ((fnum.size() <= curr_pow) && (curr_pow <= snum.size()) && !lessNumPow(fnum, snum, 0)) {
            for (int i = 0; i < fnum.size(); i++) {
                result.push_back(0);
            }
        }
        int new_pow = getMaxPow(fnum, snum);
        if (new_pow == -1) {
            break;
        }
        while (curr_pow != new_pow + 1) {
            --curr_pow;
            result.push_back(0);
        }
        --curr_pow;
        hight_rank = 1;
        current = snum;
    }
    std::reverse(result.begin(), result.end());
    return std::pair<BigNum, BigNum>(toBigNum(result), toBigNum(fnum));
}

void modify(BigNum &num, const BigNum &mod) {
    num = extract(num, mod).second;
}

BigNum add(const BigNum &left, const BigNum &right, const BigNum &mod) {
    auto t_num2 = right % mod;
    auto t_num1 = left % mod;
    BigNum result = t_num1 + t_num2;
    modify(result, mod);
    return result;
}

BigNum subtract(const BigNum &left, const BigNum &right, const BigNum &mod) {
    auto t_num2 = right % mod;
    auto t_num1 = left % mod;
    BigNum result;
    if (t_num1 >= t_num2) {
        result = t_num1 - t_num2;
    } else {
        result = mod - t_num2 + t_num1;
    }
    return result;
}

BigNum operator%(const BigNum &left, const BigNum &right) {
    BigNum result = extract(left, right).second;
    return result;
}

namespace {
    /*
    *  @brief Class for read-only range of elements of some std::vector.
    */
    template <typename T>
    class ArrayView  {
    public:
        template <typename Iter,
                  typename = std::enable_if <std::is_same_v<typename std::iterator_traits<Iter>::iterator_category,
                                                            std::random_access_iterator_tag>
                                          || std::is_same_v <Iter, T*>>>
        explicit ArrayView(Iter begin,
                           Iter end)
                         : m_begin(&(*begin)),
                           m_size(end - begin) {
        }

        const T* begin() const {
            return m_begin;
        }

        const T* end() const {
            return m_begin + m_size;
        }

        const auto size() const {
            return m_size;
        }

        const auto& operator[](std::size_t n) const {
            if (n >= m_size)
                throw std::out_of_range("Out of view range.");
            return *(m_begin + n);
        }

    private:
        const T* m_begin;
        std::size_t m_size;
    };

    template <typename Iter>
    ArrayView (Iter begin, Iter end) -> ArrayView<typename std::iterator_traits<Iter>::value_type>;

    using IntVectorView = ArrayView<int64_t>;

    /**
     *  @brief Returns nearest number bigger than n that is degree of two
    */
    inline unsigned upperLog2(unsigned n) {
        return std::pow(2, static_cast<int>(std::log2(n)) + 1);
    }

    std::vector<int64_t> naiveMultiplication(const IntVectorView& lhs,
                                              const IntVectorView& rhs) {
        std::vector<int64_t> result(lhs.size() + rhs.size());

        for (int i = 0; i < lhs.size(); ++i) {
            for (int j = 0; j < rhs.size(); ++j) {
                result[i + j] += lhs[i] * rhs[j];
            }
        }

        return result;
    }

    /**
     * @brief Minimum size of vector of digits to do
     *        fast multiplication instead of naive approach
     */
    constexpr inline int MIN_FOR_KARATSUBA = 32;

    /*
     * @brief Karatsuba's method implements fast multiplication of numbers [AB] and [CD] like
     *        like (A * 10 + B) * (C * 10 + D) = AC * 100 + BD + ((A + B) * (C + D) - AC - BD) * 10
     */

    std::vector<int64_t> karatsuba(const IntVectorView& lhs, const IntVectorView& rhs) {

        if (lhs.size() <= MIN_FOR_KARATSUBA)
            return naiveMultiplication(lhs, rhs);

        const auto length = lhs.size();
        std::vector<int64_t> result(length * 2);

        ArrayView lhsL(lhs.begin() + length / 2, lhs.end());
        ArrayView rhsL(rhs.begin() + length / 2, rhs.end());
        ArrayView lhsR(lhs.begin(), lhs.begin() + length / 2);
        ArrayView rhsR(rhs.begin(), rhs.begin() + length / 2);

        const auto c1 = karatsuba(lhsL, rhsL);
        const auto c2 = karatsuba(lhsR, rhsR);

        std::vector<int64_t> lhsLR(length / 2);
        std::vector<int64_t> rhsLR(length / 2);

        for (auto i = 0; i < length / 2; ++i) {
            lhsLR[i] = lhsL[i] + lhsR[i];
            rhsLR[i] = rhsL[i] + rhsR[i];
        }

        auto c3 = karatsuba(IntVectorView(lhsLR.begin(), lhsLR.end()),
                            IntVectorView(rhsLR.begin(), rhsLR.end()));

        for (auto i = 0; i < length; ++i){
            c3[i] -= c2[i] + c1[i];
        }

        for (auto i = 0; i < length; ++i) {
            result[i] = c2[i];
        }

        for (auto i = length; i < length * 2; ++i) {
            result[i] = c1[i - length];
        }

        for (auto i = length / 2; i < length + length / 2; ++i) {
            result[i] += c3[i - length / 2];
        }

        return result;
    }

    /*
     * @brief Normalizing number ceils to NUM_BASE
     */
    inline void finalize(std::vector<int64_t>& num) {
        for (auto i = 0; i < num.size(); ++i) {
            if (i != num.size() - 1) {
                num[i + 1] += num[i] / NUM_BASE;
            }
            num[i] %= NUM_BASE;
        }
    }

    /*
    *  @return Pair of x, y
    *          ax + by = gcd(a, b)
    */
    std::pair<BigNum, BigNum> extendedEuclid(const BigNum& a, const BigNum& b, const BigNum& mod) {

        if (b == BigNum("0")) {
            return std::pair(BigNum("1"), BigNum("0"));
        }
        const auto [int_part, remainder] = extract(a, b);
        const auto [x, y] = extendedEuclid(b, remainder, mod);
        return std::pair(y, subtract(x, (int_part * y) % mod, mod));
    }

    BigNum gcd(const BigNum& lhs, const BigNum& rhs) {
        if (lhs == BigNum("0")) {
            return rhs;
        }
        return gcd (rhs % lhs, lhs);
    }

    bool isPrime(const BigNum& num) {
        if (num <= 1_bn) {
            return false;
        }
        if (num <= 3_bn) {
            return true;
        }

        if (num % 2_bn == 0_bn || num % 3_bn == 0_bn) {
            return false;
        }

        for (auto i = 5_bn; i * i <= num; i = i + 6_bn) {
            if (num % i == 0_bn || num % (i + 2_bn) == 0_bn) {
                return false;
            }
        }
        return true;
    }

    BigNum pow(const BigNum& num, const BigNum& degree, const BigNum& mod) {
        if (degree == 0_bn) {
            return 1_bn;
        }

        auto result = pow(num, extract(degree, 2_bn).first, mod) % mod;
        result = (result * result) % mod;
        return degree % 2_bn == 0_bn ? result : (result * num) % mod;
    }
}

BigNum operator* (const BigNum& lhs, const BigNum& rhs) {

    std::vector<int64_t> lhsTemp(lhs._digits.begin(), lhs._digits.end());
    std::vector<int64_t> rhsTemp(rhs._digits.begin(), rhs._digits.end());
    auto maxSize = std::max(lhsTemp.size(), rhsTemp.size());

    lhsTemp.resize(upperLog2(maxSize));
    rhsTemp.resize(upperLog2(maxSize));

    auto nums = karatsuba(IntVectorView(lhsTemp.begin(), lhsTemp.end()),
                          IntVectorView(rhsTemp.begin(), rhsTemp.end()));
    finalize(nums);

    while (!nums.empty() && nums.back() == 0) {
        nums.pop_back();
    }

    BigNum result;
    result._digits = std::vector<int>(nums.begin(), nums.end());
    return result;
}

BigNum multiply(const BigNum& lhs, const BigNum& rhs, const BigNum& mod) {
        return (lhs % mod * rhs % mod) % mod;
}

BigNum inverted(const BigNum &num, const BigNum& mod,
                                   BigNum::InversionPolicy policy = BigNum::InversionPolicy::Euclid){

    if (policy == BigNum::InversionPolicy::Euclid) {
        if (gcd(num, mod) != 1_bn)
            throw std::invalid_argument("Nums must be coprime.");
        auto inverted_ = extendedEuclid(num, mod, mod).first;
        while (!inverted_._digits.empty() && inverted_._digits.back() == 0)
            inverted_._digits.pop_back();
        return inverted_;
    } else {
        if (!isPrime(mod)) {
            throw std::invalid_argument("Mod must be prime.");
        }
        if (gcd(num, mod) != 1_bn) {
            throw std::invalid_argument("Nums must be coprime.");
        }
        return pow (num, mod - 2_bn, mod);
    }
}
}

lab::BigNum operator"" _bn(const char* str) {
    return lab::BigNum(str);
}