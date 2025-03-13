# epiworld-measles
[![ForeSITE Group](https://github.com/EpiForeSITE/software/blob/e82ed88f75e0fe5c0a1a3b38c2b94509f122019c/docs/assets/foresite-software-badge.svg)](https://github.com/EpiForeSITE)

> [!CAUTION]
> This project is a work in progress. Use it at your own risk.
> **This model simulates a single school, so community transmission is not included**

This project aims to generate an ABM using [epiworld](https://github.com/UofUEpiBio/epiworld) to do scenario modeling of measles.

An ABM in which we pass the following inputs: 
- School size 
- If it is elementary/mid/high school (three different models) 
- Seed cases 
- Vaccination rate (which we would pull from DHHS) 
- Population distribution (gender/age/etc) (we may need to do some lit review on this). 
- Checklist on what interventions we would like to execute (TBD). 

As outputs (with/without the interventions) 
- Number of cases as time series. 
- Severity of the outbreak in terms of $$$ (Rich team will help). 
- The impact in how much time kids are taken out of school. 
- Generate some baseline figures of these statistics. 

# Data sources

- Immunization dashboard by Utah's DHHS: https://avrpublic.dhhs.utah.gov/imms_dashboard/
- Measles disease plan by Utah's DHHS: https://epi.utah.gov/wp-content/uploads/Measles-disease-plan.pdf

# Included models

- Contact tracing model ([contact_tracing](contact_tracing)): Contact tracing is performed and not all unvaccinated individuals are moved to quarantine.
- School quarantine model ([school_quarantine](school_quarantine)): There is no contact tracing and all unvaccinated individuals are moved to quarantine.