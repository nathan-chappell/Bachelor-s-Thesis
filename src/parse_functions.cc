// parse_primitives.cc

#include "parse_functions.h"

using namespace std;

namespace parse_functions {
/*
C parse_Position(const string &source, parse_state &state) {}
Grid parse_Grid(const string &source, parse_state &state) {}
NodeId parse_NodeId(const string &source, parse_state &state) {}
string parse_Info(const string &source, parse_state &state) {}
*/

void advance_state(const string &source, parse_state &state) {
  if (state.end) {
    state.error = true;
    return;
  }
  if ((source[state.source_pos]) == '\n') {
    state.col = 0;
    ++state.line;
  } else {
    ++state.col;
  }
  ++state.source_pos;
  if (state.source_pos == source.length())
    state.end = true;
}

// abstract function
string eat_while(const string &source, parse_state &state,
                 function<bool(char)> predicate) {
  string return_string;
  while (state.good()) {
    if (predicate(source[state.source_pos])) {
      return_string += source[state.source_pos];
      advance_state(source, state);
    } else {
      break;
    }
  }
  return return_string;
}

string eat_ws(const string &source, parse_state &state) {
  return eat_while(source, state, ::isspace);
}

void parse_error(const string &expected, parse_state &state) {
  if (state.test) {
    return;
  }
  state.error = true;
  *state.error_log << "parse_error: expected: \"" << expected
                   << "\" at: " << state.position() << endl;
}

void check_for_char(const string &source, parse_state &state, char c) {
  if (state.good() && source[state.source_pos] == c) {
    advance_state(source, state);
  } else {
    char cstr[2] = {c, 0};
    parse_error(cstr, state);
  }
}

string eat_alnum_string(const string &source, parse_state &state) {
  return eat_while(source, state, ::isalnum);
}

string eat_simple_attribute_declaration(const string &source,
                                        parse_state &state) {
  string return_string;
  check_for_char(source, state, '"');
  return_string = eat_alnum_string(source, state);
  if (return_string == "") {
    parse_error("simple_attribute_declaration", state);
  }
  check_for_char(source, state, '"');
  eat_ws(source, state);
  check_for_char(source, state, ':');
  return return_string;
}

string eat_compound_attribute_declaration(const string &source,
                                          parse_state &state) {
  string return_string;
  return_string = eat_simple_attribute_declaration(source, state);
  eat_ws(source, state);
  check_for_char(source, state, '{');
  return return_string;
}

bool verify_compound_attribute_declaration(const string &source,
                                           parse_state &state) {
  /*
   * True if you can eat it.  Resets state information.
   */
  parse_state original_state = state;
  eat_compound_attribute_declaration(source, state);
  bool verified = state.good();
  state = original_state;
  return verified;
}

bool verify_simple_attribute_declaration(const string &source,
                                         parse_state &state) {
  /*
   * True if a simple attribute declaration and NOT a compound attribute
   * declaration.
   */
  parse_state original_state = state;
  bool verified = true;

  eat_simple_attribute_declaration(source, state);
  verified = verified && state.good();
  state = original_state;

  eat_compound_attribute_declaration(source, state);
  verified = verified && !state.good();
  state = original_state;

  return verified;
}

bool peak_for(const string &source, parse_state &state,
              function<bool(char)> predicate) {
  if (state.good() && predicate(source[state.source_pos])) {
    return true;
  }
  return false;
}

bool peak_for(const string &source, parse_state &state, char c) {
  if (state.good() && source[state.source_pos] == c) {
    return true;
  }
  return false;
}

pair<double, bool> eat_integral(const string &source, parse_state &state) {
  double negative = 1.0;
  bool read_integral = false;
  double integral = 0;

  if (peak_for(source, state, '-')) {
    negative = -1.0;
    advance_state(source, state);
  }
  if (peak_for(source, state, ::isdigit)) {
    // has whole part
    read_integral = true;
    do {
      integral *= 10;
      integral += source[state.source_pos] - '0';
      advance_state(source, state);
    } while (state.good() && peak_for(source, state, ::isdigit));
  }
  return make_pair(integral * negative, read_integral);
}

double eat_double(const string &source, parse_state &state) {
  double upper_digits = 0;
  double lower_digits = 0;
  double negative = 1;

  bool has_whole_part = false;
  bool has_decimal_part = false;

  if (peak_for(source, state, '-')) {
    // negative
    negative = -1;
    advance_state(source, state);
  }

  pair<double, bool> upper_result = eat_integral(source, state);

  upper_digits = upper_result.first;
  has_whole_part = upper_result.second;

  if (peak_for(source, state, '.')) {
    // has decimal part
    has_decimal_part = true;
    advance_state(source, state);
    if (!peak_for(source, state, ::isdigit)) {
      parse_error("[0-9]", state);
      return numeric_limits<double>::quiet_NaN();
    } else {
      double factor = .1;
      while (peak_for(source, state, ::isdigit)) {
        lower_digits += (source[state.source_pos] - '0') * factor;
        advance_state(source, state);
        factor *= .1;
      }
    }
  }

  if (!has_whole_part && !has_decimal_part) {
    parse_error("double", state);
    return numeric_limits<double>::quiet_NaN();
  } else {
    return negative * (upper_digits + lower_digits);
  }
}

/*
 * Refactor candidate:
 * eat_complex_type<T>...
 *
 * More complicated than it looks, parsing T won't be straightforward..
 */

C eat_C(const string &source, parse_state &state) {
  double r, i; // r - real, i - imaginary
  r = eat_double(source, state);
  eat_ws(source, state);
  check_for_char(source, state, ',');
  eat_ws(source, state);
  i = eat_double(source, state);
  /*
   * Note: this used to check for state.good(), but this check return false at
   * the end of the input.  While this may be okay for parsing, since this
   * shouldn't ever happen, it became a bit surprising (i.e. confusing) during
   * testing, so it is the way it is now because it seems to be more natural.
   */
  if (!state.error) {
    return C(r, i);
  } else {
    *state.error_log << "error state while eating C" << endl;
    double nan = numeric_limits<double>::quiet_NaN();
    return C(nan, nan);
  }
}

// Completely analagous to eat_C
Grid eat_Grid(const std::string &source, parse_state &state) {
  int r, i;
  r = eat_int(source, state);
  eat_ws(source, state);
  check_for_char(source, state, ',');
  eat_ws(source, state);
  i = eat_int(source, state);

  //
  if (!state.error) {
    return Grid(r, i);
  } else {
    *state.error_log << "error state while eating Grid" << endl;
    double nan = numeric_limits<double>::quiet_NaN();
    return Grid(nan, nan);
  }
}

template <typename T>
T eat_integral_type(const string &source, parse_state &state,
                    const string &type_name) {
  pair<double, bool> integral_res = eat_integral(source, state);

  if (!integral_res.second) {
    parse_error(type_name, state);
    return numeric_limits<T>::max();
  }

  if (integral_res.first > numeric_limits<T>::max()) {
    *state.error_log << "detected a value that is too large!" << endl;
    parse_error(type_name, state);
    return numeric_limits<T>::max();
  }

  if (integral_res.first < numeric_limits<T>::min()) {
    *state.error_log << "detected a value that is too small!" << endl;
    parse_error(type_name, state);
    return numeric_limits<T>::min();
  }

  return (T)integral_res.first;
}

/*
 * Refactor candidate:
 * eat_integral_type<T> ...
 */

unsigned eat_unsigned(const string &source, parse_state &state) {
  return eat_integral_type<unsigned>(source, state, "unsigned");
}

// Analagous to eat_unsigned
int eat_int(const string &source, parse_state &state) {
  return eat_integral_type<int>(source, state, "int");
}

/*
 * The strings to be eaten here will be eaten character by character, delimited
 * by quotation marks.  The only exceptional states are escaping with the
 * backslash.  This causes the next character to be added to the result string,
 * allowing backslashes and quotation marks to appear there.
 */
string eat_escaped_string(const string &source, parse_state &state) {
  bool escaped = false;
  char cur;
  string result;

  check_for_char(source, state, '"');
  while (state.good()) {
    cur = source[state.source_pos];
    if (!escaped) {
      if (cur == '\\') {
        escaped = true;
        advance_state(source, state);
        continue;
      } else if (cur == '"') {
        advance_state(source, state);
        break;
      }
    } else {
      escaped = false;
    }
    result.push_back(cur);
    advance_state(source, state);
  }
  if (cur != '"') {
    parse_error("\"", state);
  }
  return result;
}

}; // namespace parse_functions
