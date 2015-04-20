# basics

All messages are sent as json. Every message to the server must contain a "msg" string and in most cases and "argument" string.  Acceptable msg values are:

* open

* close

* clearFileChanges

* execScript

* execFile

* help

* listVariables

* getVariable

* toggleVariableWatch

