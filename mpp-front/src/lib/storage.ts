export function setStoredValue<T = unknown>(
  key: string,
  value: T,
  updateState?: (value: T) => void
) {
  localStorage.setItem(key, JSON.stringify(value));
  if (updateState) {
    updateState(value);
  }
}

export function getStoredValue<T = unknown>(key: string, defaultValue: T) {
  const r = localStorage.getItem(key);
  const value = r ? JSON.parse(r) : null;
  if (value == null) setStoredValue<T>(key, defaultValue);
  return value || defaultValue;
}
