# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

getArithmeticEuropeanPutPrice <- function(nInt, Strike, Spot, Vol, Rfr, Expiry, Barrier, nReps = 1000L) {
    .Call(`_EuropeanOption_getArithmeticEuropeanPutPrice`, nInt, Strike, Spot, Vol, Rfr, Expiry, Barrier, nReps)
}

rcpp_hello_world <- function() {
    .Call(`_EuropeanOption_rcpp_hello_world`)
}

