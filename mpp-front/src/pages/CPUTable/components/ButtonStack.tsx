import { PlaySquareOutlined } from "@ant-design/icons";
import IconButton from "../../../components/IconButton";
import { execute } from '../../../lib/core/index';

export default function ButtonStack() {
  function handleRunState() {
    execute('run_clock_cycle');
  }

  return (
    <div>
      <IconButton icon={<PlaySquareOutlined />} title="Run state" onClick={handleRunState} />
    </div>
  );
}
