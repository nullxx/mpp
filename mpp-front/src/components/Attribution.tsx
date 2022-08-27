import packageJSON from "../../package.json";
import { Text } from "atomize";
import I18n from "./i18n";

export default function Attribution() {
  return (
    <Text>
      <I18n k="words.madeWith" />{" "}
      <span role="img" aria-label="heart">
        ❤️
      </span>{" "}
      <I18n k="words.by" />{" "}
      <a href={packageJSON.author.url} target="_blank" rel="noreferrer">
        {packageJSON.author.name} ({packageJSON.author.email})
      </a>
    </Text>
  );
}
