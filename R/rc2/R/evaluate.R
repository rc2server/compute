rc2.evaluateCode <- function(src, parent = emptyenv(), baseImageName = "previewImage") {
  wrapper <- evaluateWrapper(parent = parent)
  wrapper$evaluate(src, baseImageName)
}


evaluateWrapper <- function(parent = emptyenv()) {
  myEnv <- parent
  assign("allItems", list(), envir = myEnv)

  convertItem <- function(item) {
    switch(class(item),
           "rc2src" = list(type = "source", src = item$data$src),
           "rc2plot" = list(type = "plot", filename = attr(item, "filename")),
           "rc2msg" = list(type = "message", message = item$data$message),
           "rc2err" = list(type = "error", message = item$data$message, call = item$data$call),
    )
  }

  rc2evaluate <- function(src, baseImageName = "previewImage") {
    assign("allItems", list(), envir = myEnv)
    on.exit(rm(allItems))
    eres <- evaluate(src, envir = myEnv, output_handler = outputHandler, stop_on_error = 1L)
    all <- get("allItems", envir = myEnv)
    all2 <- processItems(all, baseImageName)
    rm("allItems")
    return(all2)
  }

    outputHandler <- new_output_handler(
    source=function(value, visible = TRUE) {
      all = get("allItems", envir = myEnv)
      newval <- list(src=value[[1]][1])
      class(newval) <- "rc2src"
      all[[length(all) + 1]] = newval
      assign("allItems", all, envir = myEnv)
      invisible(NULL)
    },
    text=function(txt) {
    },
    message=function(msg) {
      all = get("allItems", envir = myEnv)
      newval <- list(data=msg)
      class(newval) <- "rc2msg"
      all[[length(all) + 1]] = newval
      assign("allItems", all, envir = myEnv)

      invisible(NULL)
    },
    warning=function(msg) {
      invisible(NULL)
    },
    error=function(msg) {
      all = get("allItems", envir = myEnv)
      newval <- list(data=msg)
      class(newval) <- "rc2err"
      all[[length(all) + 1]] = newval
      assign("allItems", all, envir = myEnv)

      invisible(NULL)
    },
    graphics = function(data) {
      all = get("allItems", envir = myEnv)
      newval <- structure(list(img = data), class = "rc2plot")
      all[[length(all) + 1]] = newval
      assign("allItems", all, envir = myEnv)

      invisible(NULL)
    },
    value=function(value, visible = TRUE) {
      # was getting calls with NULL, FALSE because funcs return invisible(NULL). now skip that
      if(visible == FALSE) { return(invisible(NULL)) }
      vals <- list(data=value, visible=visible)
      class(vals) <- "valuePair"
      all = get("allItems", envir = myEnv)
      newval <- structure(list(vals), myclass="val")
      class(newval) <- "rc2value"
      all[[length(all) + 1]] = newval
      assign("allItems", all, envir = myEnv)

      invisible(NULL)
    })
  saveImage <- function(img, name, ...) {
    png(name, ...)
    replayPlot(img)
    dev.off()
  }
  structure(list(evaluate=rc2evaluate, saveImage=saveImage))
}
