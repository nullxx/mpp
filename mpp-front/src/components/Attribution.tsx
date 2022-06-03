import packageJSON from "../../package.json";
import { Text } from "atomize";

export default function Attribution() {
  return (
    <Text>
      Made with{" "}
      <span role="img" aria-label="heart">
        ❤️
      </span>{" "}
      by{" "}
      <a href={packageJSON.author.url} target="_blank" rel="noreferrer">
        {packageJSON.author.name} ({packageJSON.author.email})
      </a>
    </Text>
  );
}
