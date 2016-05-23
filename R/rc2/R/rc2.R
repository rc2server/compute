helpIndexSearch <- getFromNamespace("index.search", getNamespace("utils"))

#called from EnvList.cpp c++ code
rc2.defineFunction <- function(func) {
	zz <- textConnection("def", open = "w", local = TRUE)
	capture.output(print(func), file=zz)
	close(zz)
	paste(def, sep="\n")
}

#override to just return help paths
help <- function(topic) {
#  thepath <- utils::help(...)
    ischar <- tryCatch(is.character(topic) && length(topic) == 
        1L, error = identity)
    if (inherits(ischar, "error")) 
        ischar <- FALSE
    if (!ischar) {
        reserved <- c("TRUE", "FALSE", "NULL", "Inf", "NaN", 
            "NA", "NA_integer_", "NA_real_", "NA_complex_", "NA_character_")
        stopic <- deparse(substitute(topic))
        if (!is.name(substitute(topic)) && !stopic %in% reserved) 
            stop("'topic' should be a name, length-one character vector or reserved word")
        topic <- stopic
    }
   thepath <- helpIndexSearch(topic, find.package())
  list(rc2type="help",path=thepath)
}

rc2.pngdev <- function()
{
	png("rc2img%03d.png")
}

rc2.pngoff <- function()
{
	if (dev.cur()[1] != 1)
		dev.off();
}
