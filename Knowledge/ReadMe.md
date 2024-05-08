### What is the project?
Goal: hobbyist-friendly, hackable multitouch trackpad on Pi Pico/RP2040.  
  
Motivation: RP2040 is a beast of a chip with a lot of wind in its sails; I want to enable more projects for it on the cheap (i.e. with very little & inexpensive additional hardware). Off-the-shelf ICs are what you _should_ use for production projects, here we are prototyping and hacking stuff together.

### Current status
- [x] Iteration 0: principal demonstrator. Single mutual capacitance cell made out of hand-cut copper foil
- [ ] Iteration 1: faithful original research reproduction on Pi Pico with factory PCB

### How to use
- Project docs are in "Knowledge" folder; I recommend using [Obsidian](https://obsidian.md/) to browse
- Code is served straight up as CMake project for Pi Pico; I am using VSCode, look up Pi Pico VSCode setup for easy build of the project
- PCB design is in EasyEDA format; it is a bit picky about where you store project files, but symlinks can help you work around that. Gerbers in "PCB/Fab"

Decision to not use github's wiki is conscious - that way docs (hopefully) reflect the state of the project at each and every commit. Besides, Obsidian is nice, give it a whirl!