library(inline)
local( {
myenv <- new.env(parent = emptyenv())
oldtitle <- graphics::title
#assign("oldtitle", graphics::title, envir = myenv)

#oldtitle <- title
mytitle <- function(...) {
	args <- list(...)
	if (!is.null(args$main)) { 
		rc2.errlog("rc2.imgtitle=%s\n", args$main)
	}
	oldtitle(...)
}

inc <- '
/* This is taken from envir.c in the R 2.15.1 source 
   https://github.com/SurajGupta/r-source/blob/master/src/main/envir.c
*/
#define FRAME_LOCK_MASK (1<<14)
#define FRAME_IS_LOCKED(e) (ENVFLAGS(e) & FRAME_LOCK_MASK)
#define UNLOCK_FRAME(e) SET_ENVFLAGS(e, ENVFLAGS(e) & (~ FRAME_LOCK_MASK))
'

src <- '
  if (TYPEOF(env) == NILSXP)
    error("use of NULL environment is defunct");
  if (TYPEOF(env) != ENVSXP)
    error("not an environment");
  UNLOCK_FRAME(env);
  // Return TRUE if unlocked; FALSE otherwise
  SEXP result = PROTECT( Rf_allocVector(LGLSXP, 1) );
  LOGICAL(result)[0] = FRAME_IS_LOCKED(env) == 0;
  UNPROTECT(1);
  return result;
'

unlockEnvironment <- cfunction(signature(env = "environment"),
                        includes = inc,
                        body = src)


ns_g <- asNamespace('graphics')
unlockEnvironment(ns_g)
unlockBinding("title", ns_g)
ns_g$title <- mytitle
lockBinding("title", ns_g)
detach(package:graphics)
library(graphics)

})
