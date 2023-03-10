####################################################################
# APPLIED FINANCE                                                  #
# Path-dependent option pricing with Monte Carlo and Rcpp package  #
# labs02: application of the Rcpp package                          #
# Paweł Sakowski                                                   #
# University of Warsaw                                             #
####################################################################

# loading packages
library(tidyverse)

# 1. remove package if it exists ===============================================
remove.packages("EuropeanOption")
detach("package:optionPricer2", unload = TRUE) # if it still is in memory

# 2. install package and load to memory ========================================
# (adjust file names and/or paths, if necessary)

# from binaries (no need to rebuild)
install.packages("packages/optionPricer2_1.0_R_x86_64-pc-linux-gnu.tar.gz",
                 type = "binaries",
                 repos = NULL)

# or from source (rebuilt automatically)
install.packages("AppliedFinanceEuropean/EuropeanOption_1.0.tar.gz",
                 type = "source",
                 repos = NULL)
library(EuropeanOption)
EuropeanOption::getArithmeticEuropeanPutPrice(126, 100, 95, 0.24, 0.07, 0.75, 80, 10000)
# 3. call the function from the package ========================================
# optionPricer2::getArithmeticAsianCallPrice(126, 100, 95, 0.2, 0.06, 0.5, 10000)

# 4. build an R wrapping function: option price vs. time to maturity ===========
getMCEuropeanCallPriceWithExpiry <- function (expiry) {
  return(
    EuropeanOption::getArithmeticEuropeanPutPrice(126, 100, 95, 0.24, 0.07, expiry, 80, 10000)
  )
}

# call the wrapping function 
getMCEuropeanCallPriceWithExpiry(0.5)

# arguments values of values of function 
expiry <- seq(0.01, 1, by = 0.01)
prices <- sapply(expiry, getMCEuropeanCallPriceWithExpiry)

# visualization: options price vs. expiry 
tibble( expiry, prices) %>%
  ggplot(aes(expiry, prices)) +
  geom_point(col = "red") +
  labs(
    x     = "time to maturity",
    y     = "option price",
    title = "price of arithmetic Asian call option vs. time  to maturity",
    caption = "source: own calculations with the optionPricer2 package")

# 5. build an R wrapping function: option price vs. number of loops ============
getMCEuropeanCallPriceWithLoops <- function (loops) {
  return(
    EuropeanOption::getArithmeticEuropeanPutPrice(126, 100, 95, 0.24, 0.07, 0.5, 80, loops)
  )
}

# call the wrapping function 
getMCEuropeanCallPriceWithLoops(500)

# arguments values of values of function 
loops  <- seq(100, 10000, by = 100)
prices <- sapply(loops, getMCEuropeanCallPriceWithLoops)

# visualization: options price vs. numbers of loops 
tibble(loops, prices) %>%
  ggplot(aes(loops, prices)) +
  geom_point(col = "blue") +
  labs(
    x     = "number of loops",
    y     = "option price",
    title = "price of arithmetic Asian call option vs. number of loops",
    caption = "source: own calculations with the optionPricer2 package")

# note the same seed within one second!

# 6. build an R wrapping function: option price vs. spot and volatility =======
getMCEuropeanCallPriceWithSpotAndVol <- function (spot, vol) {
  return(
    EuropeanOption::getArithmeticEuropeanPutPrice(126, 100, spot, vol, 0.07, 0.75, 80, 5000)
  )
  }

# call function once
getMCEuropeanCallPriceWithSpotAndVol(95, 0.2)

# sequences of argument values
spot <- seq(90, 105, by = 0.5)
vol  <- c(0.001, 0.01, 0.02, 0.05, 0.1, 0.15, 0.2, 0.3, 0.5, 1)

grid      <- expand.grid(spot = spot, vol = vol)
prices    <- mapply(getMCEuropeanCallPriceWithSpotAndVol, 
                    spot = grid$spot, vol = grid$vol)
result.df <- data.frame(grid, prices)
head(result.df)

# visualization: options price vs. spot price and volatility
grid %>% 
  as_tibble() %>%
  bind_cols(price = prices) %>%
  ggplot(aes(x = spot, y = price, group = vol, colour = vol)) +
  geom_line() +
  geom_point(size = 1, shape = 21, fill = "white") +
  labs(
    x     = "spot price",
    y     = "option price",
    title = "price of arithmetic Asian call option vs. spot price and volatility",
    caption = "source: own calculations with the optionPricer2 package")



