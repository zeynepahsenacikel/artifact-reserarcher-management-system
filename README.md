# Artifact & Researcher Management System (AVL & Red-Black Tree)
ArtifactManager is an advanced C++ application designed to manage a database of historical artifacts and the researchers assigned to them. The project focuses on high-performance data retrieval and storage by utilizing two of the most efficient self-balancing binary search trees.


## 🚀 Key Features
* Dual-Tree Architecture:

* AVL Tree: Manages Artifact objects indexed by their unique IDs. It ensures O(logn) time complexity for insertions and deletions by performing rotations to maintain a strict height balance.

* Red-Black Tree: Manages Researcher profiles indexed by their full names. It provides robust performance for dynamic data with fewer rotations than AVL during insertions/deletions.

* Complex Relationship Mapping: Handles the assignment of artifacts to researchers, respecting individual capacity limits and tracking researcher workloads.

* Dynamic Value Updates: Artifact research values are automatically boosted based on their rarity level every time they are assigned to a researcher.

* Safety & Integrity: Implements robust deletion logic. If a researcher is fired, all their supervised artifacts are automatically marked as UNASSIGNED. Similarly, if an artifact is removed, it is wiped from the supervisor's list.

* System Statistics: Provides real-time metrics including total artifact counts, average rarity, and average researcher load using post-order and pre-order tree traversals.


## 🛠 Project Structure
* AVLTree.cpp: Implementation of the AVL tree including LL, RR, LR, and RL rotations.

* RedBlackTree.cpp: Implementation of the Red-Black tree including color-based fixups and rotations.

* ArtifactManager.cpp: The central controller that parses commands and synchronizes data between both trees.

* Artifact.cpp & Researcher.cpp: Core entities representing the data handled by the system.


## 🎮 Command Interface
* The system supports a variety of management commands:

ADD_ARTIFACT <id> <name> <rarity> <value>: Inserts a new artifact into the AVL tree.

HIRE_RESEARCHER <name> <capacity>: Adds a researcher to the Red-Black tree.

REQUEST <researcherName> <artifactID>: Assigns an unassigned artifact to a researcher if they have capacity.

RETURN <researcherName> <artifactID>: Returns a specific artifact to the unassigned pool.

MATCH_RARITY <minRarity>: Lists all artifacts meeting a specific rarity threshold using in-order traversal.

PRINT_STATS: Generates a full system summary and displays tree structures.

CLEAR: Safely wipes all data and frees memory using recursive tree destruction.

## 💻 Compilation and Execution
* 1. Compilation

Since the project relies on multiple source files and C++11 features (like std::stoi), use the following command:

What this command does:

g++: Invokes the compiler.

-std=c++11: Ensures compatibility with modern C++ standards.

*.cpp: Compiles all source files, linking the logic across the different managers and trees.

-o ArtifactManager: Creates a run-ready executable named ArtifactManager.

* 2. Execution

Run the program in your terminal:

The program reads commands line-by-line from the standard input. You can also automate testing by redirecting a command file: ```./ArtifactManager < commands.txt```