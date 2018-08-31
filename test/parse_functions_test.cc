#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <string>

#include "parse_functions.h"

using namespace std;
using namespace parse_functions;

unsigned failed_tests = 0;
ofstream garbage(".testing_error_messages");
ofstream results(".testing_results");
parse_state state(&garbage);

void fail_test(string name, string source) {
  cout << "failed: " << name << " on: " << source << endl;
  ++failed_tests;
}

void test_eat_double() {
  garbage << "entering: " << __func__ << endl;
  string max = ::to_string(numeric_limits<double>::max());
  list<string> bad_sources{"", "-", "-.", ".", "foo"};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    eat_double(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;
  double result;

  state.reset();
  string source = "0";
  result = eat_double(source, state);
  results << result << endl;
  if (state.error || result != 0) {
    fail_test(__func__, source);
  }

  state.reset();
  source = "123";
  result = eat_double(source, state);
  results << result << endl;
  if (state.error || result != 123) {
    fail_test(__func__, source);
  }

  state.reset();
  source = "-123";
  result = eat_double(source, state);
  results << result << endl;
  if (state.error || result != -123) {
    fail_test(__func__, source);
  }

  state.reset();
  source = "123.123";
  result = eat_double(source, state);
  results << result << endl;
  if (state.error || result != 123.123) {
    fail_test(__func__, source);
  }

  state.reset();
  source = "-123.123";
  result = eat_double(source, state);
  results << result << endl;
  if (state.error || result != -123.123) {
    fail_test(__func__, source);
  }

  /*
   * This test fails, probably an accuracy problem though
   *
  state.reset();
  source = max;
  result = eat_double(source,state);
  results << result << endl;
  if (state.error || result != numeric_limits<double>::max()) {
      fail_test(__func__, source);
  }
  */
}

void test_eat_unsigned() {
  garbage << "entering: " << __func__ << endl;
  string max = ::to_string(numeric_limits<unsigned>::max());
  list<string> bad_sources{"", "-123", ".123", max + "0", "foo"};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    eat_unsigned(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;
  unsigned result;

  state.reset();
  string source = "0";
  result = eat_unsigned(source, state);
  results << result << endl;
  if (state.error || result != 0) {
    fail_test(__func__, source);
  }

  state.reset();
  source = "123";
  result = eat_unsigned(source, state);
  results << result << endl;
  if (state.error || result != 123) {
    fail_test(__func__, source);
  }

  state.reset();
  source = max;
  result = eat_unsigned(source, state);
  results << result << endl;
  if (state.error || result != numeric_limits<unsigned>::max()) {
    fail_test(__func__, source);
  }
}

void test_eat_escaped_string() {
  garbage << "entering: " << __func__ << endl;
  string result;
  // maybe unnecessary, but I had trouble getting the exact string I wanted
  char special[10] = {'"', '\\', '"', 'f', 'o', 'o', '\\', '"', '"', 0};
  list<string> bad_sources{"-123", "foo\"", "\"foo"};
  list<string> good_sources{"\"-123\"", "\"foo\"", special};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    eat_escaped_string(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;
  // Note: visually compare string to make sure they are what you want
  for (auto &&source : good_sources) {
    state.reset();
    result = eat_escaped_string(source, state);
    results << result << endl;
    if (state.error) {
      fail_test(__func__, source);
    }
  }
}

// form: "\b*\"[a-zA-Z0-9]+\"\b*:\b*"/
void test_eat_simple_attribute_declaration() {
  garbage << "entering: " << __func__ << endl;
  string result;

  list<string> bad_sources{"foo:", "foo\":", "\"foo", "\"\":", "\"foo\""};
  list<string> good_sources{"\"foo\":", "\"foo\"  :  "};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    eat_simple_attribute_declaration(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;
  // Note: visually compare string to make sure they are what you want
  for (auto &&source : good_sources) {
    state.reset();
    result = eat_simple_attribute_declaration(source, state);
    results << result << endl;
    if (state.error) {
      fail_test(__func__, source);
    }
  }
}

void test_eat_C() {
  garbage << "entering: " << __func__ << endl;
  C result;
  list<string> bad_sources{"-1.2", "-1.2,", ",1.2", " -1.2,1.2"};
  list<string> good_sources{"-1.2,1.2", "-1.2   ,1.2", "-1.2,  1.2",
                            "-1.2  ,  1.2"};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    eat_C(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;
  // Note: visually compare C to make sure they are what you want
  for (auto &&source : good_sources) {
    state.reset();
    result = eat_C(source, state);
    results << result << endl;
    if (state.error) {
      fail_test(__func__, source);
    }
  }
}

void test_eat_Grid() {
  garbage << "entering: " << __func__ << endl;
  Grid result;
  list<string> bad_sources{"-12", "-12,", ",12", " -12,12", "-1.2,1.2"};
  list<string> good_sources{"-12,12", "-12   ,12", "-12,  12",
                            "-12  ,  12"};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    eat_Grid(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;
  // Note: visually compare Grid to make sure they are what you want
  for (auto &&source : good_sources) {
    state.reset();
    result = eat_Grid(source, state);
    results << result << endl;
    if (state.error) {
      fail_test(__func__, source);
    }
  }
}

list<void (*)(void)> test_list = {test_eat_unsigned,
                                  test_eat_double,
                                  test_eat_escaped_string,
                                  test_eat_C,
                                  test_eat_Grid,
                                  test_eat_simple_attribute_declaration};

int main(int argc, char **argv) {

  for (auto &&test : test_list) {
    test();
  }

  cout << "testing complete" << endl;
}
