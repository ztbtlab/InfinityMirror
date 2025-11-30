import { useState, useCallback } from 'react';

const SERVICE_UUID = "12345678-1234-1234-1234-1234567890ab";
const CHARACTERISTIC_UUID = "abcd1234-5678-90ab-cdef-1234567890ab";

export const useBLE = () => {
  const [device, setDevice] = useState(null);
  const [characteristic, setCharacteristic] = useState(null);
  const [isConnected, setIsConnected] = useState(false);
  const [status, setStatus] = useState("BLE接続ボタンを押してデバイスに接続してください");

  const connect = useCallback(async () => {
    try {
      setStatus("BLEデバイスを検索中...");
      const device = await navigator.bluetooth.requestDevice({
        filters: [{ namePrefix: "InfinityMirror" }],
        optionalServices: [SERVICE_UUID]
      });

      setStatus("デバイスに接続中...");
      const server = await device.gatt.connect();

      setStatus("サービスを取得中...");
      const service = await server.getPrimaryService(SERVICE_UUID);

      setStatus("キャラクタリスティックを取得中...");
      const char = await service.getCharacteristic(CHARACTERISTIC_UUID);

      setDevice(device);
      setCharacteristic(char);
      setIsConnected(true);
      setStatus("✅ 接続完了！LEDパターンを選択してください");

      device.addEventListener('gattserverdisconnected', () => {
        setIsConnected(false);
        setCharacteristic(null);
        setDevice(null);
        setStatus("❌ デバイスとの接続が切断されました");
      });

    } catch (error) {
      console.error('BLE接続エラー:', error);
      setStatus("❌ 接続に失敗しました: " + error.message);
    }
  }, []);

  const sendCommand = useCallback(async (cmd) => {
    if (!characteristic) {
      setStatus("先にBLE接続を行ってください");
      return;
    }

    try {
      setStatus(`コマンド送信中: ${cmd}`);
      await characteristic.writeValueWithResponse(new TextEncoder().encode(cmd));
      setStatus(`✅ コマンド送信完了: ${cmd}`);
    } catch (error) {
      console.error('コマンド送信エラー:', error);
      setStatus("❌ コマンド送信に失敗しました: " + error.message);
    }
  }, [characteristic]);

  return {
    connect,
    sendCommand,
    isConnected,
    status
  };
};
