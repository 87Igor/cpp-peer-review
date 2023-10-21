#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;

class Domain {
public:
    Domain(std::string name)
        : name_(std::string(name.rbegin(), name.rend()) + '.')
    {}

    bool IsSubdomain(const Domain& domain) const{
        return this->name_.substr(0, domain.name_.length()) == domain.name_;
    }

    bool operator == (const Domain& domain) const {
        return this->name_ == domain.name_;
    }

    bool operator < (const Domain& domain) const
    {
        return this->name_ < domain.name_;
    }
private:
    std::string name_;
};


class DomainChecker {
public:
    template <typename Iter>
    DomainChecker(Iter first, Iter last) {
        Iter it = first;
        while (it != last)
        {
            bidden_domains_.push_back(*it);
            ++it;
        }
        
        std::sort(bidden_domains_.begin(), bidden_domains_.end(), 
            [](Domain& left, Domain& right) {return left < right; });
        
        //оставляем только домены в массиве, поддомены удаляем.
        auto last_it = unique(bidden_domains_.begin(), bidden_domains_.end(),
                        [](Domain& left, Domain& right) { return right.IsSubdomain(left); });
        bidden_domains_.erase(last_it, bidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) {

        const auto iter = std::upper_bound(bidden_domains_.begin(), bidden_domains_.end(), domain);
        if (iter == bidden_domains_.begin()) {
            return false;
        }
        //проверяем предшествующий элемент
        Domain& domain_prev = *(iter - 1);
        return domain_prev == domain || domain.IsSubdomain(domain_prev);
    }

private:
    std::vector<Domain> bidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
std::vector<Domain> ReadDomains(istream& in, size_t count_domains) {
    
    std::vector<Domain> vec_res;
    vec_res.reserve(count_domains);

    while (count_domains != 0)
    {
        string line;
        getline(in, line);

        vec_res.push_back(Domain(line));

        --count_domains;
    }
    return vec_res;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

void TestReadDomains() {

    std::stringstream number_in;
    number_in << 4 << endl;

    std::stringstream str_in;
    str_in << "gdz.ru" << endl
        << "maps.me" << endl
        << "m.gdz.ru" << endl
        << "com" << endl;

    std::vector<Domain> v1 = ReadDomains(str_in, ReadNumberOnLine<size_t>(number_in));
    
    std::vector<Domain> v2{ Domain("ur.zdg"), Domain("em.spam"), Domain("ur.zdg.m"), Domain("moc") };
    
    assert(v1.size() == 4);
    assert(v1[1].IsSubdomain(Domain("me")));

}

void TestChecker() {

    std::stringstream number_in;
    number_in << 4 << endl;

    std::stringstream str_in;
    str_in << "ria.ru" << endl
        << "maps.me" << endl
        << "m.ria.ru" << endl
        << "com" << endl;

    std::vector<Domain> v1 = ReadDomains(str_in, ReadNumberOnLine<size_t>(number_in));

    DomainChecker checker(v1.begin(), v1.end());

    number_in << 7 << endl;

    str_in << "ria.ru" << endl
        << "ria.com" << endl
        << "m.maps.me" << endl
        << "alg.m.ria.ru" << endl
        << "maps.com" << endl
        << "maps.ru" << endl
        << "ria.ua" << endl;

    std::stringstream out;

    std::vector<Domain> v_check = ReadDomains(str_in, ReadNumberOnLine<size_t>(number_in));
    
    for (const Domain& domain : v_check) {
        out << (checker.IsForbidden(domain) ? "0"sv : "1"sv) << endl;
    }

    assert(out.str() == std::string("0\n0\n0\n0\n0\n1\n1\n"));

}

int main() {

    TestReadDomains();

    TestChecker();

    //std::ifstream in("input.txt");

    //std::ofstream out("out.txt");

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}