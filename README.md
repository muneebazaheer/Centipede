# Centipede
A simplified version of the Centipede arcade game implemented using C++ based on concepts from Programming Fundamentals

Here’s a well-structured **GitHub README** file for your **Centipede game** project. It includes a **description, setup guide, features, controls, and a contribution section**.

---

## **Centipede Game**
A simplified version of the **Centipede arcade game** implemented using C++.

---

About the Game:  
Centipede is a classic fixed shooter arcade game originally developed by Atari, Inc. in 1981. The player controls a fighter at the bottom of the screen and shoots at an advancing centipede while avoiding other creatures like spiders, scorpions, and fleas.  

This project aims to recreate a simplified version of the game while maintaining the core gameplay mechanics.

🔹 Developer: Muneeba Zaheer 
🔹 Language: C++  
🔹 Platform: Console-based  

---

Gameplay:

Fighter vs. Centipede:
- The screen is divided into rows and columns (2D grid).  
- The player controls a fighter at the bottom of the screen.  
- The centipede consists of **12 segments** at Level 1 and moves across the screen, descending when hitting an obstacle.  
- The player moves left, right, and up in the player area while shooting lasers.  
- Shooting a centipede segment splits it into two —each part continues moving independently.  
- If only the head remains and is destroyed, the entire centipede segment disappears.  
- If a centipede reaches the bottom, it **moves back and forth** in the player area.  
- Destroying the centipede completely progresses to the next level.  
- **Mushrooms** are scattered randomly and can be destroyed by shooting them (takes two shots).  

---

Features:

✅ Centipede Movement: Moves left and right, descending when hitting mushrooms or screen edges.  
✅ Laser Shooting: The player can shoot lasers to destroy centipede segments.  
✅ Mushroom Mechanics: Centipede interacts with mushrooms and changes behavior.  

---

Scoring System:

| Destroy Mushroom --> 1 |
| Destroy Centipede Body --> 10 |
| Destroy Centipede Head --> 20 |
| Regenerate Mushroom --> 5 |
| Destroy Spider --> 300, 600, 900 (based on proximity) |
| Destroy Scorpion --> 1,000 |
| Extra Life --> Every 10,000 to 20,000 points |

Controls:

🎮 Arrow Keys - Move the Fighter  
🎮 Spacebar - Fire Laser  
🎮 Esc - Quit Game  
---

How to Play  

🚀 Goal: Destroy the centipede before it reaches the bottom!  
- Move your fighter in the lower area using arrow keys.  
- Shoot at the centipede to destroy segments.  
- Avoid touching the centipede or poisonous mushrooms!  
- Advance to the next level by eliminating all enemies.  