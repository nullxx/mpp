import { Edge, MarkerType } from "../../../lib/ReactFlow";

const initialEdges: Edge[] = [
  // {
  //   id: "ri-databus",
  //   source: "ri",
  //   target: "databus",
  //   animated: false,
  //   type: "smoothstep",
  //   zIndex: 1,
  //   markerEnd: {
  //     width: 50,
  //     height: 50,
  //     type: MarkerType.Arrow,
  //   },
  //   targetHandle: "databus-bottom-target-20",
  // },
  {
    id: "databus-ri",
    source: "databus",
    target: "ri",
    animated: false,
    type: "smoothstep",
    zIndex: 1,
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-bottom-source-10",
  },
  {
    id: "databus-registersbank",
    source: "databus",
    target: "registers-bank",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-bottom-source-40",
  },
  {
    id: "registersbank-databus",
    source: "registers-bank",
    target: "databus",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    targetHandle: "databus-bottom-target-50",
  },
  {
    id: "databus-memory",
    source: "databus",
    target: "memory",
    animated: true, // always animated because is always passing data to memory
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-bottom-source-80",
  },
  {
    id: "memory-databus",
    source: "memory",
    target: "databus",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 80,
      type: MarkerType.Arrow,
    },
    targetHandle: "databus-bottom-target-90",
  },
  

  {
    id: "databus-pc",
    source: "databus",
    target: "pc",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-top-source-5",
  },
  // {
  //   id: "pc-databus",
  //   source: "pc",
  //   target: "databus",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.Arrow,
  //   },
  //   targetHandle: "databus-top-target-10",
  // },

  {
    id: "databus-acum",
    source: "databus",
    target: "acum",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-top-source-20",
    targetHandle: 'acum-bottom-target-30'
  },
  {
    id: "acum-databus",
    source: "acum",
    target: "databus",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 80,
      type: MarkerType.Arrow,
    },
    targetHandle: "databus-top-target-30",
    sourceHandle: 'acum-bottom-source-70'
  },

  {
    id: "databus-sp",
    source: "databus",
    target: "sp",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-top-source-55",
  },
  // {
  //   id: "sp-databus",
  //   source: "sp",
  //   target: "databus",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.Arrow,
  //   },
  //   targetHandle: "databus-top-target-45",
  // },

  {
    id: "databus-2op",
    source: "databus",
    target: "2op",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-top-source-37",
    targetHandle: '2op-bottom-target-30'
  },
  // {
  //   id: "2op-databus",
  //   source: "2op",
  //   target: "databus",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.Arrow,
  //   },
  //   targetHandle: "databus-top-target-60",
  // },

  {
    id: "databus-h",
    source: "databus",
    target: "h",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-top-source-72",
  },
  // {
  //   id: "h-databus",
  //   source: "h",
  //   target: "databus",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.Arrow,
  //   },
  //   targetHandle: "databus-top-target-77",
  // },

  {
    id: "databus-7",
    source: "databus",
    target: "l",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    sourceHandle: "databus-top-source-90",
  },
  {
    id: "acum-alu",
    source: "acum",
    target: "alu",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    label: "A",
    sourceHandle: "acum-top-source-70",
    targetHandle: 'alu-input-A'
  },
  {
    id: "2op-alu",
    source: "2op",
    target: "alu",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    label: "B",
    sourceHandle: "2op-top-source-30",
    targetHandle: 'alu-input-B'
  },
  {
    id: "alu-flagsfz",
    source: "alu",
    target: "flags",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    label: "FZ",
    sourceHandle: "alu-output-FZ",
    targetHandle: 'flags-input-FZ'
  },
  {
    id: "alu-flagsfc",
    source: "alu",
    target: "flags",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.Arrow,
    },
    label: "FC",
    sourceHandle: "alu-output-FC",
    targetHandle: 'flags-input-FC'
  },
  // {
  //   id: "7-databus",
  //   source: "l",
  //   target: "databus",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.Arrow,
  //   },
  //   targetHandle: "databus-top-target-95",
  // },
];

export default initialEdges;
