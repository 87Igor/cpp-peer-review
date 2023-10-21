City CreateCity(const Node& city_node, const Country& country) {

    return City(city_node["name"s].AsString(),
                city_node["iso_code"s].AsString(),
                country.phone_code + city_node["phone_code"s].AsString(),
                country.name, country.iso_code,
                country.time_zone, country.languages);
}

Country CreateCountry(const Node& country_node) {
    return Country(country_node["name"s].AsString(),
                   country_node["iso_code"s].AsString(),
                   country_node["phone_code"s].AsString(),
                   country_node["time_zone"s].AsString());
}

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const Country& country) {
    for (const auto& city_json : json.AsList()) {
        //получаем city_obj, создаем City и добавляем в cities
        cities.push_back(std::move(CreateCity(city_json.AsObject(), country)));
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        
        const auto& country_obj = country_json.AsObject();
        
	//создаем Country и добавляем в countries
        countries.push_back(std::move(CreateCountry(country_obj)));
        
        Country& country = countries.back();
        
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        
        ParseCitySubjson(cities, country_obj["cities"s], country);
    }
}