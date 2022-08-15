import "./Solution.css";
import * as React from "preact";

export default function Solution({
  title = "Solution",
  sol,
}: {
  title: string;
  sol: string | string[];
}) {
  const renderSolution = (sol: string) => {
    return (
      <pre
        className="shiki github-dark"
        style={{ backgroundColor: "#0d1117", color: "#c9d1d9" }}
      >
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
