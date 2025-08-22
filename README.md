# 🚗⚡ OBD2 RPM LED Visualizer

<div align="center">

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![BMW](https://img.shields.io/badge/BMW-0066CC?style=for-the-badge&logo=bmw&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)

*Transform your car's RPM data into a stunning visual experience! 🌈*

[🚀 Features](#-features) • [📷 Demo](#-demo) • [🔧 Installation](#-installation) • [⚙️ Configuration](#-configuration) • [🤝 Contributing](#-contributing)

</div>

---

## 🌟 What is This?

Ever wanted to see your engine's RPM in real-time with a cool LED strip? This project reads OBD2 data directly from your BMW and creates a **dynamic RPM tachometer** using WS2813 addressable LEDs!

### 🎯 **The Magic**
- **Real-time RPM visualization** 📊
- **Color-coded performance zones** 🌈
- **Warning blinks at redline** ⚠️
- **Plug-and-play OBD2 connection** 🔌

---

## 📹 Demo

### 🎬 **See It In Action!**

https://github.com/Arsalan134/BMW-OBD2/blob/master/🚙%20bmw_rpm.mp4

*Watch your BMW's engine come alive with real-time RPM visualization!*

### 🌈 **What You'll See:**
- **Smooth Color Transitions**: Yellow to red gradient based on RPM
- **Real-time Response**: Instant LED updates as engine speed changes
- **Warning System**: Dramatic red blinking at high RPM
- **Rainbow Standby**: Beautiful flowing colors when engine is off

### 📊 **Performance Zones:**
```
🟡 Idle Zone (700-1500 RPM)    → Yellow/Green LEDs
🟠 Cruise Zone (1500-3000 RPM) → Orange LEDs  
🔴 Power Zone (3000-5000 RPM)  → Red LEDs
⚠️ Danger Zone (5000+ RPM)     → Blinking Red Warning!
```

---

## 🚀 Features

- **🌈 Real-time RPM Visualization**: Dynamic LED strip that fills up based on engine RPM
- **⚠️ Redline Warning**: Aggressive blinking when RPM exceeds safe limits
- **🔆 Adjustable Brightness**: 4 brightness levels controlled by a physical button
- **🎯 BMW Optimized**: Pre-configured RPM ranges for BMW engines
- **🔧 Dual Environment**: Test mode for bench testing without a running engine

## � Installation

### �🛠️ Hardware Requirements

| Component | Specification | Purpose |
|-----------|---------------|---------|
| **Arduino Uno** | ATmega328P | Main controller |
| **OBD2 Shield/Module** | CAN Bus compatible | Read vehicle data |
| **WS2813 LED Strip** | 9 LEDs (configurable) | Visual display |
| **Push Button** | Momentary switch | Brightness control |
| **Resistors** | 10kΩ pull-up | Button debouncing |

## 🔌 Wiring Diagram

```
Arduino Uno    →    Component
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

## ⚙️ Configuration

### 🔧 Customize RPM Ranges
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

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for full details.

### What does this mean?
- ✅ **Commercial Use** - You can use this code in commercial projects
- ✅ **Modification** - You can modify and improve the code
- ✅ **Distribution** - You can share and redistribute the code
- ✅ **Private Use** - You can use it for personal projects
- ℹ️ **Attribution Required** - Just include the original license notice

The MIT License is one of the most permissive open-source licenses, making it easy for others to use and contribute to this project!

## 👨‍💻 Author

**Arsalan Iravani**
- 🌐 [LinkedIn](https://www.linkedin.com/in/airavani/)
- 📧 Email: [Contact](mailto:airavani2018@gmail.com)

*iOS Developer & Electronics Enthusiast passionate about bridging digital and physical worlds*

## ⚠️ Disclaimer

This project is for educational and entertainment purposes. Always ensure safe driving practices and comply with local traffic laws. Modifications to vehicle electronics should be performed by qualified professionals.

---

**Made with ❤️ for BMW enthusiasts and Arduino hackers alike!**
