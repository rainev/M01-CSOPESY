#pragma once
#include "core/instruction/handlers/_all.hpp"
#include "core/process/Process.hpp"
#include "types.hpp"

namespace csopesy {

  /** Central registry and dispatcher for all instruction types. */
  class InstructionInterpreter {
    using map  = unordered_map<str, InstructionHandler>;
    using list = vector<ref<const InstructionHandler>>;

    // Registry of available instruction handlers
    map handlers;  

    public:

    InstructionInterpreter() {
      for (auto& inst: instruction::get_all()) {
        register_instruction(move(inst));
      }
    }

    /** Registers a new instruction handler using its internal name. */
    void register_instruction(InstructionHandler handler) {
      handlers[handler.opcode] = move(handler);
    }

    /** Executes a single instruction using the mapped handler. */
    void execute(const Instruction& inst, Process& proc) const {
      auto it = handlers.find(inst.opcode);
      if (it == handlers.end())
        throw runtime_error("Unknown instruction: " + inst.opcode);

      it->second.execute(proc, inst);
    }

    /** Returns a list of all registered instruction handlers by const reference. */
    list get_handlers() const {
      list result; 
      result.reserve(handlers.size());
      for (const auto& pair: handlers)
        result.push_back(cref(pair.second));
      return result;
    }
  };

  inline InstructionInterpreter inst_interpreter;
}
