import React, { useEffect, useState, useRef } from "react";

import type * as monaco from "monaco-editor/esm/vs/editor/editor.api";
import Editor from "@monaco-editor/react";

import { parse } from "./src";
import type { Instruction, Variable } from "./src/lib/instruction";

export default function Transpiler({ code }: { code: string }) {
  const [input, setInput] = useState<string>(code);
  const [output, setOutput] = useState<Instruction[] | null>();
  const [error, setError] = useState<string | undefined>("");
  const [variables, setVariables] = useState<Variable[]>([]);

  const inputEditor = useRef<monaco.editor.IStandaloneCodeEditor | null>(null);

  useEffect(() => {
    setError("");
    try {
      const { result, variables } = parse(input, 0);
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
      <p style={{ color: "red" }}>{error}</p>
      <div style={{ width: "100%", display: "flex" }}>
        <Editor
          height="40vh"
          width="50%"
          value={input}
          onChange={(v) => setInput(v || "")}
          defaultLanguage="mpp"
          defaultValue={code}
          onMount={(e, m) => (inputEditor.current = e)}
        />

        <Editor
          height="40vh"
          width="50%"
          options={{
            readOnly: true,
            lineNumbers: (line) => {
              const index = line - 1;

              const instructions = output?.slice(0, index) ?? [];
              const cost = instructions
                ?.map((i) => i.getCost())
                .reduce((partialSum, a) => partialSum + a, 0);

              return Number(cost).toString(16).padStart(4, "0").toUpperCase();
            },
          }}
          value={
            output === null ? "" : output?.map((i) => i.format()).join("\n")
          }
          defaultLanguage="assembly"
          defaultValue=""
        />
      </div>

      <>
        <h2>Variables info</h2>
        <table>
          <thead>
            <tr>
              <th>Variable</th>
              <th>Address</th>
            </tr>
          </thead>
          <tbody>
            {variables.map((v) => (
              <tr key={v.name}>
                <td>{v.name}</td>
                <td>
                  {v.address?.toString(16).padStart(4, "0").toUpperCase()}
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </>
    </>
  );
}
