#include "database.h"
#include "record.h"
#include "query.h"
#include <iostream>
#include <sstream>
int main() {
    auto r = q2::record{};
    
    r.set_value("name", "Alice");
    r.set_value("age", "30");
    r.set_value("city", "Wonderland");

    std::cout << "Initial record:" << std::endl;
    std::cout << r;

    std::cout << "Checking attribute 'name': " << (r.has_attribute("name") ? "Exists" : "Does not exist") << std::endl;
    std::cout << "Checking attribute 'country': " << (r.has_attribute("country") ? "Exists" : "Does not exist") << std::endl;

    std::cout << "Value of 'name': " << r.get_value("name") << std::endl;
    std::cout << "Value of 'country': " << r.get_value("country") << std::endl;

    r.set_value("age", "31");
    std::cout << "Record after updating 'age':" << std::endl;
    std::cout << r;

    bool deleted = r.delete_attribute("city");
    std::cout << "Deleted 'city': " << (deleted ? "Success" : "Failed") << std::endl;
    std::cout << "Record after deleting 'city':" << std::endl;
    std::cout << r;

    deleted = r.delete_attribute("country");
    std::cout << "Deleted 'country': " << (deleted ? "Success" : "Failed") << std::endl;
    std::cout << "Record after trying to delete 'country':" << std::endl;
    std::cout << r;

    std::istringstream input_stream("{\nname=Bob\nage=25\n}\n");
    r = q2::record{};
    input_stream >> r;
    std::cout << "Record after reading from input stream:" << std::endl;
    std::cout << r;

}