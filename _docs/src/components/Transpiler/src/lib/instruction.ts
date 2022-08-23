import { formatHexValue, formatHexDir } from "./number";

export class Variable {
  address?: number;

  constructor(public name: string) {}
}

export class Label {
  address?: number;
  constructor(public name: string) {}
}

export type Mnemonic =
  | "MOV"
  | "SUB"
  | "CMP"
  | "AND"
  | "OR"
  | "XOR"
  | "INC"
  | "ADD"
  | "CMA"
  | "LDA"
  | "STA"
  | "STAX"
  | "LDAX"
  | "PUSH"
  | "POP"
  | "INISP"
  | "BEQ"
  | "BC"
  | "JMP"
  | "LFA"
  | "SFA"
  | "CALL"
  | "RET"
  | "IRET"
  | "FIN";

export type Register = "AC" | "RB" | "RC" | "RD" | "RE";

export class PendingDirReferencedVariable {
  constructor(public varName: string) {}
}

export class PendingDirReferencedInternalLabel {}
export type PendingDir =
  | "pending-assignation"
  | PendingDirReferencedVariable
  | PendingDirReferencedInternalLabel;

export class Instruction {
  value?: number;
  dir?: number | PendingDir;
  source?: Register;
  destination?: Register;
  variable?: Variable; // when doing STA
  intLabel?: Label;

  constructor(
    public mnemonic: Mnemonic,
    {
      value,
      dir,
      source,
      destination,
      variable,
      intLabel,
    }: {
      value?: number;
      dir?: number | PendingDir;
      source?: Register;
      destination?: Register;
      variable?: Variable;
      intLabel?: Label;
    }
  ) {
    this.value = value;
    this.dir = dir;
    this.source = source;
    this.destination = destination;
    this.variable = variable;
    this.intLabel = intLabel;
  }

  getCost(): number {
    if (this.source !== undefined && this.destination !== undefined) {
      return 1;
    } else if (this.value !== undefined && this.destination !== undefined) {
      return 2;
    } else if (this.value !== undefined) {
      return 2;
    } else if (this.source !== undefined) {
      return 1;
    } else if (this.dir !== undefined) {
      return 3;
    } else if (this.intLabel !== undefined) {
      return 3;
    }
    return 1;
  }

  format() {
    if (this.source !== undefined && this.destination !== undefined) {
      return `${this.mnemonic} ${this.source}, ${this.destination}`;
    } else if (this.value !== undefined && this.destination !== undefined) {
      const valueHex = formatHexValue(this.value);
      return `${this.mnemonic} ${valueHex}, ${this.destination}`;
    } else if (this.value !== undefined) {
      const valueHex = formatHexValue(this.value);
      return `${this.mnemonic} ${valueHex}`;
    } else if (this.source !== undefined) {
      return `${this.mnemonic} ${this.source}`;
    } else if (this.dir !== undefined) {
      if (this.dir === "pending-assignation") {
        throw new Error("Could not format, dir is <pending-assignation>");
      } else if (this.dir instanceof PendingDirReferencedVariable) {
        throw new Error(
          `Could not format, linked variable dir is not replaced`
        );
      } else if (this.dir instanceof PendingDirReferencedInternalLabel) {
        throw new Error(
          `Could not format, linked internal label dir is not replaced`
        );
      }

      const dirHex = formatHexDir(this.dir);
      return `${this.mnemonic} ${dirHex}`;
    } else if (this.intLabel !== undefined && this.intLabel instanceof Label) {
      if (!this.intLabel || this.intLabel.address === undefined)
        throw new Error(
          "Internal label does not exist or has no address attached"
        );
      const hexDir = formatHexDir(this.intLabel.address);
      return `${this.mnemonic} ${hexDir}`;
    }
    return this.mnemonic;
  }
}
