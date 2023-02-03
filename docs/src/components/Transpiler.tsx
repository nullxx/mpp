import { useState } from "react";
import { parseCode } from "../lib/mpp-compiler/src";
import CodeEditor from "./CodeEditor";
import Code from "./Code/Code";

export default function Transpiler({ code }: { code: string }) {
  const [result, setResult] = useState(parseCode(code, 0));
  const [error, setError] = useState<Error | null>(null);

  return (
    <>
      {error && (
        <div style={{ backgroundColor: "red", padding: 7, borderRadius: 8 }}>
          <p style={{ color: "white" }}>{error.message}</p>
        </div>
      )}
      <div style={{ display: "flex", height: "35rem", gap: "2px" }}>
        <CodeEditor
          code={code}
          onChange={(value) => {
            let newError = null;
            try {
              const result = parseCode(value, 0);
              setResult(result);
            } catch (error) {
              newError = error as Error;
            }
            setError(newError);
          }}
        />

        {!error && (
          <>
            <div style={{ width: 400, overflow: "scroll", wordWrap: "normal" }}>
              <Code
                code={result?.map((i) => i.format()).join("\n")}
                testCodeText="RUN"
              />
            </div>
            <div>
              <h4>Variables</h4>
              <table>
                <thead>
                  <tr>
                    <th>Variable</th>
                    <th>Address</th>
                  </tr>
                </thead>
                <tbody>
                  {result
                    .filter((a) => a.destignatesVariable)
                    // delete duplicates
                    .filter(
                      (a, index, self) =>
                        index ===
                        self.findIndex(
                          (t) =>
                            t.destignatesVariable?.name ===
                            a.destignatesVariable?.name
                        )
                    )
                    .map(({ destignatesVariable }) => (
                      <tr key={destignatesVariable?.name}>
                        <td>{destignatesVariable?.name}</td>
                        <td>
                          {destignatesVariable?.address
                            ?.toString(16)
                            .toUpperCase()}
                        </td>
                      </tr>
                    ))}
                </tbody>
              </table>
              <hr />
              <h4>Labels</h4>
              <table>
                <thead>
                  <tr>
                    <th>Name</th>
                    <th>Address</th>
                  </tr>
                </thead>
                <tbody>
                  {result
                    .filter((a) => a.labeled && a.labeled.type === "user")
                    .filter(
                      (a, index, self) =>
                        index ===
                        self.findIndex(
                          (t) => t.labeled?.name === a.labeled?.name
                        )
                    )
                    .map(({ labeled }) => (
                      <tr key={labeled?.name}>
                        <td>{labeled?.name}</td>
                        <td>{labeled?.address?.toString(16).toUpperCase()}</td>
                      </tr>
                    ))}
                </tbody>
              </table>
            </div>
          </>
        )}
      </div>
    </>
  );
}