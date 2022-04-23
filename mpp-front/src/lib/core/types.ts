export interface MppCoreExtension {
  getValue: (ptr: any, type?: string) => any;
}

export interface MppCore {
  print_hello(): void;
  linker_set_update_ui(fnptr: number): void;

  init(): void;
  shutdown(): void;

  get_memory_size(): number;
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
}

export type UIUpdateCallbackFn = () => void;

export function emptyMppCore(): MppCore & MppCoreExtension {
  return {
    getValue: (ptr: any, type?: string) => {
      throw new Error(`Method getValue not implemented`);
    },
    print_hello: () => {
      throw new Error("MppCore not loaded");
    },
    linker_set_update_ui: (fnptr: number) => {
      throw new Error("MppCore not loaded");
    },

    init: () => {
      throw new Error("MppCore not loaded");
    },
    shutdown: () => {
      throw new Error("MppCore not loaded");
    },

    get_memory_size: () => {
      throw new Error("MppCore not loaded");
    },
    get_memory_value: (offset: number) => {
      throw new Error("MppCore not loaded");
    },

    get_register_acum: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_fc: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_fz: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_b: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_c: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_d: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_e: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_h: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_l: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_2op: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_pch: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_pcl: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_pc: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_sp: () => {
      throw new Error("MppCore not loaded");
    },
    get_register_ri: () => {
      throw new Error("MppCore not loaded");
    },
    run_clock_cycle: () => {
      throw new Error("MppCore not loaded");
    },
    get_state: () => {
      throw new Error("MppCore not loaded");
    }
  };
}
