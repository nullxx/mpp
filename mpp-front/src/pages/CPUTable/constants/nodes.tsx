import type { Node } from "../../../lib/ReactFlow";

const nodes: Node[] = [
  {
    id: "registers-bank",
    data: {
      label: "Registers bank",
      width: 400,
      height: 200,
      controlBusBitLoad: {
        label: "REGCAR",
        getFunction: "get_control_bus_regcar",
      },
      controlBusBitRelease: {
        label: "REGBUS",
        getFunction: "get_control_bus_regbus",
      },
    },
    draggable: false,
    position: { x: -371, y: 24 },
    className: "light",
    type: "loadableNode",
    isParent: true,
  },
  {
    id: "rb",
    position: { x: 20, y: 50 },
    parentNode: "registers-bank",
    draggable: false,
    type: "registerNode",
    data: {
      label: "RB",
      readOnly: true,
      getFunction: "get_register_b",
      helpInfo: "General purpose register. Use it as 'RB' in the code.",
    },
  },
  {
    id: "rc",
    position: { x: 200, y: 50 },
    parentNode: "registers-bank",
    draggable: false,
    type: "registerNode",
    data: {
      label: "RC",
      readOnly: true,
      getFunction: "get_register_c",
      helpInfo: "General purpose register. Use it as 'RC' in the code.",
    },
  },
  {
    id: "rd",
    position: { x: 20, y: 120 },
    parentNode: "registers-bank",
    draggable: false,
    type: "registerNode",
    data: {
      label: "RD",
      readOnly: true,
      getFunction: "get_register_d",
      helpInfo: "General purpose register. Use it as 'RD' in the code.",
    },
  },
  {
    id: "re",
    position: { x: 200, y: 120 },
    parentNode: "registers-bank",
    draggable: false,
    type: "registerNode",
    data: {
      label: "RE",
      readOnly: true,
      getFunction: "get_register_e",
      helpInfo: "General purpose register. Use it as 'RE' in the code.",
    },
  },

  {
    id: "acum",
    position: { x: -537, y: -227 },
    draggable: false,
    type: "registerNode",
    data: {
      label: "ACUM",
      handlePos: ["bottom", "top"],
      handlesCount: 2,
      readOnly: true,
      width: 70,
      height: 63,
      controlBusBitLoad: {
        label: "ACCAR",
        getFunction: "get_control_bus_accar",
      },
      controlBusBitRelease: {
        label: "ACBUS",
        getFunction: "get_control_bus_acbus",
      },
      getFunction: "get_register_acum",
      helpInfo:
        "Accumulator is a register that is the same length as the processor word (ALU operands). It is used to contain the result of an arithmetic operation or one of the ALU operands.",
    },
  },

  {
    id: "2op",
    position: { x: -337, y: -229 },
    draggable: false,
    type: "registerNode",
    data: {
      label: "2OP",
      handlePos: ["bottom", "top"],
      handlesCount: 2,
      readOnly: true,
      width: 70,
      height: 63,
      controlBusBitLoad: {
        label: "2OPCAR",
        getFunction: "get_control_bus_2opcar",
      },
      getFunction: "get_register_2op",
    },
  },
  {
    id: "pc",
    position: { x: -751, y: -226 },
    draggable: false,
    type: "registerNode",
    data: {
      label: "PC",
      readOnly: true,
      handlePos: ["bottom"],
      handlesCount: 1,
      width: 70,
      height: 63,
      controlBusBitLoad: {
        label: "PCCAR",
        getFunction: "get_control_bus_pccar",
      },
      // controlBusBitRelease: { label: "???", getFunction: 'get_control_bus_pcbus??' },
      getFunction: "get_register_pc",
      helpInfo:
        "Program Counter is a register that contains the address of the next instruction to be executed. It is used to control the execution of the program.",
    },
  },

  {
    id: "sp",
    position: { x: -137, y: -228 },
    draggable: false,
    type: "registerNode",
    data: {
      label: "SP",
      handlePos: ["bottom"],
      handlesCount: 1,
      readOnly: true,
      width: 70,
      height: 63,
      controlBusBitLoad: {
        label: "SPCAR",
        getFunction: "get_control_bus_spcar",
      },
      getFunction: "get_register_sp",
      helpInfo:
        "Stack Pointer is a register that contains the address of the top of the stack. It is used to control the execution of the program in some cases (e.g. subrutine call).",
    },
  },
  {
    id: "h",
    position: { x: 66, y: -225 },
    draggable: false,
    type: "registerNode",
    data: {
      label: "H",
      handlePos: ["bottom"],
      handlesCount: 1,
      readOnly: true,
      width: 70,
      height: 63,
      controlBusBitLoad: { label: "HCAR", getFunction: "get_control_bus_hcar" },
      getFunction: "get_register_h",
    },
  },
  {
    id: "l",
    position: { x: 263, y: -226 },
    draggable: false,
    type: "registerNode",
    data: {
      label: "L",
      handlePos: ["bottom"],
      handlesCount: 1,
      readOnly: true,
      width: 70,
      height: 63,
      controlBusBitLoad: { label: "LCAR", getFunction: "get_control_bus_lcar" },
      getFunction: "get_register_l",
    },
  },

  {
    id: "8",
    data: {
      label: "Control unit",
      width: 400,
      height: 120,
    },
    draggable: false,
    position: { x: -787, y: 62 },
    className: "light",
    type: "loadableNode",
    isParent: true,
  },

  {
    id: "ri",
    position: { x: 20, y: 40 },
    parentNode: "8",
    draggable: false,
    type: "registerNode",
    data: {
      label: "RI",
      readOnly: true,
      width: 70,
      height: 63,
      handlePos: ["top"],
      handlesCount: 1,
      controlBusBitLoad: {
        label: "RICAR",
        getFunction: "get_control_bus_ricar",
      },
      getFunction: "get_register_ri",
      helpInfo:
        "Register Instruction is a register that contains the instruction currently being executed. It is used to control the execution of the program.",
    },
  },
  {
    id: "8b",
    position: { x: 200, y: 40 },
    parentNode: "8",
    draggable: false,
    type: "registerNode",
    data: {
      label: "State",
      readOnly: true,
      width: 70,
      height: 63,
      getFunction: "get_state",
    },
  },
  {
    id: "memory",
    position: { x: 50, y: 54 },
    type: "memoryNode",
    draggable: false,
    data: {
      label: "Memory",
      readOnly: false,
      // controlBusBitLoad: { label: "Read (1), Write (0)", getFunction: 'get_control_bus_le' },
      controlBusBitRelease: {
        label: "MEMBUS",
        getFunction: "get_control_bus_membus",
      },
      width: 300,
      helpInfo:
        "Memory is a set of values that contains the instructions and data of the program. If not initialized it is filled randomly.",
    },
  },
  {
    id: "10",
    position: { x: -1028, y: 141 },
    type: "stateTransition",
    data: {
      label: "Transition state",
    },
    draggable: false,
  },
  {
    id: "11",
    position: { x: -1027, y: -229 },
    type: "debuggerNode",
    data: {
      label: "Debugger",
    },
    draggable: false,
  },
  {
    id: "flags",
    position: { x: -170, y: -378 },
    draggable: false,
    type: "flagsNode",
    data: {
      label: "Flags",
      helpInfo:
        "Flags are used to indicate the status of the last ALU operation.",
      fzHelpInfo:
        "FZ is a flag that indicates whether the result of the last ALU operation is zero.",
      fcHelpInfo:
        "FC is a flag that indicates whether the result of the last ALU operation has carry.",
    },
  },
  {
    id: "13",
    position: { x: -777, y: 207 },
    draggable: false,
    type: "cycleTimeNode",
    data: {
      label: "Clock cycle time",
    },
  },
  {
    id: "alu",
    position: { x: -448, y: -383 },
    draggable: false,
    type: "aluNode",
    data: {
      label: "ALU",
      handlePos: ["bottom"],
      handlesCount: 2,
    },
  },
  {
    id: "databus",
    position: { x: -760, y: -100 },
    draggable: false,
    type: "busNode",
    data: {
      label: "Data bus",
      width: 1200,
      getFunction: "get_data_bus",
    },
  },
];
export default nodes;
