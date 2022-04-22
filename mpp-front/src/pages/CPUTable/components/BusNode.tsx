import React, { memo } from "react";
import { Handle, Position } from "../../../lib/ReactFlow";

export default memo(({ data, isConnectable }: any) => {
  return (
    <div
      style={{
        // width: data.width,
        // height: 20,
        // borderBottom: data.height + "px solid black",
        // position: "absolute",
      }}
    >
      <Handle
        type="source"
        position={Position.Bottom}
        style={{ background: "#555" }}
        onConnect={(params) => console.log("handle onConnect", params)}
        isConnectable={isConnectable}
      />
    </div>
  );
});
