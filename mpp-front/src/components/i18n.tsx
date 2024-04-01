import { useState, useEffect, memo } from "react";
import { loc, LocalizeOptions, onLanguageChange } from "../lib/i18n";

function I18n({ k, options, capitalize = false, format, evalu = false }: { k: string; options?: LocalizeOptions; capitalize?: boolean; format?: (text: string) => string, evalu?: boolean }) {
  const [text, setText] = useState("");

  useEffect(() => {
    const out = loc(k, options);
    setText(format ? format(out) : out);
  }, [k, options]);

  useEffect(() => {
    const del = onLanguageChange(() => {
      const out = loc(k, options);
      setText(format ? format(out) : out);
    });

    return del;
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  if (capitalize) {
    if (evalu) return <div dangerouslySetInnerHTML={{ __html: text.charAt(0).toUpperCase() + text.slice(1) }} />;
    return <>{text.charAt(0).toUpperCase() + text.slice(1)}</>;
  }

  if (evalu) return <div dangerouslySetInnerHTML={{ __html: text }} />;
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
