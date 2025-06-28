#pragma once
#include "core/common/imports/_all.hpp"


namespace csopesy {
  enum class Opcode {
    Print,
    Declare,
    Add,
    Subtract,
    Sleep,
    For
  };

  /**
   * @brief Represents a single instruction in a process program.
   * 
   * Each instruction consists of:
   * - An Opcode defining what the instruction does.
   * - A list of string arguments, interpreted differently per OpCode.
   * - (Optional) a nested body of instructions, used only by FOR.
   */
  struct Instruction {
    using arg_list = vector<str>;
    using ins_list = vector<Instruction>;

    Opcode opcode;
    arg_list args;
    ins_list body; // used only for FOR loops

    Instruction(Opcode opcode, arg_list args={}, ins_list body={}): 
      opcode(opcode), args(move(args)), body(move(body)) {}

    static Instruction Print(str msg) {
      return { Opcode::Print, {move(msg)} };
    }

    static Instruction Declare(str var, str value) {
      return { Opcode::Declare, {move(var), move(value)} };
    }

    static Instruction Add(str target, str left, str right) {
      return { Opcode::Add, {move(target), move(left), move(right)} };
    }

    static Instruction Subtract(str target, str left, str right) {
      return { Opcode::Subtract, {move(target), move(left), move(right)} };
    }

    static Instruction Sleep(str ticks) {
      return { Opcode::Sleep, {move(ticks)} };
    }

    static Instruction For(vector<Instruction> body, str repeat_count) {
      return { Opcode::For, {move(repeat_count)}, move(body) };
    }
  };
}
