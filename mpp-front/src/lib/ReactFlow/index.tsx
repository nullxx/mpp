import ReactFlow, {
  Controls,
  ReactFlowProps,
} from "react-flow-renderer";

export interface Point {
  x: number;
  y: number;
}

function Flow({
  nodes,
  edges,
  onNodesChange,
  onEdgesChange,
  onConnect,
  fitView,
  fitViewOptions,
  nodeTypes,
  edgeTypes,
  connectionLineComponent,
}: {
  nodes: ReactFlowProps["nodes"];
  edges: ReactFlowProps["edges"];
  onNodesChange?: ReactFlowProps["onNodesChange"];
  onEdgesChange?: ReactFlowProps["onEdgesChange"];
  onConnect?: ReactFlowProps["onConnect"];
  fitView?: ReactFlowProps["fitView"];
  fitViewOptions?: ReactFlowProps["fitViewOptions"];
  nodeTypes?: ReactFlowProps["nodeTypes"];
  edgeTypes?: ReactFlowProps["edgeTypes"];
  connectionLineComponent?: ReactFlowProps["connectionLineComponent"];
}) {
  return (
    <ReactFlow
      nodes={nodes}
      edges={edges}
      onNodesChange={onNodesChange}
      onEdgesChange={onEdgesChange}
      onConnect={onConnect}
      fitView={fitView}
      fitViewOptions={fitViewOptions}
      nodeTypes={nodeTypes}
      edgeTypes={edgeTypes}
      connectionLineComponent={connectionLineComponent}
    >
      {/* <MiniMap /> */}
      <Controls />
    </ReactFlow>
  );
}

export default Flow;

export {
  applyEdgeChanges,
  applyNodeChanges,
  getBezierPath,
  Handle,
  Position,
  MarkerType,
  useNodesState,
  useEdgesState,
} from "react-flow-renderer";

export type {
  Node,
  Edge,
  OnNodesChange,
  OnEdgesChange,
  NodeChange,
  EdgeChange,
  NodeTypes,
  ConnectionLineComponentProps
} from "react-flow-renderer";
