/*
  Copyright 2021 Ryan M. Jeong <ryan.m.jeong@hotmail.com>
*/
#include <iostream>
#include <iomanip>
#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

void first_class_data_type(bool);
void serialization_deserialization(bool);

int main() {
  first_class_data_type(false);
  serialization_deserialization(true);

  return 0;
}

void first_class_data_type(bool f) {
  // create an empty structure (null)
  json j1;

  // add a number that is stored as double
  // (note the implicit conversion of j to an object)
  j1["pi"] = 3.141;

  // add a Boolean that is stored as bool
  j1["happy"] = true;

  // add a string that is stored as std::string
  j1["name"] = "Niels";

  // add another null object by passing nullptr
  j1["nothing"] = nullptr;

  // add an object inside the object
  j1["answer"]["everything"] = 42;

  // add an array that is stored as std::vector (using an initializer list)
  j1["list"] = { 1, 0, 2 };

  // add another object (using an initializer list of pairs)
  j1["object"] = { {"currency", "USD"}, {"value", 42.99} };

  if (f) {
    std::cout << "j1's content:" << std::endl
              << j1["pi"] << std::endl
              << j1["happy"] << std::endl
              << j1["name"] << std::endl
              << j1["nothing"] << std::endl
              << j1["answer"]["everything"] << std::endl
              << j1["list"] << std::endl
              << j1["object"] << std::endl << std::endl;
  }

  // instead, you could also write (which looks very similar to the JSON above)
  json j2 = {
    {"pi", 3.141},
    {"happy", true},
    {"name", "Niels"},
    {"nothing", nullptr},
    {"answer", {
      {"everything", 42}
    }},
    {"list", {1, 0, 2}},
    {"object", {
      {"currency", "USD"},
      {"value", 42.99}
    }}
  };

  if (f) {
    std::cout << "j2's content:" << std::endl
              << j2["pi"] << std::endl
              << j2["happy"] << std::endl
              << j2["name"] << std::endl
              << j2["nothing"] << std::endl
              << j2["answer"]["everything"] << std::endl
              << j2["list"] << std::endl
              << j2["object"] << std::endl << std::endl;
  }
  
  // a way to express the empty array []
  json empty_array_explicit = json::array();

  // ways to express the empty object {}
  json empty_object_implicit = json({});
  json empty_object_explicit = json::object();

  // a way to express an _array_ of key/value pairs
  // [["currency", "USD"], ["value", 42.99]]
  json array_not_object = json::array({
    {"currency", "USD"},
    {"value", 42.99}
  });
}

void serialization_deserialization(bool f) {
  // create object from string literal
  json j1 = "{ \"happy\": true, \"pi\": 3.141 }"_json;

  if (f) {
    std::cout << "j1's content:" << std::endl
              << j1["happy"] << std::endl
              << j1["pi"] << std::endl;
  }

  // or even nicer with a raw string literal(C++)
  auto j2 = R"(
    {
      "happy": true,
      "pi": 3.141
    }
  )"_json;

  if (f) {
    std::cout << "j2's content:" << std::endl
              << j2["happy"] << std::endl
              << j2["pi"] << std::endl;
  }

  // parse explicitly
  auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");
  if (f) {
    std::cout << "j3's content:" << std::endl
              << j3["happy"] << std::endl
              << j3["pi"] << std::endl;
  }

  // explicit conversion to string
  std::string s = j3.dump();    // {"happy":true,"pi":3.141}

  // serialization with pretty printing
  // pass in the amount of spaces to indent
  std::cout << j3.dump(4) << std::endl;
  // {
  //     "happy": true,
  //     "pi": 3.141
  // }
  std::cout << j3.dump(2) << std::endl;
  // {
  //   "happy": true,
  //   "pi": 3.141
  // }

  // store a string in a JSON value
  json j_string = "this is a string";

  // retrieve the string value
  auto cpp_string = j_string.get<std::string>();
  // retrieve the string value (alternative when an variable already exists)
  std::string cpp_string2;
  j_string.get_to(cpp_string2);

  // retrieve the serialized value (explicit JSON serialization)
  // .dump() returns the originally stored string value.
  std::string serialized_string = j_string.dump();

  // output of original string
  std::cout << cpp_string << " == " << cpp_string2 << " == " << j_string.get<std::string>() << '\n';
  // output of serialized value
  std::cout << j_string << " == " << serialized_string << std::endl;

  // deserialize from standard input
  json j;
  // { "happy": true, "pi": 3.141 }
  std::cin >> j;

  // serialize to standard output
  std::cout << j << std::endl;

  // the setw manipulator was overloaded to set the indentation for pretty printing
  std::cout << std::setw(4) << j << std::endl << std::endl;
}