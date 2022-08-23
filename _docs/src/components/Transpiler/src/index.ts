/*
  DON'T LOOK AT ME, I AM OUGLY
*/

import {
  Instruction,
  PendingDirReferencedInternalLabel,
  PendingDirReferencedVariable,
  Register,
  Variable,
} from "./lib/instruction";
import { formatHexValue, parseString } from "./lib/number";
import { Label } from "./lib/instruction";
const asignationRegex = /(^\S+)\s=\s([^;]+)/gm;
const showRegex = /show[^\w]+([^;]+)/gm;
const labelRegex = /^(?!if)([^:|]+):[^\w]+(.+)/gm;
const jmpRegex = /^jmp[^\w]+([^;]+)/gm;
const conditionRegex = /if[^(]+\(([^)]+)\):[^\w]+(.+)/gm;

const forbiddenVariableNamePrefix = /^[0-9](d|x|b)?/;

const ADD_ARITHMETICAL_OPERATOR = "+";
const SUB_ARITHMETICAL_OPERATOR = "-";

const CHAR_LINE_END = ";";

const VALUE_BITS = 8;
const ADDRESS_BITS = 16;

class LePaEv {
  lineIndex = 0;
  variables: Variable[] = [];
  labels: Label[] = [];

  instructions: Instruction[] = [];

  constructor(
    public code: string,
    public initialAddress: number,
    public isParent = true
  ) {}

  import(parser: LePaEv) {
    this.variables = [...parser.variables];
    this.labels = [...parser.labels];
  }

  syncWith(parser: LePaEv, { variables = true, instructions = true }) {
    if (variables) {
      this.variables.push(...parser.variables);
    }

    if (instructions) {
      this.instructions.push(...parser.instructions);
    }
  }

  getAllocated() {
    return this.instructions
      .map((i) => i.getCost())
      .reduce((partialSum, a) => partialSum + a, 0);
  }

  prepare(allocateFin = true) {
    const pendingInstructionDirs = this.instructions.filter(
      (i) => i.dir === "pending-assignation"
    );
    if (pendingInstructionDirs.length === 0) return;
    let nextFreeAddress =
      this.initialAddress + this.getAllocated() + Number(allocateFin); // Number(fin) +1 or +0 for the FIN instruction

    if (
      nextFreeAddress + pendingInstructionDirs.length >
      Math.pow(2, ADDRESS_BITS) - 1
    )
      throw new Error("No space left in memory");

    pendingInstructionDirs.forEach((pid) => {
      // check if already allocated an address in this loop before
      const assignatedBefore = pendingInstructionDirs.find(
        (pid2) =>
          pid2.variable?.name === pid.variable?.name &&
          typeof pid2.dir === "number"
      );
      pid.dir = assignatedBefore
        ? Number(assignatedBefore.dir)
        : nextFreeAddress++; // Number(assigned.dir) to avoid typescript from annoying
      if (!assignatedBefore && pid.variable && typeof pid.dir === "number") {
        pid.variable.address = pid.dir;
      }
    });

    const pendingInstructionsVariables = this.instructions.filter((i) =>
      this.variables.find((v) => {
        return (
          i.dir instanceof PendingDirReferencedVariable &&
          v.name === i.dir.varName
        );
      })
    );

    pendingInstructionsVariables.forEach((piv) => {
      const variable = this.variables.find(
        (v) =>
          piv.dir instanceof PendingDirReferencedVariable &&
          v.name === piv.dir.varName
      );
      piv.dir = variable?.address;
    });

    if (allocateFin) {
      this.instructions.push(new Instruction("FIN", {}));
    }
  }

  preParseLabels() {
    let lineIndex = 0;
    const lines = this.code.split("\n");
    while (lineIndex < lines.length) {
      const line = lines[lineIndex];

      const labelMatch = new RegExp(labelRegex).exec(line);
      if (labelMatch && labelMatch.length > 1) {
        this.preParseLabel(labelMatch[1]);
        lineIndex++;
        continue;
      }

      lineIndex++;
    }
  }

  parse() {
    this.preParseLabels(); // find all the labels

    const lines = this.code.split("\n");
    while (this.lineIndex < lines.length) {
      const line = lines[this.lineIndex];
      if (line.length === 0) {
        this.lineIndex++;
        continue;
      }

      try {
        if (line && !line.endsWith(CHAR_LINE_END))
          throw new Error(`Expected ${CHAR_LINE_END} at col ${line.length}`);

        const labelMatch = new RegExp(labelRegex).exec(line);
        if (labelMatch && labelMatch.length > 1) {
          this.parseLabel(labelMatch[1], labelMatch[2]);
          this.lineIndex++;
          continue;
        }

        const conditionMatch = new RegExp(conditionRegex).exec(line);
        if (conditionMatch && conditionMatch.length > 1) {
          this.parseCondition(conditionMatch[1], conditionMatch[2]);
          this.lineIndex++;
          continue;
        }

        const asignationMatch = new RegExp(asignationRegex).exec(line);
        if (asignationMatch && asignationMatch.length > 1) {
          this.parseAsignation(asignationMatch[1], asignationMatch[2]);
          this.lineIndex++;
          continue;
        }

        const showMatch = new RegExp(showRegex).exec(line);
        if (showMatch && showMatch.length > 1) {
          this.parseShow(showMatch[1]);
          this.lineIndex++;
          continue;
        }

        const jmpMatch = new RegExp(jmpRegex).exec(line);
        if (jmpMatch && jmpMatch.length > 1) {
          this.parseJmp(jmpMatch[1]);
          this.lineIndex++;
          continue;
        }

        throw new Error(`Line could not be parsed`);
      } catch (error) {
        if (this.isParent) {
          if (error instanceof Error)
            throw new Error(`Line ${this.lineIndex + 1}: ${error.message}`);
        }
        throw error;
      }
    }
  }

  parseFn(functionName: string) {
    throw new Error("Not implemented");
  }

  parseAsignationInmediateSource(source: string): Instruction[] {
    const _instructions: Instruction[] = [];

    const num = parseString(source);
    if (num !== null && num > Math.pow(2, VALUE_BITS) - 1) {
      throw new Error(
        `Value ${formatHexValue(num)} overflows ${VALUE_BITS} value bits`
      );
    }

    if (num !== null) {
      const __instructions = [
        new Instruction("MOV", {
          value: num,
          destination: "AC",
        }),
      ];

      _instructions.push(...__instructions);
    }

    return _instructions;
  }

  parseAsignationOtherSource(source: string): Instruction[] {
    const _instructions: Instruction[] = [];
    const sourceIsArithmeticalOperation = /([^\s]+)[^\w]+(\+|-)[^\w]+(.+)/gm;
    const sourceIsVariable = new RegExp(
      `(${this.variables.map((v) => v.name).join("|")})$`,
      "gm"
    );

    const arithmeticalMatch = sourceIsArithmeticalOperation.exec(source);
    const variableMatch = sourceIsVariable.exec(source);

    if (
      arithmeticalMatch &&
      arithmeticalMatch.length > 1 &&
      arithmeticalMatch[1] &&
      arithmeticalMatch[2] &&
      arithmeticalMatch[3]
    ) {
      const a = arithmeticalMatch[1];
      const op = arithmeticalMatch[2];
      const b = arithmeticalMatch[3];

      const parsedNumA = parseString(a);
      const parsedNumB = parseString(b);

      const variableA = this.variables.find((v) => v.name === a);
      const variableB = this.variables.find((v) => v.name === b);

      if (parsedNumA === null && !variableA) {
        throw new Error(
          `Arithmetical operand (variable or number) ${a} is not valid`
        );
      }

      if (parsedNumB === null && !variableB) {
        throw new Error(
          `Arithmetical operand (variable or number) ${b} is not valid`
        );
      }

      if (op === ADD_ARITHMETICAL_OPERATOR) {
        const __instructions = [];
        if (parsedNumA !== null && __instructions.length === 0) {
          __instructions.push(
            new Instruction("MOV", {
              destination: "AC",
              value: parsedNumA,
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (parsedNumA !== null && __instructions.length > 0) {
          __instructions.push(
            new Instruction("ADD", {
              value: parsedNumA,
            })
          );
        }

        if (parsedNumB !== null && __instructions.length === 0) {
          __instructions.push(
            new Instruction("MOV", {
              destination: "AC",
              value: parsedNumB,
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (parsedNumB !== null && __instructions.length > 0) {
          __instructions.push(
            new Instruction("ADD", {
              value: parsedNumB,
            })
          );
        }

        if (variableA && __instructions.length === 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableA.name),
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (variableA && __instructions.length > 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableA.name),
            }),
            new Instruction("ADD", {
              source: "RB",
            })
          );
        }

        if (variableB && __instructions.length === 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableB.name),
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (variableB && __instructions.length > 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableB.name),
            }),
            new Instruction("ADD", {
              source: "RB",
            })
          );
        }
        _instructions.push(...__instructions);
      } else if (op === SUB_ARITHMETICAL_OPERATOR) {
        const __instructions = [];
        if (parsedNumA !== null && __instructions.length === 0) {
          __instructions.push(
            new Instruction("MOV", {
              destination: "AC",
              value: parsedNumA,
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (parsedNumA !== null && __instructions.length > 0) {
          __instructions.push(
            new Instruction("SUB", {
              value: parsedNumA,
            })
          );
        }

        if (parsedNumB !== null && __instructions.length === 0) {
          __instructions.push(
            new Instruction("MOV", {
              destination: "AC",
              value: parsedNumB,
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (parsedNumB !== null && __instructions.length > 0) {
          __instructions.push(
            new Instruction("SUB", {
              value: parsedNumB,
            })
          );
        }

        if (variableA && __instructions.length === 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableA.name),
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (variableA && __instructions.length > 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableA.name),
            }),
            new Instruction("SUB", {
              source: "RB",
            })
          );
        }

        if (variableB && __instructions.length === 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableB.name),
            }),
            new Instruction("MOV", {
              source: "AC",
              destination: "RB",
            })
          );
        } else if (variableB && __instructions.length > 0) {
          __instructions.push(
            new Instruction("LDA", {
              dir: new PendingDirReferencedVariable(variableB.name),
            }),
            new Instruction("SUB", {
              source: "RB",
            })
          );
        }
        _instructions.push(...__instructions);
      } else {
        throw new Error(`Arithmetical operation ${op} is not recognized`);
      }
    } else if (variableMatch && variableMatch.length > 1 && variableMatch[1]) {
      const __instructions = [
        new Instruction("LDA", {
          dir: new PendingDirReferencedVariable(variableMatch[1]),
        }),
      ];
      _instructions.push(...__instructions);
    }

    return _instructions;
  }

  parseAsignation(destVarName: string, source: string) {
    if (forbiddenVariableNamePrefix.test(destVarName))
      throw new Error(`Variable declaration '${destVarName}' is not valid`);

    const sourceOtherSource = this.parseAsignationOtherSource(source); // this first:: order is important
    const sourceInmediateValueInstructions =
      this.parseAsignationInmediateSource(source);

    let variable = this.variables.find((v) => v.name === destVarName);
    if (!variable) {
      variable = new Variable(destVarName);
      this.variables.push(variable);
    }

    if (sourceOtherSource.length > 0) {
      const additionalInstruction = new Instruction("STA", {
        dir: "pending-assignation",
        variable,
      });
      sourceOtherSource.push(additionalInstruction);
      this.instructions.push(...sourceOtherSource);
    } else if (sourceInmediateValueInstructions.length > 0) {
      const additionalInstruction = new Instruction("STA", {
        dir: "pending-assignation",
        variable,
      });
      sourceInmediateValueInstructions.push(additionalInstruction);
      this.instructions.push(...sourceInmediateValueInstructions);
    } else {
      throw new Error(`Unknown source ${source}`);
    }
  }

  parseShow(variableName: string) {
    const variable = this.variables.find((v) => v.name === variableName);
    if (!variable)
      throw new Error(`Referenced variable ${variableName} is not defined`);

    const _instructions = [
      new Instruction("LDA", {
        dir: new PendingDirReferencedVariable(variableName),
      }),
    ];
    this.instructions.push(..._instructions);
  }

  preParseLabel(labelName: string) {
    let label = this.labels.find((l) => l.name === labelName);
    if (label) return;

    label = new Label(labelName);
    this.labels.push(label);
  }

  parseLabel(labelName: string, rest: string) {
    const label = this.labels.find((l) => l.name === labelName);
    if (!label) throw new Error(`Label ${labelName} missmatch with pre`);

    const allocatedUntilNow = this.getAllocated();
    label.address = allocatedUntilNow;

    const parser = new LePaEv(rest, label.address, false);
    parser.import(this);
    parser.parse();

    this.syncWith(parser, { instructions: true, variables: true });
  }

  parseJmp(labelName: string) {
    const label = this.labels.find((l) => l.name === labelName);
    if (!label) throw new Error(`Referenced label ${labelName} is not defined`);

    const _instructions = [
      new Instruction("JMP", {
        intLabel: label,
      }),
    ];

    this.instructions.push(..._instructions);
  }

  loadInput(input: string, destination: Register): Instruction[] {
    // input can be a number or a variableName
    const _instructions: Instruction[] = [];

    const parsedInput = parseString(input);
    const variable = this.variables.find((v) => v.name === input);

    if (!variable && parsedInput === null) {
      throw new Error(`${input} is not recognized`);
    }

    if (parsedInput !== null) {
      _instructions.push(
        new Instruction("MOV", {
          value: parsedInput,
          destination,
        })
      );
    }

    if (variable) {
      _instructions.push(
        new Instruction("LDA", {
          dir: new PendingDirReferencedVariable(variable?.name),
        })
      );

      if (destination !== "AC") {
        _instructions.push(
          new Instruction("MOV", {
            source: "AC",
            destination,
          })
        );
      }
    }
    return _instructions;
  }

  parseCondition(condition: string, body: string) {
    const conditionalRegex = /([^\s]+)[^=]+(==|!=|>=|<=|>|<)[^\w]+([^\s]+)/gm;

    const conditionalMatch = conditionalRegex.exec(condition);
    if (!conditionalMatch || conditionalMatch.length !== 4)
      throw new Error("Invalid conditional");

    const a = conditionalMatch[1];
    const op = conditionalMatch[2];
    const b = conditionalMatch[3];

    const loadBInstructions = this.loadInput(b, "RB");
    const loadAInstructions = this.loadInput(a, "AC");

    this.instructions.push(...loadBInstructions, ...loadAInstructions);

    switch (op) {
      case "==": {
        this.instructions.push(
          new Instruction("CMP", {
            source: "RB",
          })
        );

        const nextFreeAddress = this.getAllocated();

        const beq = new Instruction("BEQ", {
          dir: new PendingDirReferencedInternalLabel(),
        });

        const jmp = new Instruction("JMP", {
          dir: new PendingDirReferencedInternalLabel(),
        });

        const beqCost = beq.getCost();
        const jmpCost = jmp.getCost();
        this.instructions.push(beq, jmp);

        const conditionStartAddress = nextFreeAddress + beqCost + jmpCost;

        const parser = new LePaEv(body, conditionStartAddress, false);
        parser.import(this);
        parser.parse();

        this.syncWith(parser, { instructions: true, variables: true });

        const continueStartAddress =
          conditionStartAddress + parser.getAllocated();

        beq.dir = conditionStartAddress;
        jmp.dir = continueStartAddress;
        break;
      }

      case "!=": {
        this.instructions.push(
          new Instruction("CMP", {
            source: "RB",
          })
        );

        const nextFreeAddress = this.getAllocated();

        const beq = new Instruction("BEQ", {
          dir: new PendingDirReferencedInternalLabel(),
        });

        const beqCost = beq.getCost();
        this.instructions.push(beq);

        const conditionStartAddress = nextFreeAddress + beqCost;

        const parser = new LePaEv(body, conditionStartAddress, false);
        parser.import(this);
        parser.parse();
        this.syncWith(parser, { instructions: true, variables: true });

        const continueStartAddress =
          conditionStartAddress + parser.getAllocated();

        beq.dir = continueStartAddress;
        break;
      }
    }
  }
}

export function parse(code: string, initialAddress: number) {
  const parser = new LePaEv(code, initialAddress, true);

  parser.parse();
  parser.prepare();

  return {
    result: parser.instructions,
    variables: parser.variables,
  };
}
