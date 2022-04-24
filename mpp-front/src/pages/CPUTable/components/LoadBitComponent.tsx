import { Text, Checkbox } from "atomize";
import RightArrow, { RIGHT_ARROW_WIDTH } from "./RightArrow";

export interface LoadBit {
  label: string;
  value?: boolean;
}

export const DEFAULT_LOADBIT_VALUE = false;

const LoadBitComponent = ({
  lb,
  totalWidth,
  onChange,
}: {
  lb: LoadBit;
  totalWidth: number;
  onChange: (...args: any[]) => any;
}) => (
  <>
    <div
      style={{
        position: "absolute",
        left: -15,
        textAlign: 'right',
        top: -5,
        width: "fit-content",
      }}
    >
      <Text tag="p" textSize="display5">
        {lb.label}
      </Text>
    </div>

    <div
      style={{
        position: "absolute",
        left: RIGHT_ARROW_WIDTH,
        top: 0,
        width: "fit-content",
      }}
    >
      <RightArrow />
    </div>

    <div
      style={{
        position: "absolute",
        right: totalWidth + 15,
        top: 40,
        width: "fit-content",
      }}
    >
      <Checkbox
        onChange={onChange}
        checked={
          typeof lb.value === "undefined" ? DEFAULT_LOADBIT_VALUE : lb.value
        }
        readOnly
      />
    </div>
  </>
);
export default LoadBitComponent;
