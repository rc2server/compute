oldVariableValues <- new.env()
helpIndexSearch <- getFromNamespace("index.search", getNamespace("utils"))

#figures out the symbol names from the global env that have changed since last call
#called by listVariables
determineDeltaSymbols <- function(syms) {
	oldSyms <- ls(all=TRUE, envir=oldVariableValues)
	deltaSyms <- vector(mode="character")
	parEnv <- .GlobalEnv
	for (aSym in syms) {
		newVal <- get(aSym, envir=parEnv, inherits=FALSE)
		oldVal <- tryCatch({get(aSym, envir=oldVariableValues, inherits=FALSE)},
			error = function(e) {})
		if (is.null(oldVal) || !identical(newVal, get(aSym, envir=oldVariableValues))) {
			deltaSyms <- append(deltaSyms, aSym)
		}
		assign(aSym, newVal, envir=oldVariableValues)
	}
	#remove any symbols that were rm()'d, after saving their names
	goneSyms = setdiff(oldSyms, syms)
	if (length(goneSyms) < 1) { 
		goneSyms <- NULL
	} else {
		goneSyms = as.character(goneSyms)
	}
	tryCatch({rm(list=goneSyms, envir=oldVariableValues)}, error = function(e) {})
	list(syms=deltaSyms, gone=goneSyms)
}

#called by rcsession.m
rc2.clearEnvironment <- function() {
	rm(list=ls(envir=.GlobalEnv), envir=.GlobalEnv)
	list(rc2type = TRUE)
}

#returns a subvariable
rc2.sublistValue <- function(identifier) {
	args = c()
	baseName <- gsub('\\[.*', '\\1', identifier)
	if (identical(TRUE, as.logical(grep('\\[\\d+\\]', identifier)))) {
		mdata = gregexpr('\\[\\d+\\]', identifier, perl=TRUE)
		i = 1
		while (i <= length(mdata[1][[1]])) {
			start <- mdata[1][[1]][i] + 1
			len <- attr(mdata[1][[1]], "match.length")[1] - 2
			thisArg <- substr(identifier, start, len - 1 + start)
			args <- c(args, thisArg)
			i <- i + 1
		}
	}
	results <- .Call("Rc2_SublistValue", c(baseName), args)
}

#main function to show variables
rc2.listVariables <- function(delta=FALSE, symbols=ls(envir=.GlobalEnv)) {
	syms <- symbols
#	syms <- ls(envir=.GlobalEnv)
	delSyms <- NULL
	if (delta) {
		dlist <- determineDeltaSymbols(syms)
		syms <- dlist[["syms"]]
		delSyms <- dlist[["gone"]]
		#if empty, short-circuit the call to the native library
		if (length(syms) < 1 && length(delSyms) < 1) { return("{}") }
	} else {
		tryCatch({rm(list=ls(all=TRUE, envir=oldVariableValues), envir=oldVariableValues)}, error = function(e) {})
	}
	if (length(delSyms) < 1) { delSyms <- NULL }
	results <- .Call("Rc2_ListVariables", syms, delSyms)
	if (!delta) {
		for (aSym in syms) {
			tryCatch({assign(aSym, get(aSym, envir=.GlobalEnv), envir=oldVariableValues) },
				error = function(e) {})
		}
	}
	if (!is.list(results) && length(results) < 1)
		return(list());
	invisible(results)
}

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
