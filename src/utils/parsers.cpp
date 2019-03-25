//
// Created by lirundong on 2019-03-25.
//

#include "utils/parsers.hpp"

namespace toy_json {

// TODO: Implement parsing helpers in this file. If any parsing error occurs,
//   e.g., meet with invalid input string, just throw `std::runtime_error` with
//   bellowing error information (without surrounding ` ):
//   - fail parsing `bool`:    `parsing bool type failed`
//   - fail parsing `number`:  `parsing number type failed`
//   - fail parsing `string`:  `parsing string type failed`
//   - fail parsing `array`:   `parsing array type failed`
//   - fail parsing `object`:  `parsing object type failed`

bool parse_bool(std::string::const_iterator &str_it) {
  bool result = false;
  // TODO: implement parsing and advance iterator

  return result;
}

double parse_number(std::string::const_iterator &str_it) {
  double result = 0.0;
  // TODO: implement parsing and advance iterator

  return result;
}

std::string parse_string(std::string::const_iterator &str_it) {
  std::string result;
  // TODO: implement parsing and advance iterator
  // NOTE: you are required to store unicode code-points in UTF-8 encoding

  return result;
}

JsonNode::array parse_array(std::string::const_iterator &str_it) {
  JsonNode::array result;
  // TODO: implement parsing and advance iterator
  // NOTE: we did not provide `parse_null` above, however `null` is one of the
  //   valid array values

  return result;
}

JsonNode::object parse_object(std::string::const_iterator &str_it) {
  JsonNode::object result;
  // TODO: implement parsing and advance iterator
  // NOTE: 1. we did not provide `parse_null` above, however `null` is one of the
  //          valid object values
  //       2. object key can be any kind of json string, which means it may
  //          contain spaces, escapes, unicode code-points, etc., so take care!

  return result;
}

}
