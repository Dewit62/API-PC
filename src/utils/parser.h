#ifndef PARSER_H_
#define PARSER_H_

#include <memory>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "identifier_register.h"
#include "statements/expression.h"
#include "statements/expression_arithmetic.h"
#include "statements/expression_assignment.h"
#include "statements/expression_ppid.h"
#include "statements/instruction.h"
#include "utils/reader.h"
#include "utils/status.h"
#include "variable_id_allocator.h"

// Using YAML grammar and reader constructs instructions.
class Parser {
 public:
  // Provide a grammar to specify language parsed and generated by fluff.
  Parser(const YAML::Node& grammar);

  // Reads input using reader and tries to parse a single new instruction.
  // If this succeseeds, an Ok status is returned and a new instruction
  // is inserted into the vector. Otherwise, an errorneus status is returned and
  // the vector remains unchanged.
  Status GetInstruction(Reader& reader,
                        std::vector<std::unique_ptr<Instruction>>* instruction,
                        IdentifierRegister* identifier_register,
                        int max_recursion_depth = 50) const;

  // Sets parser to pure mode which prevents corruptions from happening.
  void SetPureRun();

 private:
  // Reads input using reader and constructs a new Expression.
  Status GetExpression(Reader& reader, std::unique_ptr<Expression>* expression,
                       VariableIdAllocator* allocator,
                       IdentifierRegister* identifier_register,
                       int max_recursion_depth) const;

  // Given an instruction name (e.x. "Addition") constructs this instruction.
  // Uses reader to construct the instruction as described by input file.
  Status GetInstructionClosure(const std::string& operation, Reader& reader,
                               std::unique_ptr<Instruction>* instruction,
                               VariableIdAllocator* allocator,
                               IdentifierRegister* identifier_register,
                               int max_recursion_depth) const;

  Status GetBinaryOperatorOperands(ExpressionArithmetic* expression,
                                   Reader& reader,
                                   VariableIdAllocator* allocator,
                                   IdentifierRegister* identifier_register,
                                   int max_recursion_depth) const;

  Status GetAssignmentOperands(IdentifierRegister* identifier_register,
                               ExpressionAssignment* assignment, Reader& reader,
                               VariableIdAllocator* allocator,
                               int max_recursion_depth) const;

  Status CorruptStatement(Reader& reader,
                          std::unique_ptr<Instruction>* instruction) const;

  Status GetVariablePPID(Reader& reader,
                         IdentifierRegister* identifier_register,
                         ExpressionPPID* ppid) const;

  YAML::Node grammar_;
  bool pure_run_ = false;
};

#endif  // ifndef PARSER_H_
