# 🏎️ BMW OBD2 RGB Tachometer 

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for full details.

### What does this mean?
- ✅ **Commercial Use** - You can use this code in commercial projects
- ✅ **Modification** - You can modify and improve the code
- ✅ **Distribution** - You can share and redistribute the code
- ✅ **Private Use** - You can use it for personal projects
- ℹ️ **Attribution Required** - Just include the original license notice

The MIT License is one of the most permissive open-source licenses, making it easy for others to use and contribute to this project!.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange.svg)](https://platformio.org/)

**Transform your BMW's RPM data into a mesmerizing LED light show!**

This Arduino-based project reads real-time RPM data from your BMW's OBD2 port and visualizes it using a stunning WS2813 RGB LED strip. Watch as your engine's heartbeat comes to life with dynamic colors, smooth gradients, and warning blinks at redline!

![BMW RPM Visualization](bmw_rpm.mp4)

## ✨ Features

- **🌈 Real-time RPM Visualization**: Dynamic LED strip that fills up based on engine RPM
- **⚠️ Redline Warning**: Aggressive blinking when RPM exceeds safe limits
- **🔆 Adjustable Brightness**: 4 brightness levels controlled by a physical button
- **🎯 BMW Optimized**: Pre-configured RPM ranges for BMW engines
- **🔧 Dual Environment**: Test mode for bench testing without a running engine

## 🛠️ Hardware Requirements

| Component | Specification | Purpose |
|-----------|---------------|---------|
| **Arduino Uno** | ATmega328P | Main controller |
| **OBD2 Shield/Module** | CAN Bus compatible | Read vehicle data |
| **WS2813 LED Strip** | 9 LEDs (configurable) | Visual display |
| **Push Button** | Momentary switch | Brightness control |
| **Resistors** | 10kΩ pull-up | Button debouncing |

## 🔌 Wiring Diagram

```
Arduino Uno     →    Component
Pin 3          →    LED Strip Data (WS2813)
Pin 7          →    Button (with 10kΩ pull-up)
5V             →    LED Strip VCC
GND            →    LED Strip GND + Button GND
OBD2 Shield    →    Stacked on Arduino
```

## 📊 RPM Mapping

The LED strip responds to different RPM ranges with stunning visual effects:

### 🧪 Test Environment (Bench Testing)
- **Idle Range**: 700-2000 RPM → Smooth yellow-to-red gradient
- **Warning Zone**: 2300+ RPM → Aggressive red blinking
- **LED Count**: 9 LEDs for precise visualization

### 🏁 Real Environment (In-Vehicle)
- **Operating Range**: 3000-5000 RPM → Dynamic color mapping
- **Danger Zone**: 5200+ RPM → Critical warning blinks
- **Engine Off**: <100 RPM → Rainbow standby mode

## 🎮 Controls

- **Short Press**: Cycle through 4 brightness levels (5, 50, 150, 255)
- **Long Press** (1s+): Reserved for future features
- **Auto-Off**: LEDs turn off when engine is stopped (<100 RPM)

## 🚀 Quick Start

### 1. Environment Setup
```bash
# Install PlatformIO (if not already installed)
pip install platformio

# Clone the repository
git clone <your-repo-url>
cd BMW-OBD2
```

### 2. Choose Your Environment
Edit `include/main.h`:
```cpp
// Comment this line for real BMW testing
#define isTesting  // For bench testing

// Uncomment for real vehicle use
// #undef isTesting
```

### 3. Build & Upload
```bash
# Build the project
pio run

# Upload to Arduino
pio run --target upload

# Monitor serial output
pio device monitor
```

### 4. Installation
1. Connect your Arduino to the BMW OBD2 port
2. Mount the LED strip in your desired location
3. Secure all connections and test functionality

## 🎨 Visual Modes

### 🌈 Rainbow Mode (Standby)
When the engine is off or RPM is below threshold, enjoy a mesmerizing rainbow pattern that flows across the LED strip with dynamic brightness variations.

### 📈 RPM Visualization Mode
- **Yellow Zone**: Low RPM - Efficient driving range
- **Orange Zone**: Medium RPM - Performance driving
- **Red Zone**: High RPM - Maximum performance
- **Blinking Red**: Danger zone - Protect your engine!

## 🔧 Configuration

### Customize RPM Ranges
```cpp
// Edit in main.h
#define RPM_MIN 700               // Start visualization
#define RPM_MAX 2000              // Full LED strip
#define BLINK_RPM_THRESHOLD 2300  // Warning threshold
```

### Adjust LED Count
```cpp
#define NUM_LEDS 9  // Change to your strip length
```

### Modify Blink Speed
```cpp
#define BLINK_DURATION 75  // milliseconds
```

## 📚 Libraries Used

- **[OBD2 Library](https://github.com/sandeepmistry/arduino-OBD2)** - Communication with vehicle ECU
- **[FastLED](https://fastled.io/)** - High-performance LED control
- **Arduino CAN** - CAN bus communication

## 🔍 Troubleshooting

| Issue | Solution |
|-------|----------|
| No RPM data | Check OBD2 connection and vehicle compatibility |
| LEDs not responding | Verify wiring and power supply |
| Incorrect RPM values | Adjust RPM_MIN/MAX in configuration |
| Button not working | Check pull-up resistor and wiring |

## 🎯 Future Enhancements

- [ ] **Multi-Parameter Display**: Show speed, coolant temp, fuel level
- [ ] **Wireless Connectivity**: Bluetooth/WiFi data streaming
- [ ] **Mobile App**: Remote monitoring and configuration
- [ ] **Data Logging**: SD card storage for performance analysis
- [ ] **Custom Profiles**: Different visual themes per driving mode

## 🤝 Contributing

Feel free to contribute to this project! Here's how:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## � License

This project is licensed under the MIT License - see the `LICENSE` file for details.

## 👨‍💻 Author

**Arsalan Iravani**
- 🌐 [LinkedIn](https://www.linkedin.com/in/airavani/)
- 📧 Email: [Contact](mailto:airavani2018@gmail.com)

*iOS Developer & Electronics Enthusiast passionate about bridging digital and physical worlds*

## ⚠️ Disclaimer

This project is for educational and entertainment purposes. Always ensure safe driving practices and comply with local traffic laws. Modifications to vehicle electronics should be performed by qualified professionals.

---

**Made with ❤️ for BMW enthusiasts and Arduino hackers alike!**
