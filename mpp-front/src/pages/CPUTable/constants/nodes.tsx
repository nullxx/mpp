import type { Node } from "../../../lib/ReactFlow";

const nodes: Node[] = [
  {
    id: "1",
    data: {
      label: "Registers bank",
      width: 200,
      height: 200,
      loadBit: { label: "REGCAR", value: 0 },
    },
    position: { x: 100, y: 100 },
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
    },
  },
  {
    id: "1b",
    position: { x: 110, y: 50 },
    parentNode: "1",
    draggable: false,
    type: "registerNode",
    data: {
      label: "C",
      readOnly: true,
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
    },
  },
  {
    id: "1d",
    position: { x: 110, y: 120 },
    parentNode: "1",
    draggable: false,
    type: "registerNode",
    data: {
      label: "E",
      readOnly: true,
    },
  },

  {
    id: "2",
    position: { x: 420, y: 50 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "ACUM",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "ACCAR", value: 0 },
    },
  },

  {
    id: "3",
    position: { x: 420, y: 120 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "2OP",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "2CAR", value: 0 },
    },
  },
  {
    id: "4",
    position: { x: 420, y: 190 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "PC",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "PCCAR", value: 0 },
    },
  },

  {
    id: "5",
    position: { x: 420, y: 260 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "SP",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "SPCAR", value: 0 },
    },
  },
  {
    id: "6",
    position: { x: 420, y: 330 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "H",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "HCAR", value: 0 },
    },
  },
  {
    id: "L",
    position: { x: 420, y: 400 },
    draggable: true,
    type: "registerNode",
    data: {
      label: "L",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "LCAR", value: 0 },
    },
  },

  {
    id: "8",
    data: {
      label: "Control unit",
      width: 200,
      height: 120,
    },
    position: { x: -200, y: 100 },
    className: "light",
    type: "loadableNode",
    isParent: true,
  },

  {
    id: "8a",
    position: { x: 20, y: 50 },
    parentNode: "8",
    draggable: false,
    type: "registerNode",
    data: {
      label: "RI",
      readOnly: true,
      width: 70,
      height: 63,
      loadBit: { label: "RICAR", value: 0 },
    },
  },
  {
    id: "9",
    position: { x: 560, y: 200 },
    draggable: true,
    type: "memoryNode",
    data: {
      label: "Memory",
      readOnly: true,
      width: 300,
      height: 500,
    },
  },
];
export default nodes;
