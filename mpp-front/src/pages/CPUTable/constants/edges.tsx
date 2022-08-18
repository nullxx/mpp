import { Edge, MarkerType } from "../../../lib/ReactFlow";

const initialEdges: Edge[] = [
  // {
  //   id: "ri-databus",
  //   source: "ri",
  //   target: "14",
  //   animated: false,
  //   type: "smoothstep",
  //   zIndex: 1,
  //   markerEnd: {
  //     width: 50,
  //     height: 50,
  //     type: MarkerType.ArrowClosed,
  //   },
  //   targetHandle: "14-bottom-target-20",
  // },
  {
    id: "databus-ri",
    source: "14",
    target: "ri",
    animated: false,
    type: "smoothstep",
    zIndex: 1,
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-bottom-source-10",
  },
  {
    id: "databus-registersbank",
    source: "14",
    target: "registers-bank",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-bottom-source-40",
  },
  {
    id: "registersbank-databus",
    source: "registers-bank",
    target: "14",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    targetHandle: "14-bottom-target-50",
  },
  {
    id: "14-9",
    source: "14",
    target: "9",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-bottom-source-60",
  },
  {
    id: "9-14",
    source: "9",
    target: "14",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 80,
      type: MarkerType.ArrowClosed,
    },
    targetHandle: "14-bottom-target-90",
  },
  

  {
    id: "databus-pc",
    source: "14",
    target: "pc",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-top-source-0",
  },
  // {
  //   id: "pc-databus",
  //   source: "pc",
  //   target: "14",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.ArrowClosed,
  //   },
  //   targetHandle: "14-top-target-10",
  // },

  {
    id: "14-2",
    source: "14",
    target: "2",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-top-source-20",
  },
  {
    id: "2-14",
    source: "2",
    target: "14",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 80,
      type: MarkerType.ArrowClosed,
    },
    targetHandle: "14-top-target-30",
  },

  {
    id: "databus-sp",
    source: "14",
    target: "sp",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-top-source-37",
  },
  // {
  //   id: "sp-databus",
  //   source: "sp",
  //   target: "14",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.ArrowClosed,
  //   },
  //   targetHandle: "14-top-target-45",
  // },

  {
    id: "databus-2op",
    source: "14",
    target: "2op",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-top-source-55",
  },
  // {
  //   id: "2op-databus",
  //   source: "2op",
  //   target: "14",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.ArrowClosed,
  //   },
  //   targetHandle: "14-top-target-60",
  // },

  {
    id: "databus-h",
    source: "14",
    target: "h",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-top-source-72",
  },
  // {
  //   id: "h-databus",
  //   source: "h",
  //   target: "14",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.ArrowClosed,
  //   },
  //   targetHandle: "14-top-target-77",
  // },

  {
    id: "databus-7",
    source: "14",
    target: "l",
    animated: false,
    type: "smoothstep",
    markerEnd: {
      width: 50,
      height: 50,
      type: MarkerType.ArrowClosed,
    },
    sourceHandle: "14-top-source-90",
  },
  // {
  //   id: "7-databus",
  //   source: "l",
  //   target: "14",
  //   animated: false,
  //   type: "smoothstep",
  //   markerEnd: {
  //     width: 50,
  //     height: 80,
  //     type: MarkerType.ArrowClosed,
  //   },
  //   targetHandle: "14-top-target-95",
  // },
];

export default initialEdges;
