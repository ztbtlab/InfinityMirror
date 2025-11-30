import React, { useState } from 'react';

const MODES = [
	{ id: 0, name: "オーロラ", emoji: "🌌", className: "aurora" },
	{ id: 1, name: "サイバーパンク", emoji: "🌆", className: "cyberpunk" },
	{ id: 2, name: "マトリックス", emoji: "🟩", className: "matrix" },
	{ id: 3, name: "炎", emoji: "🔥", className: "fire" },
	{ id: 4, name: "流星", emoji: "🌠", className: "meteor" },
	{ id: 5, name: "レインボー", emoji: "🌈", className: "rainbow" },
	{ id: 6, name: "スパークル", emoji: "✨", className: "sparkle" },
];

export const ModeGrid = ({ sendCommand }) => {
	const [activeMode, setActiveMode] = useState(null);

	const handleModeClick = (modeId) => {
		setActiveMode(modeId);
		sendCommand(`MODE:${modeId}`);
	};

	return (
		<div className="button-grid">
			{MODES.map((mode) => (
				<button
					key={mode.id}
					className={`mode-button ${mode.className} ${activeMode === mode.id ? 'active' : ''}`}
					onClick={() => handleModeClick(mode.id)}
				>
					<span className="emoji">{mode.emoji}</span>
					<span>{mode.name}</span>
				</button>
			))}
		</div>
	);
};
