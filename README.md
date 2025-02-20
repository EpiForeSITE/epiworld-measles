# epiworld-measles
[![ForeSITE Group](https://github.com/EpiForeSITE/software/blob/e82ed88f75e0fe5c0a1a3b38c2b94509f122019c/docs/assets/foresite-software-badge.svg)](https://github.com/EpiForeSITE)

This project aims to generate an ABM using epiworldR to do scenario modeling of measles.

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
