//
// Created by lirundong on 2019-03-25.
//
#include <locale>
#include <codecvt>
#include <iostream>
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
  // Parse "true"
  if (*str_it == 't'){
    std::string true_str = "true";
    int len_true = 4;
    for (int i = 0; *str_it != '\0' && i < len_true ; i++) {
      if ( *str_it != true_str[i]) {
        throw std::runtime_error("parsing bool type failed");
        break;
      }else {
        str_it++;
      }
    }
    result = true;

  // Parse "false"
  }else {
    std::string false_str = "false"; 
    int len_false = 5;
    for (int i = 0; *str_it != '\0' && i < len_false ; i++) {
      if ( *str_it != false_str[i]) {
        throw std::runtime_error("parsing bool type failed");
        break;
      } else {
        str_it++;
      }
    }
    result = false;
  }

  return result;
}

double parse_number(std::string::const_iterator &str_it) {
  double result = 0.0;
  // TODO: implement parsing and advance iterator
  std::string number_str = std::string();
  bool end_lable = false;
  int len_number = 0;
  // Parse optional '-'
  if ( *str_it == '-' ) {
    len_number++;
    str_it++;
  }
  // Parse two case '0' | ('1-9'+'0-9'*)
  if ( isdigit(*str_it) ) {
    if ( *str_it == '0' ) {
      len_number++;
      str_it++;
    }else {
      len_number++;
      str_it++;
      while ( *str_it != '0' ) {
        if ( isdigit(*str_it) ) {
          len_number++;
          str_it++;
        }else {
          break;
        }
      }
    }
  }else {
    throw std::runtime_error("parsing number type failed");
  }
  // Parse optional '.'+'0-9'+'0-9'* 
  if ( *str_it == '.' ) {
    len_number++;
    str_it++;
    if ( isdigit(*str_it) ) {
      len_number++;
      str_it++;
      while ( *str_it != '0' ) {
        if ( isdigit(*str_it) ) {
          len_number++;
          str_it++;
        }else {
          break;
        }
      }
    }else {
      throw std::runtime_error("parsing number type failed");
    }
  }
  // Parse optional 'e | E' + '+ | | -' + '0-9' + '0-9'*
  if ( *str_it == 'e' || *str_it == 'E' ) {
    len_number++;
    str_it++;
    if ( *str_it == '+' || *str_it == '-' || isdigit(*str_it) ) {
      len_number++;
      str_it++;
      while ( *str_it != '0' ) {
        if ( isdigit(*str_it) ) {
          len_number++;
          str_it++;
        }else {
          break;
        }
      }
    }else {
      throw std::runtime_error("parsing number type failed");
    }
  }
  // Convert string to double
  number_str = std::string(str_it-len_number, str_it);
  // Ensure automata when single unit parse 
  if ( *str_it == '\0' ) {
    result = std::stod(number_str);
  // Ensure automate when parse in Json text
  }else if ( *str_it == ',' || *str_it == ']' || *str_it == '}' ) {
    result = std::stod(number_str);
  }
  else {
    throw std::runtime_error("parsing number type failed");
  }
  return result;
}

std::string parse_string(std::string::const_iterator &str_it) {
  std::string result = std::string();
  // TODO: implement parsing and advance iterator
  // NOTE: you are required to store unicode code-points in UTF-8 encoding
  bool end_with_quote = false;
  if ( *str_it == '"') {
    str_it++;
    // Deal with string obj name
    while ( *str_it != '\0' ){
      if ( *str_it == '\\') {
        str_it++;
        switch ( *str_it ) {
          case '"' :
            result.append("\"");
            str_it++;
            break;
          case '\\':
            result.append("\\");
            str_it++;
            break;
          case '/' :
            result.append("/");
            str_it++;
            break;
          case 'b' :
            result.append("\b");
            str_it++;
            break;
          case 'f' :
            result.append("\f");
            str_it++;
            break;
          case 'n' :
            result.append("\n");
            str_it++;
            break;
          case 'r' :
            result.append("\r");
            str_it++;
            break;
          case 't' :
            result.append("\t");
            str_it++;
            break;
          // Unicode string convert to uft-8
          case 'u' : {
            str_it++;
            std::string unicode_str = std::string("0x");
            for (int i = 0; i < 4 ; i++ ) {
              if ( isalpha(*(str_it+i)) || isdigit(*(str_it+i))) {
                continue;
              }else {
                throw std::runtime_error("parsing string type failed");
              }
            }
            unicode_str.append(str_it, str_it+4);
            str_it+=4;
            // Convert hex string to hex unsigned
            std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
            unsigned hex;
            sscanf(unicode_str.c_str(), "%x", &hex);
            // If is surrogate pair, read one more encode
            if ( 0xd800 < hex && hex < 0xdfff ) {
              if ( *str_it == '\\' && *(str_it+1) == 'u') {
                str_it+=2;
                std::string unicode_str_2 = std::string("0x");
                for (int i = 0; i < 4 ; i++ ) {
                  if ( isalpha(*(str_it+i)) || isdigit(*(str_it+i))) {
                    continue;
                  }else {
                    throw std::runtime_error("parsing string type failed");
                  }
                }
                unicode_str_2.append(str_it, str_it+4);
                str_it+=4;
                unsigned hex_2;
                sscanf(unicode_str_2.c_str(), "%x", &hex_2);
                if ( 0xd800 < hex_2 && hex_2 < 0xdfff ) {
                  unsigned hex_high = hex&0x3ff;
                  unsigned hex_low = hex_2&0x3ff;
                  hex = ((hex_high << 10) | hex_low ) + 0x10000; 
                  //std::cout <<"QTQ " << std::hex <<hex<<" " <<hex_high<<" "<<hex_low <<std::endl;
                }else {
                  throw std::runtime_error("parsing string type failed");
                }
              }else {
                throw std::runtime_error("parsing string type failed");
              }
            }
            // Convert hex unsigned to utf-8
            std::string utf8_str = converter.to_bytes(hex);
            result.append(utf8_str);     
            break;
          }
          default:
            break;
        }
      // Ensure that end with '"'
      }else if ( *str_it == '"') {
        end_with_quote = true;
        str_it++;
        break;
      // Normal char, put in result string
      }else {
        result.append(str_it, str_it+1);
        str_it++;
      }
    }
  }else {
    throw std::runtime_error("parsing string type failed");
  }
  if ( end_with_quote ){
    return result;
  }else {
    throw std::runtime_error("parsing string type failed");
  }
}

bool ignore_space(std::string::const_iterator &str_it) {
  while ( *str_it != '\0') {
    if ( *str_it == ' ') {
      str_it++;
    }else {
      return true;
    }
  }
  return false;
}

JsonNode::array parse_array(std::string::const_iterator &str_it) {
  JsonNode::array result;
  // TODO: implement parsing and advance iterator
  // NOTE: we did not provide `parse_null` above, however `null` is one of the
  //   valid array values
  // for (; *str_it != '\0'; str_it++){
  //   std::cout << *str_it;
  // }
  // return result;
  bool end_with_square_bracket = false;
  if ( *str_it != '[') {
    throw std::runtime_error("parsing array type failed");
  }
  str_it++;

  JsonNode *value; 
  while ( *str_it != '\0' ) {
    ignore_space( str_it );
    if ( *str_it == '"'){
      std::string value_str = parse_string(str_it); 
      std::unique_ptr<std::string> value_unique_str = std::make_unique<std::string>(value_str);
      value = new JsonNode(std::move(value_unique_str));  
      result.push_back(*value);
      // std::cout <<"before="<< value_str << std::endl;
      // std::cout <<"before="<< value->type() << std::endl;
      // std::cout <<"after = "<< result[0].type()<<std::endl;
    }else if ( isdigit(*str_it) || *str_it == '-') {
      double value_number = parse_number(str_it);
      value = new JsonNode(value_number); 
      result.push_back(*value);
    }else if ( *str_it == 't' || *str_it == 'f') {
      bool value_bool = parse_bool(str_it);
      value = new JsonNode(value_bool);
      result.push_back(*value);  
    }else if ( *str_it == 'n') {
      str_it+=4;
      value = new JsonNode();
      result.push_back(*value);
    }else if ( *str_it == '[') {
      JsonNode::array value_array = parse_array(str_it);
      std::unique_ptr<JsonNode::array> value_array_ptr = std::make_unique<JsonNode::array>(value_array);
      value = new JsonNode(std::move(value_array_ptr));
      result.push_back(*value);
    }else if ( *str_it == '{') {
      JsonNode::object value_object = parse_object(str_it);
      std::unique_ptr<JsonNode::object> value_object_ptr = std::make_unique<JsonNode::object>(value_object);
      value = new JsonNode(std::move(value_object_ptr));
      result.push_back(*value);
    }else{
        throw std::runtime_error("parsing array type failed");
    }
    if ( *str_it == ',') {
      str_it++;
      continue;
    }else if ( *str_it == ']') {
      str_it++;
      end_with_square_bracket = true;
      break;
    }
  }
  if ( end_with_square_bracket ) {
    return result;
  }else {
    throw std::runtime_error("parsing array type failed");
  }
  
}

JsonNode::object parse_object(std::string::const_iterator &str_it) {
  JsonNode::object result;
  // TODO: implement parsing and advance iterator
  // NOTE: 1. we did not provide `parse_null` above, however `null` is one of the
  //          valid object values
  //       2. object key can be any kind of json string, which means it may
  //          contain spaces, escapes, unicode code-points, etc., so take care!
  bool end_with_brace = false;
  ignore_space( str_it );
  if ( *str_it != '{') {
    //std::cout <<*str_it<<*(str_it+1)<<*(str_it+2)<<std::endl;
    throw std::runtime_error("parsing object type1 failed");
  }
  //std::cout <<"QAQ "<<std::endl;
  str_it++;
  std::string key;
  JsonNode *value;
  while( *str_it != '\0') {
    ignore_space( str_it);
    // Deal with (string : value) pair
    if ( *str_it == '"') {
      key = parse_string(str_it);
    }else {
      throw std::runtime_error("parsing object type2 failed");
    }

    ignore_space( str_it );
    if (*str_it != ':') {
        throw std::runtime_error("parsing object type3 failed");
    }
    str_it++;

    ignore_space( str_it );
    // Deal with obj value
    if ( *str_it == '"') {
      std::string value_str = parse_string(str_it);
      std::unique_ptr<std::string> value_ptr = std::make_unique<std::string>(value_str);
      value = new JsonNode(std::move(value_ptr));
      result.insert(std::make_pair(key, *value));
    }else if ( *str_it == '{') {
      JsonNode::object value_obj = parse_object(str_it);
      std::unique_ptr<JsonNode::object> value_ptr = std::make_unique<JsonNode::object>(value_obj);
      value = new JsonNode(std::move(value_ptr));
      result.insert(std::make_pair(key, *value));
    }else if ( *str_it == '[') {
      JsonNode::array value_array =  parse_array(str_it);
      std::unique_ptr<JsonNode::array> value_ptr = std::make_unique<JsonNode::array>(value_array);
      value = new JsonNode(std::move(value_ptr));
      result.insert(std::make_pair(key, *value));
    }else if ( *str_it == 't' || *str_it == 'f') {
      bool value_bool = parse_bool(str_it);
      value = new JsonNode(value_bool);
      result.insert(std::make_pair(key, *value));
    }else if ( *str_it == 'n') {
      value = new JsonNode();
      result.insert(std::make_pair(key, *value));
    }else if ( *str_it == '-'
              || isdigit(*str_it) ){
      double value_number = parse_number(str_it);
      value = new JsonNode(value_number);
      result.insert(std::make_pair(key, *value));
    }else {
      throw std::runtime_error("parsing object type4 failed");
    }
    if (*str_it == '}') {
      str_it++;
      end_with_brace =true;
      break;
    }else if (*str_it == ',') {
      str_it++;
      continue;
    }else {
      throw std::runtime_error("parsing object type5 failed");
    }   
  }
  return result;
}

}
