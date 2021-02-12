# Basics
All messages are JSON. Every message to the server must contain a `msg` string and, in most cases, an `argument` string.  Acceptable `msg` values are:

#### Rc2Appserver `msg` to Rc2Compute
* `open`
* `close`
* `saveEnv`
* `execScript`
* `execAndReturn`
* `execFile`
* `help`
* `listVariables`
* `getVariable`
* `toggleVariableWatch`
* `clearEnvironment`
* `createEnvironment`
* `initPreview`
* `updatePreview`
* `removePreview`

#### Rc2Compute `msg` to Rc2Appserver

* `openresponse`
* `execComplete`
* `results`
* `showoutput`
* `variableupdate`
* `variablevalue`
* `help`
* `error`
* `envCreated`
* `previewInited`
* `previewUpdated`
* `previewUpdateStarted`