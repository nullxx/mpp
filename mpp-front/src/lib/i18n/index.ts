import en from './en.json';
import es from './es.json';

import { getStoredValue } from "../storage";

export type SupportedLanguages = "en" | "es";
export interface LocalizeOptions {
  lang: SupportedLanguages;
}

type LanguageChangeListener = (lang: SupportedLanguages) => void;
const languageChangeListeners: LanguageChangeListener[] = [];

const availableLanguages: SupportedLanguages[] = ["en", "es"];

let activeLang: SupportedLanguages = getStoredValue("settings::language", "en"); // default 'en'

export function setLanguage(language: SupportedLanguages) {
  activeLang = language;
  languageChangeListeners.forEach((listener) => listener(language));
}

export function getLanguage() {
  return activeLang;
}

function getLanguageData(lang: SupportedLanguages): any {
  switch (lang) {
    case "en":
      return en;
    case "es":
      return es;
    default:
      return en;
  }
}

export function loc(key: string, opts?: LocalizeOptions) {
  const translations = getLanguageData(opts?.lang ?? activeLang);
  if (translations[key]) return translations[key];
}

export function getAvailableLanguageNames() {
  return availableLanguages;
}

export function onLanguageChange(cb: LanguageChangeListener) {
  languageChangeListeners.push(cb);
  return () => {
    languageChangeListeners.splice(languageChangeListeners.indexOf(cb), 1);
  }
}
