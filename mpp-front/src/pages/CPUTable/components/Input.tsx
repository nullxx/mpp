import { Input as AtomizeInput } from "atomize";

function Input({ x, y, height }: { x: number; y: number; height: string }) {
  return (
    <div
      style={{
        position: "absolute",
        top: y,
        left: x,
      }}
    >
      <AtomizeInput
        h={height}
        w="30px"
        p="0px"
        style={{ textAlign: "center" }}
      />
    </div>
  );
}

export default Input;
