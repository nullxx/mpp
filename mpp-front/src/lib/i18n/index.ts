import en from './en.json';
import es from './es.json';
import eu from './eu.json';

import { getStoredValue } from "../storage";

export type SupportedLanguages = "eu" | "en" | "es";
export interface LocalizeOptions {
  lang: SupportedLanguages;
}

type LanguageChangeListener = (lang: SupportedLanguages) => void;
const languageChangeListeners: LanguageChangeListener[] = [];

const availableLanguages: SupportedLanguages[] = ["eu", "es", "en"]; // availableLanguages must be > 0

const navigatorLang = navigator.language.split("-")[0] ?? availableLanguages[0];
let activeLang: SupportedLanguages = getStoredValue("settings::language", navigatorLang);

export function setLanguage(language: SupportedLanguages) {
  activeLang = language;
  languageChangeListeners.forEach((listener) => listener(language));
}

export function getLanguage() {
  return activeLang;
}

function getLanguageData(lang: SupportedLanguages): any {
  switch (lang) {
    case "eu":
      return eu;
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
