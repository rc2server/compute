options(stringsAsFactors=FALSE)
cdf <- data.frame(c1=c(FALSE, TRUE, NA, NA, TRUE, FALSE),
                c2=c(3.14, NA, NaN, 21, Inf, -Inf),
                c3=c("Aladdin", NA, "NA", "Mario", "Mark", "Alex"),
                c4=c(1L, 2L, 3L, NA, 5L, NA))
cdf
