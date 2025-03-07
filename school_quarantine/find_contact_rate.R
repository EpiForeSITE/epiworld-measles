R0 <- function(
  contact_rate,
  transmission_rate = .9,
  promodral_period = 3.0,
  rash_period = 4.0
  ) {

  contact_rate * transmission_rate * (
    promodral_period + rash_period
    )

}

find_crate <- function(R0_target, ..., optim_args = list()) {

  fun <- function(x) {
    (R0_target - R0(x, ...)) ^ 2
  }

  optim_args$method <- "L-BFGS-B"
  do.call(optim, c(list(1, fun), optim_args))

}

crate_optim <- find_crate(15)$par
R0(crate_optim)

find_crate_exact <- function(
  R0_target,
  transmission_rate = .9,
  promodral_period = 3.0,
  rash_period = 4.0) {

  R0_target / (transmission_rate * (promodral_period + rash_period))

}

# Calibration testing
find_crate_exact(
  11, transmission_rate = .7, promodral_period = 3, rash_period = 2
  )

# Davis
find_crate_exact(
  15,
  transmission_rate = .9,
  promodral_period = 3,
  rash_period = 4
  )

