export interface MppCoreExtension {
  getValue: (ptr: any, type?: string) => any;
}

export interface MppCore {
  print_hello(): void;
  linker_set_update_ui(fnptr: number): void;

  init(): void;
  shutdown(): void;

  get_memory_size(): number;
  get_memory_value_size_bits(): number;
  get_memory_value(offset: number): number;

  get_register_acum(): number;
  get_register_fc(): number;
  get_register_fz(): number;
  get_register_b(): number;
  get_register_c(): number;
  get_register_d(): number;
  get_register_e(): number;
  get_register_h(): number;
  get_register_l(): number;
  get_register_2op(): number;
  get_register_pch(): number;
  get_register_pcl(): number;
  get_register_pc(): number;
  get_register_sp(): number;
  get_register_ri(): number;

  run_clock_cycle(): number;
  get_state(): number;
  get_next_state(): number;
  set_memory_value(offset: number, value: number): void;
  set_register_pc(value: number): void;
}

export type UIUpdateCallbackFn = () => void;

function throwUninitializedError(fnName: string): never {
  throw new Error(
    `Cannot call function ${fnName} because core is not initialized`
  );
}
export function emptyMppCore(): MppCore & MppCoreExtension {
  return {
    getValue: (ptr: any, type?: string) => {
      throwUninitializedError("getValue");
    },
    print_hello: () => {
      throwUninitializedError("print_hello");
    },
    linker_set_update_ui: (fnptr: number) => {
      throwUninitializedError("linker_set_update_ui");
    },
    init: () => {
      throwUninitializedError("init");
    },
    shutdown: () => {
      throwUninitializedError("shutdown");
    },
    get_memory_size: () => {
      throwUninitializedError("get_memory_size");
    },
    get_memory_value_size_bits: () => {
      throwUninitializedError("get_memory_value_size");
    },
    get_memory_value: (offset: number) => {
      throwUninitializedError("get_memory_value");
    },
    get_register_acum: () => {
      throwUninitializedError("get_register_acum");
    },
    get_register_fc: () => {
      throwUninitializedError("get_register_fc");
    },
    get_register_fz: () => {
      throwUninitializedError("get_register_fz");
    },
    get_register_b: () => {
      throwUninitializedError("get_register_b");
    },
    get_register_c: () => {
      throwUninitializedError("get_register_c");
    },
    get_register_d: () => {
      throwUninitializedError("get_register_d");
    },
    get_register_e: () => {
      throwUninitializedError("get_register_e");
    },
    get_register_h: () => {
      throwUninitializedError("get_register_h");
    },
    get_register_l: () => {
      throwUninitializedError("get_register_l");
    },
    get_register_2op: () => {
      throwUninitializedError("get_register_2op");
    },
    get_register_pch: () => {
      throwUninitializedError("get_register_pch");
    },
    get_register_pcl: () => {
      throwUninitializedError("get_register_pcl");
    },
    get_register_pc: () => {
      throwUninitializedError("get_register_pc");
    },
    get_register_sp: () => {
      throwUninitializedError("get_register_sp");
    },
    get_register_ri: () => {
      throwUninitializedError("get_register_ri");
    },
    run_clock_cycle: () => {
      throwUninitializedError("run_clock_cycle");
    },
    get_state: () => {
      throwUninitializedError("get_state");
    },
    get_next_state: () => {
      throwUninitializedError("get_next_state");
    },
    set_memory_value: (offset: number, value: number) => {
      throwUninitializedError("set_memory_value");
    },
    set_register_pc: (value: number) => {
      throwUninitializedError("set_register_pc");
    }
  };
}
