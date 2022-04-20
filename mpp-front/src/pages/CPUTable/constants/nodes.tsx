import type { Node } from "../../../lib/ReactFlow";

const nodes: Node[] = [
  {
    id: "1",
    type: "input",
    data: { label: "Node 0" },
    position: { x: 250, y: 5 },
    className: "light",
  },
  {
    id: "2",
    data: { label: "Registers bank", width: 200, height: 200, loadBit: { label: "REGCAR", value: 0 },  },
    position: { x: 100, y: 100 },
    className: "light",
    type: 'loadableNode',
    isParent: true,
  },
  {
    id: "2a",
    position: { x: 20, y: 50 },
    parentNode: "2",
    draggable: false,
    type: 'registerNode',
    data: {
      label: 'B',
      readOnly: true,
    }
  },
  {
    id: "2b",
    position: { x: 110, y: 50 },
    parentNode: "2",
    draggable: false,
    type: 'registerNode',
    data: {
      label: 'C',
      readOnly: true,
    }
  },
  {
    id: "2c",
    position: { x: 20, y: 120 },
    parentNode: "2",
    draggable: false,
    type: 'registerNode',
    data: {
      label: 'D',
      readOnly: true,
    }
  },
  {
    id: "2d",
    position: { x: 110, y: 120 },
    parentNode: "2",
    draggable: false,
    type: 'registerNode',
    data: {
      label: 'E',
      readOnly: true,
    }
  },
  {
    id: "3",
    data: { label: "Node 1" },
    position: { x: 320, y: 100 },
    className: "light",
  },
  {
    id: "4",
    data: { label: "Group B" },
    position: { x: 320, y: 200 },
    className: "light",
    style: { backgroundColor: "rgba(255, 0, 0, 0.2)", width: 300, height: 300 },
  },
  {
    id: "4a",
    data: { label: "Node B.1" },
    position: { x: 15, y: 65 },
    className: "light",
    parentNode: "4",
    extent: "parent",
  },
  {
    id: "4b",
    data: { label: "Group B.A" },
    position: { x: 15, y: 120 },
    className: "light",
    style: {
      backgroundColor: "rgba(255, 0, 255, 0.2)",
      height: 150,
      width: 270,
    },
    parentNode: "4",
  },
  {
    id: "4b1",
    data: { label: "Node B.A.1" },
    position: { x: 20, y: 40 },
    className: "light",
    parentNode: "4b",
  },
  {
    id: "4b2",
    data: { label: "Node B.A.2" },
    position: { x: 100, y: 100 },
    className: "light",
    parentNode: "4b",
  },
];
export default nodes;
