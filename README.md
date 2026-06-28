# RSD-Hand-Eye-Coordination-Project
An autonomous closed-loop robotic sorting system integrating OpenCV computer vision, 8-bit parallel serial communication, and nested-subroutine control loops for dynamic physical color puzzle classification using the Intelitek Scorbot-ER 9.

# Autonomous Closed-Loop Sorting & Hand-Eye Coordination System

An industrial-grade robotics vision implementation featuring dynamic color identification, real-time hardware-in-the-loop serial processing, and fault-tolerant automated pick-and-place trajectories. Developed as part of the Robot System Design (RSD) curriculum using an Intelitek Scorbot-ER 9 system.

## 🚀 Key Highlights & Engineering Choices
* **Architectural Robustness:** Structured a multi-tier nested control loop matrix handling up to 256 physical runtime routing combinations.
* **Fail-Safe Initializers:** Implemented critical hardware protection parameters (`Open Gripper` routine entry states) to eliminate table/end-effector mechanical impacts and spatial calibration offsets.
* **Interprocess Serial Bridge:** Developed an efficient decimal-to-binary bit manipulation pipeline mapping frame classifications straight to a physical parallel port I/O system interface.
* **Interactive UI:** Features a native CLI console app giving administrators the granularity to switch between batch operations or selective single-color sorting states.

---

## 🛠 Tech Stack & Hardware Specs
* **Control Software:** SCORBASE Robotics System Suite
* **Manipulator:** Intelitek Scorbot-ER 9 (5-Axis Articulated Arm + Gripper, Payloads $\le$ 2kg, Repeatability: 0.09mm)
* **Computer Vision Suite:** OpenCV Core Engine (C++) via Visual Studio Build Environment
* **Data Transmission:** `libserialport` library executing customized asynchronous 8-bit structures over RS-232/Parallel Bus interfaces at 9600 Baud
* **Safety Protocols:** Triple-tier logic isolation loop (Safety Cage Microswitches + Control Unit E-Stops)

---

## 📐 System Architecture Diagram

[Camera Interface] ──(BGR Frame Grab)──> [OpenCV Matrix Parsing]
│
(Bit Concatenation)
│
▼
[Manipulator Arm] <──(Subroutine Jump)── [SCORBASE Controller] <──(8-Bit Parallel)

---

## 📂 Implementation Details

### 1. Computer Vision Processing (`.cpp` Pipeline)
The system bypasses typical highly latent classification models by accessing raw pixels at spatial matrix offsets ($y=230, x \in \{130, 190, 250\}$) corresponding directly to physical part drop points. 
* To filter out fluctuating laboratory environmental metrics, the application intentionally streams and dumps a baseline pool of 100 frames upon startup to stabilize exposure values.
* Custom analytical scripts convert dynamic array states into unique multi-digit conditional registers ($brg = BluePos \cdot 100 + RedPos \cdot 10 + GreenPos$).

### 2. Physical Subroutines Matrix (SCORBASE Control Loop)
* **Predefined State Patterns:** Triggers standard macro routines based on digital states (`BRG`, `BGR`, `RBG`, `GBR`, `GRB`, `RGB`).
* **Trajectory Pathing Protection:** Utilizes explicit intermediate elevated target points (Positions 7, 8, 9) preventing cross-trajectory collisions with adjacent inventory arrays. 
* **Safe Velocity Profiles:** Fixed at a disciplined 20% total performance threshold to optimize manipulator accuracy constraints.

---

## 🔮 Future Iterative Scalability: Luminance Robustness

To optimize this layout for fluctuating operational real-world constraints (e.g., shifts in lab environment lighting), the next framework update implements an analytical Euclidean calculation logic model. Instead of relying on static, hardcoded BGR thresholds, the pipeline switches to calculating immediate scalar distance metrics to determine target colors under unpredictable ambient light levels:

$$Magnitude = \sqrt{(B_{Actual} - B_{Ideal})^2 + (G_{Actual} - G_{Ideal})^2 + (R_{Actual} - R_{Ideal})^2}$$

The control pipeline evaluates absolute closest vector matching against reference spaces ($Blue_{Ideal} = (255, 0, 0)$) to select sorting configurations accurately under variable exposures.

---

## 👥 Contributor
* **Albert Lau** - Core Architecture & Computer Vision / Serial Protocol Engineer
