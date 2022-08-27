import { useCallback, ReactNode } from "react";
import Flow, {
  applyEdgeChanges,
  applyNodeChanges,
  useNodesState,
  useEdgesState,
} from "../../lib/ReactFlow";
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
import LoadableNode from "./components/LoadableNode";
import BusNode from "./components/BusNode";
import MemoryNode from "./components/MemoryNode";
import ButtonStack from "./components/ButtonStack";
import Settings from "./components/Settings";
import Changelog from "./components/Info";
import StateTransition from "./components/StateTransition";
import DebuggerNode from "./components/DebuggerNode";
import FlagsNode from "./components/FlagsNode";
import CycleTimeNode from "./components/CycleTimeNode";
import Github from "../../components/Github";
import Attribution from "../../components/Attribution";
import Docs from "../../components/Docs";
import ALUNode from "./components/ALUNode";
import I18n from "../../components/i18n";

const { Header, Sider, Content, Footer } = Layout;

const nodeTypes: NodeTypes = {
  registerNode: RegisterNode as unknown as ReactNode,
  loadableNode: LoadableNode as unknown as ReactNode,
  memoryNode: MemoryNode as unknown as ReactNode,
  stateTransition: StateTransition as unknown as ReactNode,
  busNode: BusNode as unknown as ReactNode, // not used
  debuggerNode: DebuggerNode as unknown as ReactNode,
  flagsNode: FlagsNode as unknown as ReactNode,
  cycleTimeNode: CycleTimeNode as unknown as ReactNode,
  aluNode: ALUNode as unknown as ReactNode,
};

function CPUTable() {
  const [nodes, setNodes] = useNodesState(initialNodes);
  const [edges, setEdges] = useEdgesState(initialEdges);

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
        <Header className="mpp-header">
          <Row>
            <Space align="center">
              <h1><I18n k="title" /></h1>
              <Github />
              <Docs />
            </Space>
            <div style={{ flexGrow: 1 }} />
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
                fitView
              />
            </div>
          </Content>
          <Sider theme="light" style={{ overflowX: "hidden" }}>
            <ButtonStack />
          </Sider>
        </Layout>
        <Footer style={{ padding: 5, margin: "unset" }}>
          <Attribution />
        </Footer>
      </Layout>
    </>
  );
}

export default CPUTable;
