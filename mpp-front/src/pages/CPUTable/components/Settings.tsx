import React, { useState } from "react";
import { Drawer, Button, Input, Row, Col, Select, Divider, Switch } from "antd";
import { SettingFilled } from "@ant-design/icons";
import { setStoredValue, getStoredValue } from "../../../lib/storage";
import { bases } from "../../../constants/bases";
import I18n from "../../../components/i18n";
import {
  getAvailableLanguageNames,
  setLanguage as setI18nLang,
  SupportedLanguages,
} from "../../../lib/i18n";
import { useTour } from "@reactour/tour";

const { Option } = Select;

export enum SettingType {
  CYCLE_TIME = "settings::cycleTime",
  MEM_VALUE_BASE = "settings::memValueBase",
  LANGUAGE = "settings::language",
  ONBOARDING = "settings::onboarding",
}

export enum SettingDefaultValue {
  CYCLE_TIME = 500,
  MEM_VALUE_BASE = "HEX",
  LANGUAGE = "es",
  ONBOARDING = 1, // true
}

const Settings: React.FC = () => {
  const [visible, setVisible] = useState(false);
  const [cycleTime, setCycleTime] = useState(
    getStoredValue(SettingType.CYCLE_TIME, SettingDefaultValue.CYCLE_TIME)
  );

  const [memValueBase, setMemValueBase] = useState(
    getStoredValue(
      SettingType.MEM_VALUE_BASE,
      SettingDefaultValue.MEM_VALUE_BASE
    )
  );

  const [language, setLanguage] = useState(
    getStoredValue(SettingType.LANGUAGE, SettingDefaultValue.LANGUAGE)
  );

  const [onboarding, setOnboarding] = useState<boolean>(
    getStoredValue(SettingType.ONBOARDING, SettingDefaultValue.ONBOARDING)
  );

  const { setIsOpen } = useTour();

  const showDrawer = () => {
    setVisible(true);
  };

  const onClose = () => {
    setVisible(false);
  };

  const handleCycleTimeChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (e.target.valueAsNumber > Number(e.target.max)) return;
    if (e.target.valueAsNumber < Number(e.target.min)) return;

    setStoredValue(
      SettingType.CYCLE_TIME,
      e.target.valueAsNumber,
      setCycleTime
    );
  };

  const handleMemValueBaseChange = (selected: string) => {
    setStoredValue(SettingType.MEM_VALUE_BASE, selected, setMemValueBase);
  };

  const handleLanguageChange = (selected: SupportedLanguages) => {
    setStoredValue(SettingType.LANGUAGE, selected, () => {
      setLanguage(selected);
      setI18nLang(selected);
    });
  };

  const handleOnboardingChange = (checked: boolean) => {
    setStoredValue(SettingType.ONBOARDING, checked, setOnboarding);
    setVisible(!checked);
    setIsOpen(checked);
    if (checked)
      setStoredValue(SettingType.ONBOARDING, !checked, setOnboarding);
  };

  return (
    <>
      <Button
        type="dashed"
        className="onboarding-settings"
        onClick={showDrawer}
        icon={<SettingFilled />}
      />
      <Drawer
        title={<I18n k="settings.title" />}
        placement="right"
        onClose={onClose}
        visible={visible}
      >
        <Input.Group size="large">
          <Row gutter={8}>
            <Col span={24}>
              <Input
                addonBefore={<I18n k="settings.cycleTime" />}
                value={cycleTime}
                type="number"
                max={5000}
                min={0}
                onChange={handleCycleTimeChange}
              />
            </Col>
          </Row>
        </Input.Group>

        <Divider />

        <Input.Group size="large">
          <Row gutter={8}>
            <Col span={24}>
              <span className="ant-input-group-wrapper">
                <span className="ant-input-wrapper ant-input-group">
                  <span className="ant-input-group-addon">
                    <I18n k="settings.memoryValueBase" />
                  </span>
                  <Select
                    value={memValueBase}
                    onChange={handleMemValueBaseChange}
                  >
                    {bases.map(({ base, radix }) => (
                      <Option key={base} value={base}>
                        {base}
                        <sub>({radix})</sub>
                      </Option>
                    ))}
                  </Select>
                </span>
              </span>
            </Col>
          </Row>
        </Input.Group>

        <Divider />

        <Input.Group size="large">
          <Row gutter={8}>
            <Col span={24}>
              <span className="ant-input-group-wrapper">
                <span className="ant-input-wrapper ant-input-group">
                  <span className="ant-input-group-addon">
                    <I18n k="settings.language" />
                  </span>
                  <Select value={language} onChange={handleLanguageChange}>
                    {getAvailableLanguageNames().map((lang) => (
                      <Option key={lang} value={lang}>
                        {lang}
                      </Option>
                    ))}
                  </Select>
                </span>
              </span>
            </Col>
          </Row>
        </Input.Group>

        <Divider />

        <Switch
          checked={onboarding}
          onChange={handleOnboardingChange}
          checkedChildren={<I18n k="settings.onboarding.on" />}
          unCheckedChildren={<I18n k="settings.onboarding.off" />}
        />
      </Drawer>
    </>
  );
};

export default Settings;
