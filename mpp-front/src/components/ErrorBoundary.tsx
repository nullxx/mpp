import React from "react";
import Toast from "react-hot-toast";

export default class ErrorBoundary extends React.Component<
  {
    children: React.ReactNode;
  },
  { hasError: boolean }
> {
  state: Readonly<{ hasError: boolean }> = { hasError: false };
  static getDerivedStateFromError(error: Error) {
    return { hasError: true };
  }
  componentDidCatch(error: Error): void {
    let message = "Unknown error";
    if (error instanceof Error) {
      message = error.message;
    }

    Toast.error(message);
  }

  render(): React.ReactNode {
    return this.props.children;
  }
}
