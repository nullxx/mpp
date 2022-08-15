import "./Solution.css";
import * as React from "preact";

export default function Solution({
  title = "Solution",
  sol,
  testCodeText = 'Test code'
}: {
  title?: string;
  sol: string | string[];
  testCodeText?: string;
}) {
  const openCode = (code: string) => {
    // open new url in new tab
    const url = new URL("https://mpp.nullx.me");
    url.searchParams.set("code", code);
    window.open(url.href, "_blank");
  };

  const renderSolution = (sol: string) => {
    return (
      <pre
        className="shiki github-dark"
        style={{ backgroundColor: "#0d1117", color: "#c9d1d9" }}
      >
        <div className="run-code">
          <button onClick={() => openCode(sol)}>{testCodeText}</button>
        </div>
        <div className="code-container">
          <code>
            <div className="line">
              {sol.split("\\n").map((line, i) => (
                <span key={i}>
                  {line}
                  <br />
                </span>
              ))}
            </div>
          </code>
        </div>
      </pre>
    );
  };

  return (
    <div className="tab tabs">
      <input type="checkbox" id="chck1" />
      <label className="tab-label" htmlFor="chck1">
        {title}
      </label>
      <div className="tab-content">
        {Array.isArray(sol)
          ? sol.map((sol, i) => (
              <>
                <span>
                  {title} {i + 1}
                </span>
                {renderSolution(sol)}
              </>
            ))
          : renderSolution(sol)}
      </div>
    </div>
  );
}
