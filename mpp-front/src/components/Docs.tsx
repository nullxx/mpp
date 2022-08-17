import { Button } from "antd";

import packageJSON from "../../package.json";

export default function Docs() {
  function handleClick() {
    window.open(packageJSON.repository.documentation, "_blank");
  }

  return (
    <Button
      shape="round"
      style={{
        alignItems: "center",
        justifyContent: "center",
        display: "flex",
      }}
      icon={
        <svg
          xmlns="http://www.w3.org/2000/svg"
          width={24}
          height={24}
          fill="none"
          viewBox="0 0 24 24"
          stroke="currentColor"
          strokeWidth={2}
        >
          <path
            strokeLinecap="round"
            strokeLinejoin="round"
            d="M12 6.253v13m0-13C10.832 5.477 9.246 5 7.5 5S4.168 5.477 3 6.253v13C4.168 18.477 5.754 18 7.5 18s3.332.477 4.5 1.253m0-13C13.168 5.477 14.754 5 16.5 5c1.747 0 3.332.477 4.5 1.253v13C19.832 18.477 18.247 18 16.5 18c-1.746 0-3.332.477-4.5 1.253"
          />
        </svg>
      }
      size="large"
      onClick={handleClick}
    >
      Documentation
    </Button>
  );
}
