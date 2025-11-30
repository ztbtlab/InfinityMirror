import React from 'react';
import { Header } from './components/Header';
import { ConnectionButton } from './components/ConnectionButton';
import { Controls } from './components/Controls';
import { ModeGrid } from './components/ModeGrid';
import { useBLE } from './hooks/useBLE';

function App() {
  const { connect, sendCommand, isConnected, status } = useBLE();

  return (
    <div className="container">
      <Header />

      <ConnectionButton onConnect={connect} isConnected={isConnected} />

      <Controls sendCommand={sendCommand} />

      <ModeGrid sendCommand={sendCommand} />

      <div className="status" id="status">
        {status}
      </div>
    </div>
  );
}

export default App;
