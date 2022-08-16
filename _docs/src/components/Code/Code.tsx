import * as React from "preact";
import './Code.css';

export default function Code({
  code,
  testCodeText,
}: {
  code: string;
  testCodeText?: string;
}) {
  const openCode = (code: string) => {
    // open new url in new tab
    const url = new URL("https://mpp.nullx.me");
    url.searchParams.set("code", code);
    window.open(url.href, "_blank");
  };

  return (
    <pre
      className="shiki github-dark"
      style={{ backgroundColor: "#0d1117", color: "#c9d1d9" }}
    >
      {testCodeText && (
        <div className="run-code">
          <button onClick={() => openCode(code)}>{testCodeText}</button>
        </div>
      )}
      <div className="code-container">
        <code>
          <div className="line">
            {code.split("\\n").map((line, i) => (
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
}
