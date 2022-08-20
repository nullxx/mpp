import { Input, Select } from "antd";
import React from "react";
import { Base, bases } from "../constants/bases";
const { Option } = Select;

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
  isError,
  max,
  disabled = false
}: {
  number: number;
  initialBase: Base;
  onChange?: (number: number, base: Base) => void;
  onBaseChange?: (newBase: Base) => void;
  readOnly?: boolean;
  width?: number | "100%";
  isError?: boolean;
  max?: number;
  disabled?: boolean;
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
    let newValue = e.target.value;
    const isValid = validateNumberBase(e.target.value, base);
    setIsValid(isValid);

    const targetBase = bases.find(({ base: b }) => b === base);
    if (!targetBase) return "";

    if (isValid) {
      let num = parseInt(e.target.value, targetBase.radix);
      num = max && num > max ? max : num;
      newValue = getFormatted(num, base);
      onChange && onChange(num, base);
    }

    setFormatted(newValue);
  };

  const selectBefore = (
    <Select value={base} onChange={handleBaseChange} disabled={disabled}>
      {bases.map(({ base, radix }) => (
        <Option key={base} value={base}>
          {base}
          <sub>({radix})</sub>
        </Option>
      ))}
    </Select>
  );
  return (
    <Input
      readOnly={readOnly}
      addonBefore={selectBefore}
      disabled={disabled}
      value={formatted}
      onChange={onValueChange}
      status={!isValid || isError ? "error" : undefined}
      style={{ width }}
    />
  );
};

export default NumberBaseInput;
