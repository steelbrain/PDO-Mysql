<?hh

spl_autoload_register(function($class) {
  if ($class === 'steelbrain\\MySQL') {
    require(__DIR__.'/MySQL.hh');
  } else if ($class === 'steelbrain\\MySQLQuery') {
    require(__DIR__.'/MySQLQuery.hh');
  }
});
