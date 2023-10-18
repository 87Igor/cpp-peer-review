struct ParamsConnect {
    string_view db_name;
    int timeout;
    bool allow_exceptions;
    DBLogLevel log_level;
};

struct Filters {
    int min_age;
    int max_age;
    string_view name_filter;
};

//возвращает данные для создания DBQuery
string DataRequest(const DBHandler& db, const Filters& filters) {

    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << filters.min_age << " and "s << filters.max_age << " "s
        << "and Name like '%"s << db.Quote(filters.name_filter) << "%'"s;

    return query_str.str();

}

//создает подключение к базе данных
DBHandler ConnectDB(const ParamsConnect& params_connect) {
    DBConnector connector(params_connect.allow_exceptions, params_connect.log_level);
    DBHandler db;
    if (params_connect.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(params_connect.db_name, params_connect.timeout);
    }
    else {
        db = connector.Connect(params_connect.db_name, params_connect.timeout);
    }
    return db;
}

vector<Person> LoadPersons(const ParamsConnect& params_connect, const Filters& filters) {
  
    //создаем подключение
    const DBHandler& db = ConnectDB(params_connect);

    if (!params_connect.allow_exceptions && !db.IsOK()) {
        return {};
    }
    //Формируем данные для зароса и создаем query
    DBQuery query(std::move(DataRequest(db, filters)));

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}