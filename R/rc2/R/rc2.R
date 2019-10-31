helpIndexSearch <- getFromNamespace("index.search", getNamespace("utils"))

utils::suppressForeignCheck("rc2cache")
rc2cache <- new.env(parent = emptyenv())
assign("imgNum", 100, envir=rc2cache)

#called from EnvList.cpp c++ code
rc2.defineFunction <- function(func) {
	zz <- textConnection(NULL, open = "w", local = TRUE)
	capture.output(print(func), file=zz)
	result <- paste(textConnectionValue(zz), sep="\n")
	close(zz)
	result
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
	imgNum <- as.integer(get("imgNum", envir=rc2:::rc2cache))
	assign("imgNum", imgNum + 1, envir=rc2:::rc2cache)
	fname <- sprintf("rc2img%03d-%%d.png", imgNum, envir=rc2:::rc2cache)
	png(fname)
	rc2.errlog("rc2.imgstart=%s\n", fname)
}

rc2.pngoff <- function()
{
	if (dev.cur()[1] != 1)
		dev.off();
}


rc2.errlog <- function(...) cat(sprintf(...), sep='', file=stderr())

