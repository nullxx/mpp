import { Handle, Position } from "react-flow-renderer";

export default function Handles({ data, id }: any) {
  return (
    <>
      {data.handlePos?.includes("top") && (
        <>
          {data.handlesCount === 1 && (
            <>
              <Handle
                id={`${id}-top-target-30`}
                type="target"
                position={Position.Top}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-top-source-30`}
                type="source"
                position={Position.Top}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
            </>
          )}

          {data.handlesCount === 2 && (
            <>
              <Handle
                id={`${id}-top-source-70`}
                type="source"
                position={Position.Top}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "70%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-top-target-70`}
                type="target"
                position={Position.Top}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "70%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-top-target-30`}
                type="target"
                position={Position.Top}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-top-source-30`}
                type="source"
                position={Position.Top}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
            </>
          )}
        </>
      )}

      {data.handlePos?.includes("bottom") && (
        <>
          {data.handlesCount === 1 && (
            <>
              <Handle
                id={`${id}-bottom-target-30`}
                type="target"
                position={Position.Bottom}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-bottom-source-30`}
                type="source"
                position={Position.Bottom}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
            </>
          )}

          {data.handlesCount === 2 && (
            <>
              <Handle
                id={`${id}-bottom-source-70`}
                type="source"
                position={Position.Bottom}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "70%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-bottom-target-70`}
                type="target"
                position={Position.Bottom}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "70%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-bottom-target-30`}
                type="target"
                position={Position.Bottom}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
              <Handle
                id={`${id}-bottom-source-30`}
                type="source"
                position={Position.Bottom}
                style={{
                  background: "#555",
                  position: "absolute",
                  left: "30%",
                }}
                isConnectable={false}
              />
            </>
          )}
        </>
      )}
    </>
  );
}
