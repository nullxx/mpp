import { useEffect, useState, useRef } from "react";

import { useCodeMirror } from "@uiw/react-codemirror";

import hljs from "highlight.js/lib/core";
import "./style.css";
import "../Code/Code"; // for define mpp
import type { Variable } from "../../lib/mpp-compiler/src/lib/generator/_variable";
import type { Instruction } from "../../lib/mpp-compiler/src/lib/instruction";
import { parseCode } from "../../lib/mpp-compiler/src";

export default function Transpiler({ code }: { code: string }) {
  const [input, setInput] = useState<string>(code);
  const [output, setOutput] = useState<Instruction[] | null>();
  const [error, setError] = useState<string | undefined>("");
  const [variables, setVariables] = useState<Variable[]>([]);

  const editor = useRef(null);
  const { setContainer } = useCodeMirror({
    container: editor.current,
    value: input,
    height: "20rem",
    onChange(value, viewUpdate) {
      setInput(value);
    },
  });

  useEffect(() => {
    if (editor.current) setContainer(editor.current);
  }, [editor.current]);

  useEffect(() => {
    setError("");
    try {
      const result = parseCode(input, 0);
      let uniqueVariables = [
        ...new Map(variables.map((item) => [item["name"], item])).values(),
      ];
      setVariables(uniqueVariables);

      setOutput(result);
    } catch (error) {
      setOutput(null);
      if (error instanceof Error) setError(error.message);
    }
  }, [input]);

  return (
    <>
      {error && <div className="alert">{error}</div>}
      <div style={{ height: 5 }} />
      <div style={{ display: "flex", height: "20rem" }}>
        <div ref={editor} style={{ width: "50%", color: 'black' }} />
        <pre
          style={{
            backgroundColor: "#0d1117",
            color: "#c9d1d9",
            width: "50%",
            overflow: "scroll",
          }}
          dangerouslySetInnerHTML={{
            __html: hljs.highlight(
              output?.map((i) => i.format()).join("\n") ?? "",
              { language: "mpp" }
            ).value,
          }}
        ></pre>
      </div>
    </>
  );
}
