# BubbleSort vs BogoSort — Intelligence or Luck

This project presents an **experimental comparison between BubbleSort and BogoSort**, focusing on the contrast between a **deterministic algorithm** and a **purely random approach** to sorting.

The objective is not to propose efficient algorithms, but to **study empirically how algorithmic design, determinism, and randomness affect performance**.

---

## 📌 Description

Two sorting algorithms are compared:

- **BubbleSort**: a simple, deterministic sorting algorithm with quadratic time complexity.
- **BogoSort**: a randomized algorithm that repeatedly shuffles the array until it becomes sorted.

Although BogoSort is theoretically correct, its factorial expected complexity makes it practically infeasible for moderate input sizes.  
This project explores that infeasibility experimentally.

---

## 📊 What is measured

For each input size `n`, the following metrics are recorded:

- Execution time
- Number of comparisons
- Number of swaps
- Number of attempts (BogoSort)
- Number of failed executions (when a maximum limit is exceeded)

---

## 🧪 Methodology

- **BubbleSort** is executed a large number of times to obtain measurable and stable execution times.
- **BogoSort** is executed multiple times for each `n`, and results are averaged to reduce randomness.
- A **maximum number of attempts** is defined to prevent non-terminating executions.
- If BogoSort fails to sort within the limit, it is marked as *non-viable* for that `n`.
- All results are saved to a CSV file for later analysis and visualization.

To avoid bias:
- Measurement and visualization are strictly separated.
- BubbleSort is only used for visualization purposes when showing ordered arrays.

---

## 📁 Output

The program generates a file:


This file contains all measured data and can be used for:
- Spreadsheet analysis (Excel / LibreOffice)
- Python visualization (matplotlib, pandas)
- Animation and visualization tools (e.g. Manim)

---

## 🛠️ Compilation and execution

Example using `g++`:

```bash
g++ -std=c++17 main.cpp -o sorting_experiment
./sorting_experiment
