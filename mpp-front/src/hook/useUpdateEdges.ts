import React from "react";
import { useReactFlow, useEdges } from "react-flow-renderer";
import {
  execute,
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../lib/core";

export default function useUpdateEdges({
  data,
  id,
}: {
  data: any;
  id: string;
}) {
  const reactFlowInstance = useReactFlow();
  const allEdges = useEdges();

  function onUIUpdate() {
    if (data.controlBusBitLoad) {
      const controlBusBitLoadValue = execute(
        data.controlBusBitLoad?.getFunction
      );

      const targetEdge = allEdges.find((edge) => edge.target === id);
      if (!targetEdge) return;

      targetEdge.label = `${data.controlBusBitLoad.label}: ${controlBusBitLoadValue}`;

      const allEdgesEdited = allEdges.map((edge) => {
        if (edge.id === targetEdge.id) {
          return targetEdge;
        }
        return edge;
      });

      reactFlowInstance.setEdges(allEdgesEdited);
    }

    if (data.controlBusBitRelease) {
      const controlBusBitReleaseValue = execute(
        data.controlBusBitRelease?.getFunction
      );

      const sourceEdge = allEdges.find((edge) => edge.source === id);
      if (!sourceEdge) return;

      sourceEdge.label = `${data.controlBusBitRelease.label}: ${controlBusBitReleaseValue}`;

      const allEdgesEdited = allEdges.map((edge) => {
        if (edge.id === sourceEdge.id) {
          return sourceEdge;
        }
        return edge;
      });

      reactFlowInstance.setEdges(allEdgesEdited);
    }
  }

  React.useEffect(() => {
    subscribeToUIUpdates(onUIUpdate);
    return () => {
      unsubscribeToUIUpdates(onUIUpdate);
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);
}
