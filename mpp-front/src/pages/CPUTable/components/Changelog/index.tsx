import React, { useState } from "react";
import { GithubOutlined } from "@ant-design/icons";
import { Spin, Tabs, Timeline, Steps, Alert } from "antd";

import constants from "./constants.json";

import { GithubCommit } from "./GithubCommit";
import I18n from "../../../../components/i18n";
const { TabPane } = Tabs;
const { Step } = Steps;

async function getCommitInfo(hash: string = "") {
  let baseURL = `https://api.github.com/repos/${constants.owner}/${constants.repo}/commits`;
  if (hash) baseURL += `/${hash}`;

  const headers = new Headers();
  headers.append("Accept", "application/vnd.github.v3+json");

  const response = await fetch(baseURL);
  const data = await response.json();
  if (response.status !== 200) {
    throw new Error(data.message);
  }

  return data;
}

const Changelog: React.FC = () => {
  const [loadingReleased, setLoadingReleased] = useState(false);
  const [loadingCommits, setLoadingCommits] = useState(false);

  const [releasedVersions, setReleasedVersions] = useState<GithubCommit[]>([]);
  const [commits, setCommits] = useState<GithubCommit[]>([]);

  const [releasedVersionsError, setReleasedVersionsError] =
    useState<Error | null>(null);
  const [commitsError, setCommitsError] = useState<Error | null>(null);

  function getReleased() {
    if (releasedVersions.length > 0) return;
    setLoadingReleased(true);
    setReleasedVersions([]);

    const promise = constants.currentVersion.items.map(async (item) => {
      const commit = await getCommitInfo(item.sha);
      setReleasedVersions((prev) => [...prev, commit]);
    });

    Promise.all(promise)
      .catch((error) => setReleasedVersionsError(error))
      .finally(() => setLoadingReleased(false));
  }

  function getCommits() {
    if (commits.length > 0) return;

    setLoadingCommits(true);
    setReleasedVersions([]);
    setCommitsError(null);

    getCommitInfo()
      .then((commits) => setCommits(commits))
      .catch((error) => setCommitsError(error))
      .finally(() => setLoadingCommits(false));
  }

  React.useEffect(() => {
    getReleased(); // the first one
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  function onTabClick(activeKey: string) {
    switch (activeKey) {
      case "currentVersion":
        getReleased();
        break;
      case "commitHistory":
        getCommits();
        break;
      default:
        break;
    }
  }
  return (
    <>
      <Tabs
        defaultActiveKey="1"
        type="card"
        size={"large"}
        onTabClick={onTabClick}
      >
        <TabPane tab={<I18n k="info.changelog.currentVersion" />} key="currentVersion">
          <Spin tip={<><I18n k="words.loading" />...</>} spinning={loadingReleased}>
            <h4>Version {constants.currentVersion.name}</h4>
            {releasedVersionsError && (
              <Alert showIcon message="Github error" description={releasedVersionsError.message} type="error" />
            )}
            <Steps direction="vertical" size="small" current={-1}>
              {releasedVersions.map((commit, index) => (
                <Step
                  key={index}
                  progressDot={() => (
                    <GithubOutlined
                      style={{ fontSize: "24px", color: "black" }}
                    />
                  )}
                  title={commit.commit.message}
                  description={commit.commit.author.name}
                />
              ))}
            </Steps>
          </Spin>
        </TabPane>
        <TabPane tab={<I18n k="info.changelog.commitHistory" />} key="commitHistory">
          <Spin tip={<><I18n k="words.loading" />...</>} spinning={loadingCommits}>
            <h4>
              from {constants.owner}/{constants.repo}
            </h4>
            {commitsError && (
              <Alert showIcon message="Github error" description={commitsError.message} type="error" />
            )}
            <Timeline mode="alternate">
              {commits.map((commit, index) => (
                <Timeline.Item
                  dot={
                    <GithubOutlined
                      style={{ fontSize: "24px", color: "black" }}
                    />
                  }
                  key={index}
                >
                  <h4>{commit.commit.message}</h4>
                  <p>{commit.commit.author.name}</p>
                </Timeline.Item>
              ))}
            </Timeline>
          </Spin>
        </TabPane>
      </Tabs>
    </>
  );
};

export default Changelog;
