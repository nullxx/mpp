import type { FunctionalComponent } from "preact";
import { h, Fragment } from "preact";
import { useState, useEffect, useRef } from "preact/hooks";
import type { MarkdownHeading } from "astro";

const TableOfContents: FunctionalComponent<{ headings: MarkdownHeading[] }> = ({
  headings = [],
}) => {
  const itemOffsets = useRef([]);
  const [activeId, setActiveId] = useState<string>();
  useEffect(() => {
    const getItemOffsets = () => {
      const titles = document.querySelectorAll("article :is(h1, h2, h3, h4)");
      itemOffsets.current = Array.from(titles).map((title) => ({
        id: title.id,
        topOffset: title.getBoundingClientRect().top + window.scrollY,
      }));
    };

    getItemOffsets();
    window.addEventListener("resize", getItemOffsets);

    return () => {
      window.removeEventListener("resize", getItemOffsets);
    };
  }, []);

  return (
    <>
      <h2 className="heading">En esta página</h2>
      <ul>
        <li
          className={`heading-link depth-2 ${
            activeId === "overview" ? "active" : ""
          }`.trim()}
        >
          <a href="#overview">Vision general</a>
        </li>
        {headings
          .filter(({ depth }) => depth > 1 && depth < 4)
          .map((heading) => (
            <li
              className={`heading-link depth-${heading.depth} ${
                activeId === heading.slug ? "active" : ""
              }`.trim()}
            >
              <a href={`#${heading.slug}`}>{heading.text}</a>
            </li>
          ))}
      </ul>
    </>
  );
};

export default TableOfContents;
