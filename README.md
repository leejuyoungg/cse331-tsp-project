# Traveling Salesman Problem (TSP) Solver

CSE331 Algorithm Design — Project 2
Ulsan National Institute of Science and Technology (UNIST)

## Overview
Implementation and comparison of three TSP algorithms on TSPLIB benchmark instances,
including a novel proposed algorithm (AP-HK).

## Algorithms
- **MST-2× Approximation** — Builds a minimum spanning tree and performs a DFS
  preorder walk to construct a Hamiltonian cycle. Guaranteed 2-approximation. O(n²) time.
- **Held–Karp** — Exact dynamic programming solution using bitmask DP. O(n² · 2ⁿ) time.
  Becomes infeasible beyond n ≈ 24 due to memory limits.
- **Adaptive-Pruned Held–Karp (AP-HK)** *(proposed)* — Extends Held–Karp with
  layered double-buffering and lower-bound pruning. Matches Held–Karp exactly for
  k ∈ [6, 24] and scales to k ≈ 30–35.

## Benchmark Datasets (TSPLIB)
| Instance | n |
|---|---|
| a280 | 280 |
| XQL662 | 662 |
| kz9976 | 9,976 |
| mona-lisa100K | 100,000 |

## Key Results
| Algorithm | Approx. Ratio | Scalability |
|---|---|---|
| MST-2× | 1.35 – 1.43 | Up to n = 100,000 |
| Held–Karp | Exact (optimal) | Up to n ≈ 24 |
| AP-HK | Near-optimal | Up to n ≈ 30–35 |

## Language
C++

## Build & Run
```bash