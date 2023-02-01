import { useState, useEffect, memo } from "react";
import { loc, LocalizeOptions, onLanguageChange } from "../lib/i18n";

function I18n({ k, options, capitalize = false }: { k: string; options?: LocalizeOptions; capitalize?: boolean }) {
  const [text, setText] = useState("");

  useEffect(() => {
    const out = loc(k, options);
    setText(out);
  }, [k, options]);

  useEffect(() => {
    const del = onLanguageChange(() => {
      const out = loc(k, options);
      setText(out);
    });

    return del;
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  if (capitalize) {
    return <>{text.charAt(0).toUpperCase() + text.slice(1)}</>;
  }

  return <>{text}</>;
}

export default memo(I18n);

export function useI18n(k: string, options?: LocalizeOptions) {
  const [text, setText] = useState("");

  useEffect(() => {
    const out = loc(k, options);
    setText(out);
  }, [k, options]);

  useEffect(() => {
    const del = onLanguageChange(() => {
      const out = loc(k, options);
      setText(out);
    });

    return del;
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return text;
}
