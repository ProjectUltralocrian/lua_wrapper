#include <iostream>
#include <string>
#include "lua_wrapper.h"

struct Person
{
    std::string name;
    int age;
    Person(std::string_view name, int age) : name{name}, age{age} {}
};

std::ostream &operator<<(std::ostream &stream, const Person &person)
{
    return stream << "PERSON: " << person.name << "(" << person.age << ")" << std::endl;
}

int main()
{
    pul::LuaInstance L;

    auto fileResult = L.exec_file("script.lua");
    if (fileResult.isError())
    {
        std::cerr << fileResult.getError() << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string name = L.get_string("name").okOrDefault("Name not found");
    int age = L.get_number("age").okOrDefault(0);
    Person person(name, age);
    std::cout << person << std::endl;

    auto name2 = L.get_string("Name");
    auto age2 = L.get_number("Age");
    if (name2.isOk() && age2.isOk())
    {
        Person person2(name2.unwrap(), age2.unwrap());
        std::cout << person2 << std::endl;
    }

    auto res = pul::Result<std::string, std::string>::Ok("Hello error");
    if (res)
    {
        std::cout << "SUCCESS: " << res.unwrap() << std::endl;
    }
    else
    {
        std::cout << "ERROR: " << res.getError() << std::endl;
    }
    std::cout << res.okOrDefault("Default") << std::endl;

    res = pul::Result<std::string, std::string>::Ok("Hello OK");
    std::cout << res.unwrap() << std::endl;

    auto table = L.get_all_values_in_table("stuff");
    if (table.isError())
        std::cout << table.getError() << std::endl;
    else
        for (const auto &s : table.unwrap())
        {
            std::cout << s << std::endl;
        }

    return EXIT_SUCCESS;
}