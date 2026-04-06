# Hive-Board-Game

A C++ implementation of the classic abstract strategy board game **Hive** (by John Yianni). The game is playable both in console mode and with a graphical interface using **Qt**.

Hive is a two-player game where the goal is to surround your opponent’s Queen Bee with pieces from either side. The board is not fixed — it grows dynamically as pieces are placed on the play area. ([en.wikipedia.org](https://en.wikipedia.org/wiki/Hive_%28game%29))

---

## Features

* Full **C++ implementation** of Hive game logic
* **Console version** for terminal play
* **Graphical UI version** using the **Qt** framework
* Save/load support for games
* Implements core game rules and piece movements

---

## Requirements

* A C++ compiler supporting C++17
* Qt (version compatible with your development environment)
* Standard build tools (e.g., CMake or Qt Creator)

---

## Build & Run

1. **Clone the repository**
   git clone [https://github.com/etiennevzn/Hive-Board-Game.git](https://github.com/etiennevzn/Hive-Board-Game.git)
   cd Hive-Board-Game

2. **Build the project**

   * Using Qt Creator: open the `.pro` or CMake project and build
   * Or via command line with a supported build system

3. **Run**

   * Run the console version executable
   * Or launch the Qt graphical version

---

## Gameplay

Hive is an abstract strategy game with no fixed board. Players place hexagonal insect tiles that form the play surface. Each insect type has its own movement rules, and the objective is to trap the opponent’s queen bee completely. ([en.wikipedia.org](https://en.wikipedia.org/wiki/Hive_%28game%29))

---

## Repository Structure

Typical files include:

* `main.cpp` – Entry point
* `piece.hpp/cpp` – Piece definitions
* `plateau.hpp/cpp` – Board logic
* `partie.hpp/cpp` – Game state and flow
* `joueur.hpp/cpp` – Player representation
* `gamemanager.hpp/cpp` – High-level game manager

---

## Notes

This project was created as a Fall 2024 academic assignment for **LO21** at UTC. ([github.com]([https://github.com/etiennevzn/Hive-Board-Ga](https://github.com/etiennevzn/Hive-Board-Ga)
