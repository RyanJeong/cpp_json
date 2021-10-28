/*
  Copyright 2021 Ryan M. Jeong <ryan.m.jeong@hotmail.com>
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <set>
#include <unordered_set>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

void first_class_data_type(bool);
void serialization_deserialization(bool);
void stl_like_accesS(bool);
void conversion_from_stl_containers(bool);

int main() {
  first_class_data_type(false);
  serialization_deserialization(false);
  stl_like_accesS(false);
  conversion_from_stl_containers(true);

  return 0;
}

void first_class_data_type(bool f) {
  if (!f) {
    return;
  }
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

  std::cout << "j1's content:" << std::endl
            << j1["pi"] << std::endl
            << j1["happy"] << std::endl
            << j1["name"] << std::endl
            << j1["nothing"] << std::endl
            << j1["answer"]["everything"] << std::endl
            << j1["list"] << std::endl
            << j1["object"] << std::endl << std::endl;

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

  std::cout << "j2's content:" << std::endl
            << j2["pi"] << std::endl
            << j2["happy"] << std::endl
            << j2["name"] << std::endl
            << j2["nothing"] << std::endl
            << j2["answer"]["everything"] << std::endl
            << j2["list"] << std::endl
            << j2["object"] << std::endl << std::endl;

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
  if (!f) {
    return;
  }

  // create object from string literal
  json j1 = "{ \"happy\": true, \"pi\": 3.141 }"_json;

  std::cout << "j1's content:" << std::endl
            << j1["happy"] << std::endl
            << j1["pi"] << std::endl;

  // or even nicer with a raw string literal(C++)
  auto j2 = R"(
    {
      "happy": true,
      "pi": 3.141
    }
  )"_json;

  std::cout << "j2's content:" << std::endl
            << j2["happy"] << std::endl
            << j2["pi"] << std::endl;

  // parse explicitly
  auto j3 = json::parse(R"({"happy": true, "pi": 3.141})");
  std::cout << "j3's content:" << std::endl
            << j3["happy"] << std::endl
            << j3["pi"] << std::endl;

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
  std::cout << cpp_string << " == "
            << cpp_string2 << " == "
            << j_string.get<std::string>() << '\n';
  // output of serialized value
  std::cout << j_string << " == "
            << serialized_string << std::endl;

  // deserialize from standard input
  json j;
  // { "happy": true, "pi": 3.141 }
  std::cin >> j;

  // serialize to standard output
  std::cout << j << std::endl;

  // the setw manipulator was overloaded to
  // set the indentation for pretty printing
  std::cout << std::setw(4) << j << std::endl << std::endl;

  // read a JSON file
  std::ifstream i("file.json");
  i >> j;

  // write prettified JSON to another file
  std::ofstream o("pretty.json");
  o << std::setw(4) << j << std::endl;

  std::vector<std::uint8_t> v = {'t', 'r', 'u', 'e'};
  j = json::parse(v.begin(), v.end());
  std::cout << j << std::endl;
  j = json::parse(v);  // [begin, end)
  std::cout << j << std::endl;
}

void stl_like_accesS(bool f) {
  if (!f) {
    return;
  }

  // create an array using push_back
  json j;
  j.push_back("foo");
  j.push_back(1);
  j.push_back(true);

/*
what if you want to construct a string of 10 'x' characters? 
In this case, your choices are

vVec.push_back(std::string(10, 'x'));
vVec.emplace_back(10, 'x');

In this case, push_back involves calling a custom string constructor and 
then a move constructor, but emplace_back calls the custom string constructor 
directly, saving the call to the move constructor.
*/
  // also use emplace_back
  j.emplace_back(1.78);

  // iterate the array
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    std::cout << *it << '\n';
  }
  std::cout << std::endl;

  // range-based for
  for (auto& element : j) {
    std::cout << element << '\n';
  }
  std::cout << std::endl;

  // getter/setter
  const auto tmp = j[0].get<std::string>();
  std::cout << "tmp: " << tmp << std::endl;
  j[1] = 42;
  std::cout << "f[1]: " << j[1] << std::endl;
  bool foo = j.at(2);
  std::cout << "foo: " << foo << std::endl;

  // range-based for
  for (auto& element : j) {
    std::cout << element << '\n';
  }
  std::cout << std::endl;

  // comparison
  if (j == R"(["foo", 1, true, 1.78])"_json) {
    std::cout << "j == R([foo, 1, true, 1.78])" << std::endl;
  } else {
    std::cout << "j != R([foo, 1, true, 1.78])" << std::endl;
  }
  if (j == json::parse(R"(["foo", 42, true, 1.78])")) {
    std::cout << "j == R([foo, 42, true, 1.78])" << std::endl;
  } else {
    std::cout << "j != R([foo, 42, true, 1.78])" << std::endl;
  }

  // other stuff
  std::cout << "j.size(): "
            << j.size() << std::endl;     // 4 entries
  std::cout << "j.empty(): "
            << j.empty() << std::endl;    // false
  std::cout << "j.type() == json::value_t::array: "
            << ((j.type() == json::value_t::array) ? "True" : "False")
            << std::endl;     // json::value_t::array

  j.clear();  // the array is empty again
  std::cout << "j.empty(): "
            << j.empty() << std::endl;    // true

  // convenience type checkers
  std::cout << "j.is_null(): "
            << j.is_null() << std::endl;
  std::cout << "j.is_boolean(): "
            << j.is_boolean() << std::endl;
  std::cout << "j.is_number(): "
            << j.is_number() << std::endl;
  std::cout << "j.is_object(): "
            << j.is_object() << std::endl;
  std::cout << "j.is_array(): "
            << j.is_array() << std::endl;
  std::cout << "j.is_string(): "
            << j.is_string() << std::endl;

  // create an object
  json o;
  o["foo"] = 23;
  o["bar"] = false;
  o["baz"] = 3.141;

  // also use emplace
  o.emplace("weather", "sunny");

  // special iterator member functions for objects
  for (json::iterator it = o.begin(); it != o.end(); ++it) {
    std::cout << it.key() << " : " << it.value() << "\n";
  }

  // the same code as range for
  for (auto& el : o.items()) {
    std::cout << el.key() << " : " << el.value() << "\n";
  }

  // even easier with structured bindings (C++17)
  /*
  for (auto& [key, value] : o.items()) {
    std::cout << key << " : " << value << "\n";
  }
  */

  // find an entry
  if (o.contains("foo")) {
    std::cout << "o contains foo!" << std::endl;
    // there is an entry with key "foo"
  } else {
    std::cout << "o doesn't contain foo!" << std::endl;
  }


  // or via find and an iterator
  if (o.find("fob") != o.end()) {
    std::cout << "o contains fob!" << std::endl;
    // there is an entry with key "fob"
  } else {
    std::cout << "o doesn't contain fob!" << std::endl;
  }

  // or simpler using count()
  int foo_present = o.count("foo");  // 1
  std::cout << foo_present << std::endl;
  int fob_present = o.count("fob");  // 0
  std::cout << fob_present << std::endl;

  // delete an entry
  o.erase("foo");
  // find an entry
  if (o.contains("foo")) {
    std::cout << "o contains foo!" << std::endl;
    // there is an entry with key "foo"
  } else {
    std::cout << "o doesn't contain foo!" << std::endl;
  }
}

void conversion_from_stl_containers(bool f) {
  if (!f) {
    return;
  }

  std::vector<int> c_vector{1, 2, 3, 4};
  json j_vec(c_vector);  // [1, 2, 3, 4]
  for (auto& elem : j_vec) {
    std::cout << elem << ' ';
  }
  std::cout << std::endl;

  std::deque<double> c_deque{1.2, 2.3, 3.4, 5.6};
  json j_deque(c_deque);  // [1.2, 2.3, 3.4, 5.6]
  /*
  json::iterator
  */
  for (auto it = j_deque.begin(); it != j_deque.end(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;

  for (auto it = j_deque.rbegin(); it != j_deque.rend(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;

  std::list<bool> c_list{true, true, false, true};
  json j_list(c_list);  // [true, true, false, true]
  for (auto it = j_list.begin(); it != j_list.end(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;

  for (auto it = j_list.rbegin(); it != j_list.rend(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;


  std::forward_list<int64_t> c_flist{
    12345678909876, 23456789098765, 34567890987654, 45678909876543
  };
  json j_flist(c_flist);
  // [12345678909876, 23456789098765, 34567890987654, 45678909876543]
  for (auto it = j_flist.begin(); it != j_flist.end(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;

  for (auto it = j_flist.rbegin(); it != j_flist.rend(); ++it) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;

  std::array<uint32_t, 4> c_array{{1, 2, 3, 4}};
  json j_array(c_array);
  // [1, 2, 3, 4]
  for (auto& elem : j_array) {
    std::cout << elem << ' ';
  }
  std::cout << std::endl;

  std::set<std::string> c_set{"one", "two", "three", "four", "one"};
  json j_set(c_set);  // only one entry for "one" is used
  // ["four", "one", "three", "two"]
  for (auto& elem : j_set) {
    std::cout << elem << ' ';
  }
  std::cout << std::endl;

  std::unordered_set<std::string> c_uset{"one", "two", "three", "four", "one"};
  json j_uset(c_uset);  // only one entry for "one" is used
  // maybe ["four", "three", "two", "one"]
  for (auto& elem : j_uset) {
    std::cout << elem << ' ';
  }
  std::cout << std::endl;

  std::multiset<std::string> c_mset{"one", "two", "one", "four"};
  json j_mset(c_mset);  // both entries for "one" are used
  // maybe ["four", "one", "one", "two" ]
  for (auto& elem : j_mset) {
    std::cout << elem << ' ';
  }
  std::cout << std::endl;

  std::unordered_multiset<std::string> c_umset{"one", "two", "one", "four"};
  json j_umset(c_umset);  // both entries for "one" are used
  // maybe ["four", "two", "one", "one"]
  for (auto& elem : j_umset) {
    std::cout << elem << ' ';
  }
  std::cout << std::endl;

  std::map<std::string, int> c_map{
    {"one", 1}, {"two", 2}, {"three", 3}
  };
  json j_map(c_map);
  // {"one": 1, "three": 3, "two": 2 }
  // special iterator member functions for objects
  for (json::iterator it = j_map.begin(); it != j_map.end(); ++it) {
    std::cout << it.key() << " : " << it.value() << ' ';
  }
  std::cout << std::endl;

  std::unordered_map<const char*, double> c_umap{
    {"one", 1.2}, {"two", 2.3}, {"three", 3.4}, {"four", 4.5}, {"five", 5.6}
  };
  json j_umap(c_umap);
  // {"one": 1.2, "two": 2.3, "three": 3.4}
  for (json::iterator it = j_umap.begin(); it != j_umap.end(); ++it) {
    std::cout << it.key() << " : " << it.value() << ' ';
  }
  std::cout << std::endl;

  std::multimap<std::string, bool> c_mmap{
    {"one", true}, {"two", true}, {"three", false}, {"three", true}
  };
  std::cout << "c_mmap: ";
  for (auto& elem : c_mmap) {
    std::cout << elem.first << " : " << elem.second << ' ';
  }  // c_mmap: {"one" : 1, "three" : 0, "three" : 1, "two" : 1}
  std::cout << std::endl;
  json j_mmap(c_mmap);  // only one entry for key "three" is used
  // maybe {"one": true, "two": true, "three": true}
  for (json::iterator it = j_mmap.begin(); it != j_mmap.end(); ++it) {
    std::cout << it.key() << " : " << it.value() << ' ';
  }
  std::cout << std::endl;
  // Note that in case of multimaps only one key is used in
  // the JSON object and the value depends on the internal order
  // of the STL container.

  std::unordered_multimap<std::string, bool> c_ummap{
    {"one", true}, {"two", true}, {"three", false}, {"three", true}
  };
  json j_ummap(c_ummap);  // only one entry for key "three" is used
  for (json::iterator it = j_ummap.begin(); it != j_ummap.end(); ++it) {
    std::cout << it.key() << " : " << it.value() << ' ';
  }
  std::cout << std::endl;
  // maybe {"one": true, "two": true, "three": true}
}
