> [!CAUTION]
> This project is a work in progress. Use it at your own risk. **This model simulates a single school, so community transmission is not included**

> [!CAUTION]
> The code and model are still under development. We would love to hear your feedback and suggestions. Please open an issue in the [GitHub repository](https://github.com/EpiForeSITE/epiworld-measles) if you have any questions or suggestions.

# Models of school quarantine

The different runs here include:

- New Bridge 88: 88% vaccination rate, no NPIs ([link](./new_bridge_88)).
- New Bridge 88 NPI: 88% vaccination rate, NPIs ([link](./new_bridge_88_npi)).
- Canyon grove 58: 58% vaccination rate, no NPIs ([link](./canyon_grove_58)).
- Canyon grove 58 NPI: 58% vaccination rate, NPIs ([link](./canyon_grove_58_npi)).
- Edgemont 92: 92% vaccination rate, no NPIs ([link](./edgemont_92)).
- Edgemont 92 NPI: 92% vaccination rate, NPIs ([link](./edgemont_92_npi)).
- Canyon Crest 74: 74% vaccination rate, no NPIs ([link](./canyon_crest_74)).
- Canyon Crest 74 NPI: 74% vaccination rate, NPIs ([link](./canyon_crest_74_npi)).

The NPI runs have quarantine willingness set to 1.0, which means that all individuals will accept quarantine if they develop the disease. Otherwise, there's no quarantine or isolation whatsoever.

All runs were calibrated using an R0 of 15.
