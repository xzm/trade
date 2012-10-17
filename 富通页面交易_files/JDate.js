
// Provide a default path to dwr.engine
if (dwr == null) var dwr = {};
if (dwr.engine == null) dwr.engine = {};
if (DWREngine == null) var DWREngine = dwr.engine;

if (JDate == null) var JDate = {};
JDate._path = '/webtrade/dwr';
JDate.getMonth = function(callback) {
  dwr.engine._execute(JDate._path, 'JDate', 'getMonth', callback);
}
JDate.getHours = function(callback) {
  dwr.engine._execute(JDate._path, 'JDate', 'getHours', callback);
}
JDate.getMinutes = function(callback) {
  dwr.engine._execute(JDate._path, 'JDate', 'getMinutes', callback);
}
JDate.getSeconds = function(callback) {
  dwr.engine._execute(JDate._path, 'JDate', 'getSeconds', callback);
}
JDate.toString = function(callback) {
  dwr.engine._execute(JDate._path, 'JDate', 'toString', callback);
}
