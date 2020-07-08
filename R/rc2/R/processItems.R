unique_filename <- function(baseName, extension) {
  if(file.exists(baseName)) { return(paste0(baseName, extension, sep = '.')) }
  cnt <- 1L
  repeat {
    nextName <- sprintf("%s%03d.%s", baseName, cnt, extension)
    cnt <- cnt + 1
    if(!file.exists(nextName)) { return(nextName) }
  }
}

handleImage <- function(item, baseFileName) {
  if(class(item) != "rc2plot") { return("") }
  fname <- unique_filename(baseFileName, "png")
  png(fname)
  replayPlot(item[[1]])
  dev.off()
  fname
}

handlePlot <- function(item, baseFileName) {
  fname <- handleImage(item, baseFileName)
  return(structure(list(fname = fname), class = "rc2plot"))
}

handleValue <- function(item) {
  tc <- textConnection("valueStr", open="w", local=TRUE)
  sink(tc, append=TRUE)
  print(item[[1]]$data)
  sink()
  close(tc)
  valueStr <- paste0(valueStr, sep="", collapse="\n")
  item[[1]][["str"]] <- valueStr
  return(structure(item[[1]], class = "rc2value"))
}

handleError <- function(item, className) {
  newlist = list(message = item$data$message, call = as.character(as.expression(item$data$call)))
  return( structure(newlist, class = className))
}

processItems <- function(items, baseFileName) {
  newlist <- list()
  for(i in 1:length(items)) {
    curItem <- items[[i]]
    listitem <- switch(class(curItem),
      rc2plot = handlePlot(curItem, baseFileName = baseFileName),
      rc2src = structure(list(src = curItem$src), class="rc2src") ,
      rc2value = handleValue(curItem),
      rc2msg = handleError(curItem, "rc2msg"),
      rc2err = handleError(curItem, "rc2err")
    )
    newlist[[length(newlist) + 1]] = listitem
  }
  return(newlist)
}
