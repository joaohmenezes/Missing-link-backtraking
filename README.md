🧩 Missing Link Puzzle Solver

📌 About the Project
This project consists of an algorithm developed to automatically find the sequence of moves required to solve the **"Missing Link"** (known in Brazil as *Elo Maluco*), a cylindrical sliding puzzle highly popular in the 1980s.

The software was built using **C/C++** and applies **Object-Oriented Programming (OOP)** principles. To find the puzzle's solution, the algorithm implements advanced search and programming strategies, primarily the **Backtracking** method.

**Academic Context:** Project developed as an evaluation requirement for the **Design and Analysis of Algorithms (PAA)** course at the **Federal University of Itajubá (UNIFEI)**.

🚀 Features
* **Data Input:** The system allows initializing the game state by reading a structured `XML` file or by randomly generating an initial state.
* **Processing & Search:** Utilizes computational search techniques to explore possibilities until the path that solves the puzzle is found.
* **Step-by-Step Visualization:** Features the ability to display the current state of the puzzle on the screen at each step of the solution search.
* **Data Output:** Upon finding the solution, the program displays it on the screen and generates a new `XML` file containing the initial state and the exact sequence of steps taken to solve it.

⚙️ Representation and Encoding
The game state is mapped within the system using specific codes for colors and positions:
* **Colors:** Red (`vm`), Green (`vr`), Yellow (`am`), and White (`br`). The empty tile is represented by `vzo`.
* **Positions:** Top (`s`), Middle (`m`), and Bottom (`i`).

The actions (moves) the algorithm can perform to solve the puzzle include:
* `rsd` / `rse`: Rotate the top section to the right or left.
* `rid` / `rie`: Rotate the bottom section to the right or left.
* `mfc` / `mfb`: Move the tile below the empty space up, or the tile above the empty space down.

## 🛠️ Technologies Used
* **C/C++** (Main language)
* **TinyXML-2** (Library for `.xml` file parsing and manipulation)
