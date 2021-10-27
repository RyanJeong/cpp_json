/*
  Copyright 2020 Ryan M. Jeong <ryan.m.jeong@hotmail.com>
*/
#include <iostream>

#include "nlohmann/json.hpp"

int main() {
  // create an empty structure (null)
  nlohmann::json j1;

  // add a number that is stored as double (note the implicit conversion of j to an object)
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

  // instead, you could also write (which looks very similar to the JSON above)
  nlohmann::json j2 = {
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

  return 0;
}
