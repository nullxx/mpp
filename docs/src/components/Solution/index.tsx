import "./Solution.css";
import * as React from "react";
import Code from "../Code/Code";

export default function Solution({
  title = "Solution",
  sol = [],
  testCodeText = "Test code",
}: {
  title?: string;
  sol?: string[];
  testCodeText?: string;
}) {
  if (sol.length === 0) return <p>No solution</p>;

  return (
    <div className="tab tabs">
      <input type="checkbox" id="chck1" />
      <label className="tab-label" htmlFor="chck1">
        {title}
      </label>
      <div className="tab-content">
        {sol.length > 1
          ? sol.map((sol, i) => (
              <div key={i}>
                <span>
                  {title} {i + 1}
                </span>
                <Code code={sol} testCodeText={testCodeText} />
              </div>
            ))
          : <Code code={sol[0]} testCodeText={testCodeText} />}
      </div>
    </div>
  );
}
