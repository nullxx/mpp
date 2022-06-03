import { execute } from "../core";
import operations from "../traslator/operations.json";

interface FindOperationValue {
  operation: string;
  range: [number, number];
}
type ReplacementFn = (offset: number, operation: string) => string;

const INM_NAME = "inm";
const DIR_NAME = "dir";
export const NO_OP_NAME = "-";
const FILL_NO_OP = true;
const replacementFunctions: ReplacementFn[] = [replaceInm, replaceDir];

let memSize: null | number = null;

export function deductOperationOf(fromMemOffset: number, toMemOffset: number) {
  if (memSize == null) memSize = execute<number>("get_memory_size");
  if (toMemOffset < fromMemOffset) {
    throw new Error("toMemOffset must be greater than fromMemOffset");
  }
  if (fromMemOffset < 0) {
    fromMemOffset = 0;
  }
  if (toMemOffset > memSize) {
    toMemOffset = memSize;
  }

  const operations: FindOperationValue[] = [];

  let initOffset = fromMemOffset;
  do {
    const currentMemValue = execute<number>("get_memory_value", initOffset);
    const operation = findOperation(initOffset, currentMemValue);
    if (!operation) {
      initOffset++;
      continue;
    }

    operations.push(operation);
    initOffset = operation.range[1] + 1;
  } while (operations.length === 0 || initOffset < toMemOffset);

  return operations;
}

function findOperation(
  initOffset: number,
  memValue: number
): FindOperationValue | null {
  const op = operations.find((o) => parseInt(o.HEX, 16) === memValue);
  if (!op && !FILL_NO_OP) return null;
  if (!op) return { operation: NO_OP_NAME, range: [initOffset, initOffset] };
  return {
    operation: doReplacements(initOffset, op.NEMO),
    range: [initOffset, initOffset + op.ALLOC - 1],
  };
}

function doReplacements(offset: number, operation: string) {
  return replacementFunctions.reduce((acc, fn) => fn(offset, acc), operation);
}

function replaceInm(offset: number, operation: string) {
  return operation.replace(
    INM_NAME,
    execute<number>("get_memory_value", offset + 1).toString(16)
  );
}

function replaceDir(offset: number, operation: string) {
  const hdir = execute<number>("get_memory_value", offset + 1).toString(16);
  const ldir = execute<number>("get_memory_value", offset + 2).toString(16);
  return operation.replace(DIR_NAME, `${hdir}${ldir}`);
}
