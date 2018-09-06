// clang_machine.cc

#include "clang_machine.h"
#include "clang_predicates.h"

using namespace std;

#include <iostream>

CXChildVisitResult call_graph_visitor_function(CXCursor child, CXCursor,
                                               CXClientData data) {
  Clang_Call_Grapher *machine = (Clang_Call_Grapher *)data;
  machine->state = 1;
  //cout << "visiting: " << get_usr(child) << endl;
  if (is_clang_function_defintion(child)) {
    // push onto stack
    string usr = get_usr(child);
    string disp_name = get_display_name(child);

    machine->marshaller.add_node(usr, disp_name);
    machine->stack.push_back(usr);

    // descend recursively, looking for callees
    clang_visitChildren(child, call_graph_visitor, data);

    // pop stack, check state
    machine->stack.pop_back();
    if (machine->state == -1) {
      return CXChildVisit_Break;
    } else {
      return CXChildVisit_Continue;
    }
  } else if (is_clang_function_call(child)) {
    if (machine->stack.empty()) {
      machine->state = -1;
      return CXChildVisit_Break;
    } else {
      string usr = get_usr(clang_getCursorDefinition(child));
      machine->marshaller.add_edge(machine->stack.back(), usr, "calls");
      return CXChildVisit_Recurse;
    }
  } else {
    return CXChildVisit_Recurse;
  }
}

string Clang_Call_Grapher::dump_stack() const {
  string result;
  for (auto &&usr : stack) {
    result += usr + "\n";
  }
  return result;
}

string Clang_Call_Grapher::cur_state_description() const {
  string result;
  switch (state) {
  case 0: {
    result = "initialization";
  } break;
  case 1: {
    result = "descending";
  } break;
  case -1: {
    result = "error";
  } break;
  default: { result = "unknown state!"; }
  }

  result += "\nstack size: " + ::to_string(stack.size());
  return result;
}

CXCursorVisitor call_graph_visitor = &call_graph_visitor_function;
