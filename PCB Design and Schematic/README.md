# PCB & Schematic Revision Notes
Current PCB validated the core functionality od the LawnBot, but after testing several areas for improvement became clear.

## 🧪 Key Lessons from the first PCB
* Peripheral Oversight: Original design did not account for motor encoders and ToF sensors, both of which are critical for precise navigation and closed-loop control.
* Noise Management: Lack of proper decoupling capacitors resulted in electrical noise that impacted PCB performance.
* Debugging Limitations: Without test points, indicator LEDs, and clear labeling, diagnosing hardware/software integration issues took longer than it should have.

## ✍️ Improvements in the Next Revision
* Encoder & ToF Integration for Automation: Dedicated connections for wheel encoders and distance sensors will be added for accurate motion control and autonomous driving.
* Noise Reduction: Strategic placement of decoupling capacitors and filtering components will stabilize power rails and improve overall reliability.
* Motor Driver Support: Circuit refinements to help the driver handle transients and reduce thermal stress. (Thicker tracks for higher currents, isolating motor driver from other more noise sensitive components).
* Debug-Friendly Design: Indicator LEDs for status monitoring, labeled headers, and test points for multimeter/oscilloscope probes.
* Better Documentation: A cleaner schematic and clearer silkscreen labeling to improve readability and handoff for other engineers.
    <img width="771" height="401" alt="Screenshot 2025-08-20 164518" src="https://github.com/user-attachments/assets/2472b3a8-05fd-46f1-a19e-66e5a8c821b2" />
    <img width="292" height="171" alt="image" src="https://github.com/user-attachments/assets/4da2c928-9d13-4071-955b-00709c5b8258" />
