import React from "react";

import CodeMirror from "@uiw/react-codemirror";
// import { javascript } from '@codemirror/lang-javascript';
export default function CodeEditor({
  code,
  onChange,
}: {
  code: string;
  onChange: (value: any, viewUpdate: any) => void;
}) {
  return (
    <CodeMirror
      value={code}
      height="35rem"
      style={{ width: "100%", overflow: "scroll" }}
      theme="dark"
      onChange={onChange}
    />
  );
}
