export function getStoredValue(key: string) {
  const r = localStorage.getItem(key);
  return r ? JSON.parse(r) : null;
}

export function setStoredValue<T = unknown>(key: string, value: T, updateState: (value: T) => void) {
  localStorage.setItem(key, JSON.stringify(value));
  updateState(value);
}
