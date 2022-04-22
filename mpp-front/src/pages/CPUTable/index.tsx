import { useState, useCallback, ReactNode } from "react";
import Flow, { applyEdgeChanges, applyNodeChanges } from "../../lib/ReactFlow";
import type { Node, Edge, NodeChange, EdgeChange, NodeTypes } from "../../lib/ReactFlow";

import initialNodes from "./constants/nodes";
import initialEdges from "./constants/edges";
import RegisterNode from "./components/RegisterNode";
import FloatingEdge from "./components/FloatingEdge";
// import FloatingConnectionLine from "./components/FloatingConnectionLine";
import LoadableNode from "./components/LoadableNode";
import BusNode from "./components/BusNode";
import MemoryNode from './components/MemoryNode';

const nodeTypes: NodeTypes = {
  registerNode: RegisterNode as unknown as ReactNode,
  loadableNode: LoadableNode as unknown as ReactNode,
  memoryNode: MemoryNode as unknown as ReactNode,
  busNode: BusNode as unknown as ReactNode, // not used
};

const edgeTypes = {
  floating: FloatingEdge as unknown as ReactNode,
};

function CPUTable() {
  const [nodes, setNodes] = useState(initialNodes);
  const [edges, setEdges] = useState(initialEdges);

  const onNodesChange = useCallback(
    (changes: NodeChange[]) =>
      setNodes((nds: Node[]) => applyNodeChanges(changes, nds)),
    [setNodes]
  );

  const onEdgesChange = useCallback(
    (changes: EdgeChange[]) =>
      setEdges((eds: Edge[]) => applyEdgeChanges(changes, eds)),
    [setEdges]
  );


  return (
    <>
      <h2>CPU Table</h2>
      <div style={{ width: "100%", height: 400, border: "1px solid #ccc" }}>
        <Flow
          nodes={nodes}
          edges={edges}
          onNodesChange={onNodesChange}
          onEdgesChange={onEdgesChange}
          nodeTypes={nodeTypes}
          edgeTypes={edgeTypes}
          // connectionLineComponent={FloatingConnectionLine}
          fitView
        />
      </div>
    </>
  );
}
/*
<CPUImage />
<Input x={132} y={132} height="2rem" /> 
<Input x={175} y={132} height="2rem" /> 
<Input x={132} y={175} height="2rem" /> 
<Input x={175} y={175} height="2rem" />
<Input x={267} y={106} height="22px" /> 
*/
export default CPUTable;
