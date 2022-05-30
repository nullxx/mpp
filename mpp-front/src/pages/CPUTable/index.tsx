import { useState, useCallback, ReactNode } from "react";
import Flow, { applyEdgeChanges, applyNodeChanges } from "../../lib/ReactFlow";
import { Layout, Row, Space } from "antd";

import type {
  Node,
  Edge,
  NodeChange,
  EdgeChange,
  NodeTypes,
} from "../../lib/ReactFlow";

import initialNodes from "./constants/nodes";
import initialEdges from "./constants/edges";
import RegisterNode from "./components/RegisterNode";
import FloatingEdge from "./components/FloatingEdge";
// import FloatingConnectionLine from "./components/FloatingConnectionLine";
import LoadableNode from "./components/LoadableNode";
import BusNode from "./components/BusNode";
import MemoryNode from "./components/MemoryNode";
import ButtonStack from "./components/ButtonStack";
import Settings from "./components/Settings";
import Changelog from "./components/Info";
import { EdgeTypes } from "react-flow-renderer";
import StateTransition from "./components/StateTransition";

const { Header, Sider, Content } = Layout;

const nodeTypes: NodeTypes = {
  registerNode: RegisterNode as unknown as ReactNode,
  loadableNode: LoadableNode as unknown as ReactNode,
  memoryNode: MemoryNode as unknown as ReactNode,
  stateTransition: StateTransition as unknown as ReactNode,
  busNode: BusNode as unknown as ReactNode, // not used
};

const edgeTypes: EdgeTypes = {
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
      <Layout style={{ height: "100%" }}>
        <Header style={{ backgroundColor: "unset" }}>
          <Row justify="space-between">
            <h2>M++ Simulator</h2>
            <Space>
              <Settings />
              <Changelog />
            </Space>
          </Row>
        </Header>
        <Layout>
          <Content>
            <div
              style={{
                width: "100%",
                height: "100%",
                border: "1px solid #ccc",
              }}
            >
              <Flow
                nodes={nodes}
                edges={edges}
                onNodesChange={onNodesChange}
                onEdgesChange={onEdgesChange}
                nodeTypes={nodeTypes}
                edgeTypes={edgeTypes}
                fitView
              />
            </div>
          </Content>
          <Sider theme="light">
            <ButtonStack />
          </Sider>
        </Layout>
      </Layout>
    </>
  );
}

export default CPUTable;
