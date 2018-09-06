#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <string>

#include "graph_parser.h"

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

void test_parse_NavNode() {
  garbage << "entering: " << __func__ << endl;
  NavNode result;
  string source;
  list<string> bad_sources = {
      "id\": 0, \"info\": \"node 0\" }",  "\"id: 0, \"info\": \"node 0\" }",
      "\"id\" 0, \"info\": \"node 0\" }", "\"id\": 0 \"info\": \"node 0\" }",
      "\"id\": 0, info\": \"node 0\" }",  "\"id\": 0, \"info: \"node 0\" }",
      "\"id\": 0, \"info\": node 0\" }",  "\"id\": 0, \"info\": \"node 0 }",
      "\"id\": 0, \"info\": node 0\" "};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    parse_NavNode(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;

  state.reset();
  source = "\"id\": 0, \"info\": \"node 0\" }";
  result = parse_NavNode(source, state);
  results << result.dump() << endl;
  if (state.error) {
    fail_test(__func__, source);
  }

  state.reset();
  source = "\"id\"  :  0  , \"info\" : \"no \n\n de 0\"   }";
  result = parse_NavNode(source, state);
  results << result.dump() << endl;
  if (state.error) {
    fail_test(__func__, source);
  }
}

void test_eat_Endpoints() {
  garbage << "entering: " << __func__ << endl;
  Endpoints result;
  string source;
  list<string> bad_sources = {"  [0,1]", "[0.0, 1]", "[a,b]", "0,1]", "[0,1"};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    eat_Endpoints(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;

  state.reset();
  source = "[0,1]";
  result = eat_Endpoints(source, state);
  results << result.dump() << endl;
  if (state.error || result != Endpoints(0, 1)) {
    fail_test(__func__, source);
  }

  state.reset();
  source = "[  0  ,  1  ]";
  result = eat_Endpoints(source, state);
  results << result.dump() << endl;
  if (state.error || result != Endpoints(0, 1)) {
    fail_test(__func__, source);
  }
}

void test_parse_Edge() {
  garbage << "entering: " << __func__ << endl;
  pair<Endpoints, EdgeInfo> result;
  string source;
  list<string> bad_sources = {"endpoints\": [0,1], \"info\": \"0 to 1\" }",
                              "\"endpoints: [0,1], \"info\": \"0 to 1\" }",
                              "\"endpoints: [0,1] \"info\": \"0 to 1\" }",
                              "\"endpoints\": 0,1], \"info\": \"0 to 1\" }",
                              "\"endpoints\": [0,1, \"info\": \"0 to 1\" }",
                              "\"endpoints\": [0,1] info\": \"0 to 1\" }",
                              "\"endpoints\": [0,1], \"info: \"0 to 1\" }",
                              "\"endpoints\": [0,1], \"info\" \"0 to 1\" }",
                              "\"endpoints\": [0,1], \"info\": 0 to 1\" }",
                              "\"endpoints\": [0,1], \"info\": \"0 to 1 }",
                              "\"endpoints\": [0,1], \"info\": \"0 to 1\" "};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: " << source << endl;
    state.reset();
    parse_Edge(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;

  state.reset();
  source = "\"endpoints\": [0,1], \"info\": \"0 to 1\" }";
  result = parse_Edge(source, state);
  results << "{" << result.first.dump() << ", " << result.second.dump() << "}"
          << endl;
  if (state.error) {
    fail_test(__func__, source);
  }
}

list<string> read_graph_sources(const string &fname) {
  ifstream source_file(fname);
  list<string> bad_sources;
  string line;
  string cur;

  while (source_file.good()) {
    cur = "";
    // look for --- (beginning)
    getline(source_file, line);
    if (line == "---") {
      getline(source_file, line);
      while (line != "---" && source_file.good()) {
        cur += line + "\n";
        getline(source_file, line);
      }
      bad_sources.push_back(move(cur));
    }
  }
  return bad_sources;
}

void test_parse_UnderlyingGraph() {
  garbage << "entering: " << __func__ << endl;
  pair<Endpoints, EdgeInfo> result;
  list<string> bad_sources =
      move(read_graph_sources("test_files/parser_testing/bad_UG.test_file"));
  list<string> good_sources =
      move(read_graph_sources("test_files/parser_testing/good_UG.test_file"));
  string source;

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    garbage << "failtesting: \n" << source << endl;
    state.reset();
    parse_UnderlyingGraph(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;

  state.reset();
  source = "\"endpoints\": [0,1], \"info\": \"0 to 1\" }";
  result = parse_Edge(source, state);
  results << "{" << result.first.dump() << ", " << result.second.dump() << "}"
          << endl;
  if (state.error) {
    fail_test(__func__, source);
  }
}

void test_parse_UnderlyingGraphFromFile() {
  garbage << "entering: " << __func__ << endl;
  UnderlyingGraph result;
  list<string> bad_sources = {
      "test_files/parser_testing/bad_UGFromFile1.test_file", "test_files/parser_testing/bad_UGFromFile2.test_file",
      "test_files/parser_testing/bad_UGFromFile3.test_file", "test_files/parser_testing/bad_UGFromFile4.test_file"};

  list<string> good_sources = {"test_files/parser_testing/good_UGFromFile.test_file"};

  //"testing bad sources"
  for (auto &&source : bad_sources) {
    state.reset();
    garbage << "failtesting: \n" << source << endl;
    parse_UnderlyingGraphFromFile(source, state);
    if (!state.error) {
      fail_test(__func__, source);
    }
  }

  //"testing good sources"
  results << "results for: " << __func__ << endl;

  for (auto &&source : good_sources) {
    state.reset();
    garbage << "success testing: \n" << source << endl;
    result = parse_UnderlyingGraphFromFile(source, state);
    results << result.dump() << endl;
    if (state.error) {
      fail_test(__func__, source);
    }
  }
}

list<void (*)(void)> test_list = {test_parse_NavNode, test_eat_Endpoints,
                                  test_parse_Edge, test_parse_UnderlyingGraph,
                                  test_parse_UnderlyingGraphFromFile};

int main(int argc, char **argv) {

  for (auto &&test : test_list) {
    test();
  }

  cout << "testing complete" << endl;
}
