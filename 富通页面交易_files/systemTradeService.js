
// Provide a default path to dwr.engine
if (dwr == null) var dwr = {};
if (dwr.engine == null) dwr.engine = {};
if (DWREngine == null) var DWREngine = dwr.engine;

if (systemTradeService == null) var systemTradeService = {};
systemTradeService._path = '/webtrade/dwr';
systemTradeService.getFundAvl = function(p0, callback) {
  dwr.engine._execute(systemTradeService._path, 'systemTradeService', 'getFundAvl', p0, callback);
}
systemTradeService.getStockNumber = function(p0, p1, p2, callback) {
  dwr.engine._execute(systemTradeService._path, 'systemTradeService', 'getStockNumber', p0, p1, p2, callback);
}
systemTradeService.getStockNumberCredit = function(p0, p1, p2, p3, callback) {
  dwr.engine._execute(systemTradeService._path, 'systemTradeService', 'getStockNumberCredit', p0, p1, p2, p3, callback);
}
systemTradeService.getOrgList = function(p0, callback) {
  dwr.engine._execute(systemTradeService._path, 'systemTradeService', 'getOrgList', p0, callback);
}
