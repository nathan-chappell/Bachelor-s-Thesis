// parse_functions.h
#pragma once

#include "common_graph.h"

#include <functional>
#include <iostream>
#include <string>

namespace parse_functions {

/*
 * The test state is used for parsing operations and not testing purposes.  It
 * is used to silence the error messages when checking for the ability to
 * successfully parse an item.
 */
struct parse_state {
  size_t line;
  size_t col;
  size_t source_pos;
  bool error;
  bool end;
  bool test;
  // mutable pointer used for the operator= to work
  mutable std::ostream *error_log;

  parse_state(std::ostream *o = &std::cout) : error_log(o) { reset(); }

  bool good() const { return (!error && !end); }

  std::string position() const {
    return "line: " + std::to_string(line) + ", col: " + std::to_string(col);
  }

  void reset() {
    line = 0;
    col = 0;
    source_pos = 0;
    error = false;
    end = false;
    test = false;
  }
};

void advance_state(const std::string &source, parse_state &state);

// abstract function for eating while predicate holds
std::string eat_while(const std::string &source, parse_state &state,
                      std::function<bool(char)> predicate);

std::string eat_ws(const std::string &source, parse_state &state);

// give a useful error message
void parse_error(const std::string &expected, parse_state &state);

void check_for_char(const std::string &source, parse_state &state, char c);

// eat some yummy tokens
std::string eat_alnum_string(const std::string &source, parse_state &state);

std::string eat_simple_attribute_declaration(const std::string &source,
                                             parse_state &state);

std::string eat_compound_attribute_declaration(const std::string &source,
                                               parse_state &state);

// look-ahead functions
bool verify_compound_attribute_declaration(const std::string &source,
                                           parse_state &state);

bool verify_simple_attribute_declaration(const std::string &source,
                                         parse_state &state);

bool peak_for(const std::string &source, parse_state &state,
              std::function<bool(char)> predicate);

bool peak_for(const std::string &source, parse_state &state, char c);

// value parsing functions
double eat_double(const std::string &source, parse_state &state);

C eat_C(const std::string &source, parse_state &state);

Grid eat_Grid(const std::string &source, parse_state &state);

unsigned eat_unsigned(const std::string &source, parse_state &state);
int eat_int(const std::string &source, parse_state &state);

std::string eat_escaped_string(const std::string &source, parse_state &state);

}; // namespace parse_functions
