document.addEventListener('DOMContentLoaded', () => {
    const temperatureElem = document.getElementById('temperature');
    const minTemperatureElem = document.getElementById('min-temperature');
    const maxTemperatureElem = document.getElementById('max-temperature');
    const avgTemperatureElem = document.getElementById('average-temperature');

    const modeElem = document.getElementById('mode');
    const windowOpeningElem = document.getElementById('window-opening');
    const systemStateElem = document.getElementById('system-state');

    const windowSlider = document.getElementById('window-slider');
    const setOpeningButton = document.getElementById('set-opening');
    const resolveAlarmButton = document.getElementById('resolve-alarm');

    const switchToAutomaticButton = document.getElementById('switch-to-automatic');
    const switchToManualButton = document.getElementById('switch-to-manual');

    const ctx = document.getElementById('temperature-chart').getContext('2d');

    const temperatureChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: Array(20).fill(''), // Placeholder per i label
        datasets: [{
          label: 'Temperature',
          data: Array(20).fill(0), // Placeholder per i dati
          borderColor: 'rgba(75, 192, 192, 1)',
          borderWidth: 1,
          tension: 0.4
        }]
      },
      options: {
        scales: {
          x: { display: false },
          y: {
            beginAtZero: true,
            title: { display: true, text: 'Temperature (°C)' }
          }
        }
      }
    });

    // Fetch system data
    function fetchSystemState() {
      fetch('/api/system-state')
        .then(response => response.json())
        .then(data => {
          temperatureElem.textContent = data.temperature.toFixed(1);
          minTemperatureElem.textContent = data.minTemperature.toFixed(1);
          maxTemperatureElem.textContent = data.maxTemperature.toFixed(1);
          avgTemperatureElem.textContent = data.averageTemperature.toFixed(1);
          modeElem.textContent = data.mode;
          windowOpeningElem.textContent = data.windowOpening;
          systemStateElem.textContent = data.systemState;

          //Grafico
          const temperatureHistory = data.temperatureHistory; // Array JSON
          temperatureChart.data.labels = Array(temperatureHistory.length).fill('');
          temperatureChart.data.datasets[0].data = temperatureHistory;
          temperatureChart.update();
        })
        .catch(error => console.error('Error fetching system state:', error));
    }

    function switchMode(newMode) {
      fetch('/api/switch-mode', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ mode: newMode }),
      })
        .then(response => response.json())
        .then(data => {
          if (data.status === 'success') {
            alert(`Switched to ${data.newMode} mode successfully!`);
            fetchSystemState(); // Aggiorna lo stato del sistema
          }
        })
        .catch(error => console.error('Error switching mode:', error));
    }

    // Set manual window opening
    setOpeningButton.addEventListener('click', () => {

      const opening = parseInt(windowSlider.value, 10);
      fetch('/api/manual-mode', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ windowOpening: opening }),
      })
        .then(response => response.json())
        .then(data => {
          if (data.status === 'success') {
            alert('Window opening set successfully!');
            fetchSystemState(); // Refresh state
          }
        })
        .catch(error => console.error('Error setting window opening:', error));
    });

    // Resolve alarm
    resolveAlarmButton.addEventListener('click', () => {
      fetch('/api/resolve-alarm', { method: 'POST' })
        .then(response => response.json())
        .then(data => {
          if (data.status === 'success') {
            alert('Alarm resolved successfully!');
            fetchSystemState(); // Refresh state
          }
        })
        .catch(error => console.error('Error resolving alarm:', error));
    });

    // Event listener per cambiare modalità
    switchToAutomaticButton.addEventListener('click', () => switchMode('auto'));
    switchToManualButton.addEventListener('click', () => switchMode('manual'));

    // Initial fetch of system data
    fetchSystemState();
  });
