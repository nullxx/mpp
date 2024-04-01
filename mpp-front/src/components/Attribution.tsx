import packageJSON from "../../package.json";
import { Text } from "atomize";
import I18n from "./i18n";

export default function Attribution() {
  return (
    <Text>
      <I18n
        k="credit"
        format={(t =>
          t
            .replace('<AUTHOR_URL>', packageJSON.author.url)
            .replace('<AUTHOR_NAME>', packageJSON.author.name
            ))}
        evalu
      />
    </Text>
  );
}
