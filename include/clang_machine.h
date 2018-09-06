// clang_machine.h
#pragma once

#include "../include/marshaller.h"

#include "clang-c/Index.h"

#include <list>
#include <string>

using USR = std::string;
using MachinePair = std::pair<CXCursorVisitor, void *>;

extern CXCursorVisitor call_graph_visitor;

class Clang_Call_Grapher {
public:
  std::string dump_stack() const;
  std::string cur_state_description() const;

  //friend CXCursorVisitor call_graph_visitor;

//private:
  int state;
  std::list<USR> stack;
  Marshaller marshaller;
};
