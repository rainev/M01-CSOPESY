#pragma once
#include "core/common/imports/_all.hpp"
#include "core/process/Process.hpp"
#include "Instruction.hpp"

namespace csopesy {
  struct InstructionInterpreter {
    static void execute(Process& process, const Instruction& instr) {
      switch (instr.opcode) {

        case Opcode::Print: {
          const str& msg     = instr.args[0];
          const int core_id  = process.get_core();
          const auto now     = process.get_start_time();

          auto time_string = timestamp(now);

          auto out = ofstream(format("logs/{}.txt", process.get_name()), ios::app);
          out << format("{} Core:{} \"{}\"\n", time_string, core_id, msg);
          out.close();

          process.log(msg); // For internal viewing via process-smi
          return;
        }

        case Opcode::Declare: {
          uint value = cast_uint(instr.args[1]);
          process.get_memory().set(instr.args[0], value);
          return;
        }

        case Opcode::Add: {
          uint lhs = process.get_memory().get(instr.args[1]);
          uint rhs = process.get_memory().resolve(instr.args[2]);
          process.get_memory().set(instr.args[0], lhs + rhs);
          return;
        }

        case Opcode::Subtract: {
          uint lhs = process.get_memory().get(instr.args[1]);
          uint rhs = process.get_memory().resolve(instr.args[2]);
          process.get_memory().set(instr.args[0], lhs - rhs);
          return;
        }

        case Opcode::Sleep: {
          uint duration = cast_uint(instr.args[0]);
          process.get_state().sleep_for(duration);
          return;
        }

        case Opcode::For: {
          uint count = cast_uint(instr.args[0]);
          for (uint i = 0; i < count; ++i)
            for (const auto& sub : instr.body)
              execute(process, sub);
          return;
        }
      }
    }
  };
}
