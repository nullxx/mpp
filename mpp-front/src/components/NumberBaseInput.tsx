import { Input, Select } from "antd";
import React from "react";
const { Option } = Select;

export type Base = "DEC" | "HEX" | "BIN";
const bases: { base: Base; radix: number; regex: RegExp }[] = [
  {
    base: "DEC",
    radix: 10,
    regex: /^\d+\.?\d*$/,
  },
  {
    base: "HEX",
    radix: 16,
    regex: /^[a-fA-F0-9]+$/,
  },
  {
    base: "BIN",
    radix: 2,
    regex: /^[0-1]{1,}$/,
  },
];

export function getRadix(base: Base) {
  const targetBase = bases.find(({ base: b }) => b === base);
  if (!targetBase) return 0;

  return targetBase.radix;
}
const getFormatted = (number: number, newBase: Base) => {
  const targetBase = bases.find(({ base }) => base === newBase);
  if (!targetBase) return "";

  const value = Number(number).toString(targetBase.radix).toUpperCase();
  return value;
};

const validateNumberBase = (number: string, base: Base) => {
  const targetBase = bases.find(({ base: b }) => b === base);
  if (!targetBase) return false;

  return targetBase.regex.test(number);
};

const NumberBaseInput = ({
  number,
  initialBase,
  onChange,
  onBaseChange,
  readOnly = false,
  width,
  isError
}: {
  number: number;
  initialBase: Base;
  onChange?: (number: number, base: Base) => void;
  onBaseChange?: (newBase: Base) => void;
  readOnly?: boolean;
  width?: number | "100%";
  isError?: boolean;
}) => {
  const [base, setBase] = React.useState<Base>(initialBase);
  const [formatted, setFormatted] = React.useState(getFormatted(number, base));
  const [isValid, setIsValid] = React.useState(
    validateNumberBase(formatted, base)
  );

  React.useEffect(() => {
    setFormatted(getFormatted(number, base)); // for updating props
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [number]);

  const handleBaseChange = (newBase: Base) => {
    setBase(newBase);
    const frmt = getFormatted(number, newBase);
    if (!frmt) return;

    setFormatted(frmt);

    onBaseChange && onBaseChange(newBase);
  };

  const onValueChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setFormatted(e.target.value);

    const isValid = validateNumberBase(e.target.value, base);
    setIsValid(isValid);

    const targetBase = bases.find(({ base: b }) => b === base);
    if (!targetBase) return "";

    if (isValid) {
      const num = parseInt(e.target.value, targetBase.radix);
      onChange && onChange(num, base);
    }
  };

  const selectBefore = (
    <Select value={base} onChange={handleBaseChange}>
      {bases.map(({ base, radix }) => (
        <Option key={base} value={base}>
          {base}<sub>({radix})</sub>
        </Option>
      ))}
    </Select>
  );
  return (
    <Input
      readOnly={readOnly}
      addonBefore={selectBefore}
      value={formatted}
      onChange={onValueChange}
      status={!isValid || isError ? "error" : undefined}
      style={{width}}
    />
  );
};

export default NumberBaseInput;
