⸻

🖥️ CSOPESY Console OS Simulation

A simple C++ command-line interface (CLI) system simulation for CSOPESY (Operating Systems), showcasing custom command handlers, process management, threading, and screen navigation — all styled with a real operating system aesthetic.

⸻

🚀 Features
	•	🎮 Shell class to simulate CLI input/output
	•	🧵 Multithreaded execution (CLI + Scheduler threads)
	•	🧭 Screen navigation (MAIN_MENU, PROCESS_VIEW)
	•	🔧 report-util command to display & log active/finished processes
	•	🧾 screen command emulation with -s, -r, -ls flags
	•	✨ ASCII-art header splash for OS branding

⸻

🛠️ Build & Run

Make sure you have G++ with C++20 support (e.g., g++ 9.0+ recommended).

g++ -I src/ -std=c++20 -o main src/main.cpp && main

🔹 The -I src/ flag includes all your headers.
🔹 You must run this from the root directory of the project.

⸻

📁 Directory Structure

project-root/
│
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── shell/           # Shell logic
│   │   ├── scheduler/       # Scheduler, Process handling
│   │   ├── command/         # CLI commands and handlers
│   │   └── common/          # Utilities, types, imports
│
├── csopesylog.txt           # Generated process report (via report-util)
├── README.md


⸻

🧪 Example Command

Run this inside your main shell screen:

>>> report-util

This will:
	•	Display running and finished processes
	•	Save the same output into csopesylog.txt

⸻

👥 Authors

Dicayanan | Maramag | Maunahan | Villaver
“Like a real OS, but not written in assembly!”

⸻

Would you like a version with badge icons or markdown styling for GitHub?