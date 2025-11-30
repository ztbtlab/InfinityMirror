import React, { useState, useCallback } from 'react';

// Simple throttle function
const useThrottle = (callback, delay) => {
	const [lastCall, setLastCall] = useState(0);

	return useCallback((...args) => {
		const now = Date.now();
		if (now - lastCall >= delay) {
			setLastCall(now);
			callback(...args);
		}
	}, [callback, delay, lastCall]);
};

export const Controls = ({ sendCommand }) => {
	const [brightness, setBrightness] = useState(50);
	const [speed, setSpeed] = useState(50);
	const [color, setColor] = useState("#0000ff");

	const throttledSend = useThrottle(sendCommand, 100);

	const handleBrightnessChange = (e) => {
		const val = e.target.value;
		setBrightness(val);
		throttledSend(`BRIGHTNESS:${val}`);
	};

	const handleSpeedChange = (e) => {
		const val = e.target.value;
		setSpeed(val);
		throttledSend(`SPEED:${val}`);
	};

	const handleColorChange = (e) => {
		const hex = e.target.value;
		setColor(hex);

		const r = parseInt(hex.substr(1, 2), 16);
		const g = parseInt(hex.substr(3, 2), 16);
		const b = parseInt(hex.substr(5, 2), 16);
		sendCommand(`COLOR:${r},${g},${b}`);
	};

	return (
		<div className="control-group">
			<div className="slider-container">
				<span>🔆 明るさ</span>
				<input
					type="range"
					min="0"
					max="100"
					value={brightness}
					className="slider"
					onChange={handleBrightnessChange}
				/>
				<span>{brightness}%</span>
			</div>
			<div className="slider-container">
				<span>🚀 速度</span>
				<input
					type="range"
					min="0"
					max="100"
					value={speed}
					className="slider"
					onChange={handleSpeedChange}
				/>
				<span>{speed}%</span>
			</div>
			<div className="color-picker-container">
				<span>🎨 自由色指定</span>
				<input
					type="color"
					value={color}
					onChange={handleColorChange}
				/>
			</div>
		</div>
	);
};
