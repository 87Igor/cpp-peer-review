#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <iomanip>
#include <fstream>

using namespace std;

enum class TypeReq { READ, CHEER };

struct InfoReq {
    TypeReq type_req;
    int id_user = 0;
    int count_page = 0;
};

class StatsEbook {
private:
    const int MAX_COUNT_USER = 100000;
    const int MAX_COUNT_PAGE = 1000;

    vector<int> page_num_read_user;
    vector<int> count_user_read_page;

public:
    StatsEbook()
        //id_users может быть от 0 до 10⁵(100'000) прибавим 1 для vector
        //количество page может быть от 0 до 1000 прибавим 1 для vector
        :page_num_read_user(MAX_COUNT_USER + 1, 0), count_user_read_page(MAX_COUNT_PAGE + 1, 0)
    {};

    void Read(const InfoReq& info_req, ostream& out){
        if (info_req.id_user == 0) {
            return;
        }
        const int old_count_page_user = page_num_read_user[info_req.id_user];
        page_num_read_user[info_req.id_user] = info_req.count_page;

        //если user был уже добавлен ранее , нужно вычесть его из прошлого элемента кол-ва строк
        if (count_user_read_page[old_count_page_user] > 0) {
            --count_user_read_page[old_count_page_user];
        }
        ++count_user_read_page[info_req.count_page];
    }
    
    void Cheer(const InfoReq& info_req, ostream& out) {
        const int all_user_read = accumulate(count_user_read_page.begin(), count_user_read_page.end(), 0);

        if (all_user_read == 0 || page_num_read_user[info_req.id_user] == 0) {
            out << 0 << endl;
            return;
        }

        if (all_user_read == 1) {
            out << 1 << endl;
            return;
        }
        const int count_read_user = page_num_read_user[info_req.id_user];

        const int less_user_read = accumulate(count_user_read_page.begin(), count_user_read_page.begin() + count_read_user, 0);
        if (less_user_read == 0) {
            out << 0 << endl;
            return;
        }
        //вычитаем текущего user из общего кол-ва  {all_user_read -1}
        out << setprecision(6) << less_user_read * 1.0 / (all_user_read - 1) << endl;

    }

};

InfoReq Parse_req(string& str_req) {

    InfoReq info_req;
    
    auto last_str = str_req.find(' ', 0);
    string type_req = str_req.substr(0, last_str);

    str_req = str_req.substr(last_str + 1, str_req.npos);
    
    if (type_req == "READ") {
        info_req.type_req = TypeReq::READ;

        last_str = str_req.find(' ', 0);
        string id_user = str_req.substr(0, last_str);

        str_req = str_req.substr(last_str + 1, str_req.npos);

        string count_page = str_req;

        info_req.id_user = stoi(id_user);
        info_req.count_page = stoi(count_page);
    }
    else {
        info_req.type_req = TypeReq::CHEER;

        string id_user = str_req;

        info_req.id_user = stoi(id_user);
    }

    return info_req;

}

int main() {

    int count_req = 0;
    
    cin >> count_req;

    string str_req;

    cin.get();

    StatsEbook stats_ebook;

    while (count_req > 0)
    {

        getline(cin, str_req);

        InfoReq info_req = Parse_req(str_req);

        switch (info_req.type_req)
        {
            {
        case TypeReq::READ:
            stats_ebook.Read(info_req, cout);
            break;
            }
            {
        case TypeReq::CHEER:
            stats_ebook.Cheer(info_req, cout);
            }
        }

        --count_req;
    };
    
    return 0;
}