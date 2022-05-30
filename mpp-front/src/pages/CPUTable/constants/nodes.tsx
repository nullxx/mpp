import type { Node } from "../../../lib/ReactFlow";

const nodes: Node[] = [
  {
    id: "1",
    data: {
      label: "Registers bank",
      width: 400,
      height: 200,
      loadBit: { label: "REGCAR", value: 0 },
    },
    position: { x: -200, y: 150 },
    className: "light",
    type: "loadableNode",
    isParent: true,
  },
  {
    id: "1a",
    position: { x: 20, y: 50 },
    parentNode: "1",
    draggable: false,
    type: "registerNode",
    data: {
      label: "B",
      readOnly: true,
      getFunction: "get_register_b",
    },
  },
  {
    id: "1b",
    position: { x: 200, y: 50 },
    parentNode: "1",
    draggable: false,
    type: "registerNode",
    data: {
      label: "C",
      readOnly: true,
      getFunction: "get_register_c",
    },
  },
  {
    id: "1c",
    position: { x: 20, y: 120 },
    parentNode: "1",
    draggable: false,
    type: "registerNode",
    data: {
      label: "D",
      readOnly: true,
      getFunction: "get_register_d",
    },
  },
  {
    id: "1d",
    position: { x: 200, y: 120 },
    parentNode: "1",
    draggable: false,
    type: "registerNode",
    data: {
      label: "E",
      readOnly: true,
      getFunction: "get_register_e",
    },
  },

  {
    id: "2",
    position: { x: 270, y: 50 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "ACUM",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "ACCAR", value: 1 },
      getFunction: "get_register_acum"
    },
  },

  {
    id: "3",
    position: { x: 270, y: 120 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "2OP",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "2CAR", value: 0 },
      getFunction: 'get_register_2op'
    },
  },
  {
    id: "4",
    position: { x: 270, y: 190 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "PC",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "PCCAR", value: 0 },
      getFunction: "get_register_pc",
    },
  },

  {
    id: "5",
    position: { x: 270, y: 260 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "SP",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "SPCAR", value: 0 },
      getFunction: "get_register_sp",
    },
  },
  {
    id: "6",
    position: { x: 270, y: 330 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "H",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "HCAR", value: 0 },
      getFunction: "get_register_h",
    },
  },
  {
    id: "L",
    position: { x: 270, y: 400 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "L",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "LCAR", value: 0 },
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
    position: { x: -200, y: 0 },
    className: "light",
    type: "loadableNode",
    isParent: true,
  },

  {
    id: "8a",
    position: { x: 20, y: 40 },
    parentNode: "8",
    draggable: false,
    type: "registerNode",
    data: {
      label: "RI",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "RICAR", value: 0 },
      getFunction: "get_register_ri",
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
      getFunction: "get_state"
    },
  },
  {
    id: "9",
    position: { x: -100, y: 360 },
    draggable: true,
    type: "memoryNode",
    data: {
      label: "Memory",
      readOnly: true,
      width: 300,
    },
  },
  {
    id: "10",
    position: { x: -330, y: 360 },
    draggable: true,
    type: "stateTransition",
    data: {
      label: "Transition state",
      readOnly: true,
    },
  }
];
export default nodes;
