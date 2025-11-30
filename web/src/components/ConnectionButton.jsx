import React from 'react';

export const ConnectionButton = ({ onConnect, isConnected }) => {
	return (
		<button className="connect-button" onClick={onConnect} disabled={isConnected}>
			{isConnected ? "✅ 接続済み" : "🔗 BLE接続"}
		</button>
	);
};
