# Check-In 2 Preparation Checklist

## 📋 Pre-Meeting Checklist

Use this checklist to ensure you're ready for your 5-minute meeting with Professor Abourbih on November 24th.

---

## ✅ Code Requirements

### Arduino Code
- [ ] Arduino code file exists: `Arduino/smarthome_checkin2.ino`
- [ ] Code compiles without errors
- [ ] Code uploaded to Arduino Uno successfully
- [ ] DHT11 library installed and working
- [ ] LED control functions working
- [ ] Serial communication at 9600 baud
- [ ] Code includes comments explaining functionality
- [ ] Telemetry sends JSON formatted data
- [ ] REST commands properly parsed

### Python UI Code
- [ ] Python UI file exists: `UI/smarthome_ui.py`
- [ ] Pyserial library installed (`pip install pyserial`)
- [ ] Code runs without errors
- [ ] Menu displays correctly
- [ ] Telemetry data displays in real-time
- [ ] LED commands work (ON/OFF)
- [ ] CSV data storage working
- [ ] Data summary view working

---

## ✅ Hardware Requirements

### Physical Setup
- [ ] Arduino Uno connected via USB
- [ ] DHT11 sensor wired correctly:
  - [ ] VCC → 5V
  - [ ] DATA → Pin 2
  - [ ] GND → GND
- [ ] LED circuit wired correctly:
  - [ ] LED(+) → 220Ω resistor → Pin 13
  - [ ] LED(−) → GND
- [ ] All connections secure on breadboard
- [ ] No loose wires
- [ ] Correct COM port identified

### Hardware Testing
- [ ] Temperature readings are reasonable (15-30°C)
- [ ] Humidity readings are reasonable (30-70%)
- [ ] LED turns ON when commanded
- [ ] LED turns OFF when commanded
- [ ] No hardware errors in serial output

---

## ✅ Documentation Requirements

### README.md Must Include:
- [ ] Team member table with roles (from Check-In 1)
- [ ] **Updated** problem description (clearer than Check-In 1)
- [ ] **Updated** solution description (clearer than Check-In 1)
- [ ] API use table (MQTT and REST API)
- [ ] Planned hardware table
- [ ] Wiring diagram image embedded
- [ ] **NEW:** Architecture diagram embedded
- [ ] **NEW:** Setup instructions section
- [ ] **NEW:** Current status section
- [ ] Repository structure listed

### Documentation Folder
- [ ] `/Documentation` folder exists
- [ ] Wiring diagram image in `/Documentation`
- [ ] Wiring diagram accurate (updated if changed)
- [ ] **NEW:** Architecture diagram in `/Documentation`
- [ ] All images properly named (e.g., `wiring_diagram.png`, `architecture_diagram.png`)

### Architecture Diagram Must Show:
- [ ] User layer
- [ ] Python UI layer
- [ ] Serial communication layer
- [ ] Arduino layer
- [ ] Hardware components (DHT11, LED)
- [ ] MQTT telemetry data flow arrows
- [ ] REST API command flow arrows
- [ ] Clear labels on all components

---

## ✅ Demonstration Requirements

### Must Demonstrate Working:
- [ ] At least ONE sensor producing data (DHT11) ✓
- [ ] At least ONE output being controlled (LED) ✓
- [ ] Data flowing through ONE API (MQTT telemetry) ✓
- [ ] Commands flowing through SECOND API (REST control) ✓
- [ ] Simple UI action (display temperature) ✓
- [ ] Simple control action (LED ON/OFF) ✓

### End-to-End Path Working:
- [ ] DHT11 → Arduino → Serial → Python UI → Display (MQTT path)
- [ ] Python UI → Serial → Arduino → LED (REST API path)
- [ ] Both paths work simultaneously
- [ ] Can demonstrate live during meeting

---

## ✅ GitHub Requirements

### Repository Structure
- [ ] Private repository created
- [ ] Professor (thecodeprofessor) has access
- [ ] All team members have access
- [ ] At least TWO code directories exist (e.g., `Arduino/` and `UI/`)
- [ ] `/Documentation` folder exists
- [ ] README.md exists in root

### Commit Requirements
- [ ] Each team member has at least 4 commits since Check-In 1
  - [ ] At least 2 code commits per member
  - [ ] At least 2 documentation commits per member
- [ ] All team members committing from their own GitHub accounts
- [ ] Commit messages are meaningful (not just "update")
- [ ] Commits show real work (not just whitespace changes)

### Commit Distribution Check:
- [ ] Member 1 (Pritika): _____ commits (need 4+)
- [ ] Member 2 (Sikai): _____ commits (need 4+)
- [ ] Member 3 (Jeremiah): _____ commits (need 4+)
- [ ] Member 4 (Jaskaran): _____ commits (need 4+)

**How to check:** `git log --oneline --all` or view on GitHub

---

## ✅ API Implementation

### MQTT (Telemetry)
- [ ] Purpose clearly defined: continuous temperature streaming
- [ ] Hardware mapped: DHT11 sensor
- [ ] Data format: JSON messages
- [ ] Frequency: Every 2 seconds
- [ ] Direction: Arduino → Python UI
- [ ] Working demonstration ready

### REST API (Commands)
- [ ] Purpose clearly defined: LED control and status queries
- [ ] Hardware mapped: LED
- [ ] Data format: Command strings + JSON responses
- [ ] Pattern: Request-response
- [ ] Direction: Python UI ↔ Arduino
- [ ] Working demonstration ready

---

## ✅ Data Storage

### CSV File
- [ ] `sensor_data.csv` file created automatically
- [ ] Contains columns: Timestamp, Temperature, Humidity, Device
- [ ] Data saves correctly with each reading
- [ ] Can be opened in Excel or text editor
- [ ] File persists after program closes

### Data Summary
- [ ] Can view recent readings in UI (option 4)
- [ ] Shows last 5 readings
- [ ] Shows averages
- [ ] Shows total count
- [ ] Format is readable

---

## ✅ Meeting Preparation

### What to Bring/Show:
- [ ] Laptop with Arduino connected
- [ ] System running and working
- [ ] GitHub repository open in browser
- [ ] README.md visible
- [ ] Architecture diagram visible
- [ ] Wiring diagram visible

### What to Explain (2 minutes):
- [ ] Our problem: Small room monitoring
- [ ] Our solution: Arduino + DHT11 + LED + Python UI
- [ ] MQTT use: Continuous temp streaming
- [ ] REST API use: LED commands
- [ ] Two hardware: Sensor (DHT11) + Output (LED)

### What to Demonstrate (2 minutes):
- [ ] Show temperature streaming (MQTT)
- [ ] Show LED control (REST API)
- [ ] Show data in CSV file
- [ ] Show GitHub repository structure

### Questions to Be Ready For:
- [ ] "Why did you choose MQTT for temperature data?"
  - Answer: Continuous streaming without polling, efficient for telemetry
- [ ] "Why did you choose REST API for LED control?"
  - Answer: Request-response pattern fits control commands, clear state changes
- [ ] "Show me your architecture diagram"
  - Be ready to explain data flow
- [ ] "How does your system meet the two API requirement?"
  - MQTT for telemetry, REST for control, different purposes
- [ ] "Show me a commit from each team member"
  - Have GitHub contributors page ready

---

## ✅ Final Pre-Meeting Tests

### 30 Minutes Before Meeting:
- [ ] Upload fresh Arduino code
- [ ] Test DHT11 readings (should be reasonable)
- [ ] Test LED ON command (should light up)
- [ ] Test LED OFF command (should turn off)
- [ ] Test data summary (should show readings)
- [ ] Check sensor_data.csv has recent data
- [ ] Refresh GitHub page to show latest commits
- [ ] Close Arduino IDE Serial Monitor

### 5 Minutes Before Meeting:
- [ ] Arduino connected and powered
- [ ] Python UI running
- [ ] Temperature streaming visible
- [ ] GitHub repository open in browser
- [ ] README.md scrolled to show architecture diagram
- [ ] Team members know their roles

---

## ✅ Common Issues - Fix Before Meeting

### If Temperature Shows "ERROR":
- [ ] Check DHT11 wiring (especially DATA to Pin 2)
- [ ] Wait 2-3 seconds after power on
- [ ] Try different DHT11 if available
- [ ] Check DHT library installed

### If LED Doesn't Work:
- [ ] Check LED polarity (long leg = positive)
- [ ] Check resistor connection
- [ ] Check Pin 13 connection
- [ ] Try different LED if available

### If Python Can't Connect:
- [ ] Close Arduino IDE Serial Monitor
- [ ] Check COM port number
- [ ] Try different USB port
- [ ] Check pyserial installed: `pip install pyserial`

### If No Commits Showing:
- [ ] Make sure you pushed to GitHub: `git push`
- [ ] Check you're on correct branch
- [ ] Verify GitHub username is correct
- [ ] Check repository privacy settings

---

## 📊 Check-In 2 Grading Criteria (2.5%)

Based on the assignment description:

### What's Being Graded:
1. **Working Prototype (40%):**
   - At least one sensor producing data ✓
   - Data flows through one API to UI ✓
   - Can demonstrate end-to-end path ✓

2. **Code Quality (20%):**
   - Arduino code works
   - Python UI code works
   - Code is organized in proper directories

3. **Documentation (20%):**
   - README.md updated with all required sections
   - Architecture diagram included
   - Setup steps documented
   - Problem/solution descriptions clearer

4. **GitHub Usage (10%):**
   - Each member has 4+ commits
   - Commits show real work
   - Repository well organized

5. **Team Meeting (10%):**
   - All members present
   - Can explain the system
   - Can demonstrate working prototype
   - Can answer questions

---

## 🎯 Success Indicators

You're ready if:
- ✓ You can run the system start to finish without errors
- ✓ Temperature data streams continuously
- ✓ LED responds to commands
- ✓ GitHub shows commits from all members
- ✓ README.md is complete with diagrams
- ✓ You can explain MQTT vs REST usage
- ✓ You can show both hardware components working

---

## 📝 Last-Minute Checklist (Day Of)

**1 Hour Before:**
- [ ] Pull latest GitHub changes
- [ ] Test entire system
- [ ] Charge laptop fully
- [ ] Have Arduino USB cable ready

**15 Minutes Before:**
- [ ] System running and working
- [ ] GitHub repository loaded in browser
- [ ] Know who will speak for each part

**During Meeting:**
- [ ] Stay calm and confident
- [ ] Point at specific parts when explaining
- [ ] If something breaks, explain what it should do
- [ ] Thank professor at the end

---

## 🚀 You're Ready!

If you've checked off all items above, you're well prepared for Check-In 2. Good luck!

**Remember:**
- This is a "walking skeleton" - it doesn't need to be perfect
- Focus on proving your design works end-to-end
- Show you understand MQTT vs REST API concepts
- Demonstrate equal team contribution

---

**Team SmartSense** - You've got this! 💪
