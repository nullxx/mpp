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
  MEASURE_RUN_TIME = "settings::measureRunTime",
}

export enum SettingDefaultValue {
  CYCLE_TIME = 500,
  MEM_VALUE_BASE = "HEX",
  LANGUAGE = "es",
  ONBOARDING = 1, // true
  MEASURE_RUN_TIME = 1, // true
}

const listeners = new Map<SettingType, Set<(value: any) => void>>();
export function addChangeListener(
  type: SettingType,
  callback: (value: any) => void
) {
  if (!listeners.has(type)) {
    listeners.set(type, new Set());
  }

  listeners.get(type)?.add(callback);
}

export function removeChangeListener(
  type: SettingType,
  callback: (value: any) => void
) {
  listeners.get(type)?.delete(callback);
}

export function notifyChange(type: SettingType, value: any) {
  listeners.get(type)?.forEach((callback) => callback(value));
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

  const [measureRunTime, setMeasureRunTime] = useState<boolean>(
    getStoredValue(
      SettingType.MEASURE_RUN_TIME,
      SettingDefaultValue.MEASURE_RUN_TIME
    )
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

    notifyChange(SettingType.CYCLE_TIME, e.target.valueAsNumber);
  };

  const handleMemValueBaseChange = (selected: string) => {
    setStoredValue(SettingType.MEM_VALUE_BASE, selected, setMemValueBase);

    notifyChange(SettingType.MEM_VALUE_BASE, selected);
  };

  const handleLanguageChange = (selected: SupportedLanguages) => {
    setStoredValue(SettingType.LANGUAGE, selected, () => {
      setLanguage(selected);
      setI18nLang(selected);
    });

    notifyChange(SettingType.LANGUAGE, selected);
  };

  const handleOnboardingChange = (checked: boolean) => {
    setStoredValue(SettingType.ONBOARDING, checked, setOnboarding);
    setVisible(!checked);
    setIsOpen(checked);
    if (checked)
      setStoredValue(SettingType.ONBOARDING, !checked, setOnboarding);
    
    notifyChange(SettingType.ONBOARDING, checked);
  };

  const handleMeasureRunTimeChange = (checked: boolean) => {
    setStoredValue(SettingType.MEASURE_RUN_TIME, checked, setMeasureRunTime);
    notifyChange(SettingType.MEASURE_RUN_TIME, checked);
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

        {cycleTime === 0 && (
          <p>
            <I18n k="settings.cycleTimeZeroMsg" />
          </p>
        )}

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

        <Divider />

        <Switch
          checked={measureRunTime}
          onChange={handleMeasureRunTimeChange}
          checkedChildren={<I18n k="settings.measureRunTime.on" />}
          unCheckedChildren={<I18n k="settings.measureRunTime.off" />}
        />
      </Drawer>
    </>
  );
};

export default Settings;
