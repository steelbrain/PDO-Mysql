<?hh

require(__DIR__.'/../src/autoload.hh');

use steelbrain\MySQL;

function runTests() {
  $Connection = MySQL::create(shape(
    'Host' => 'localhost',
    'Name' => 'test',
    'User' => 'root',
    'Pass' => ''
  ));
  describe('PDO-MySQL', function() use ($Connection) {
    describe('query', function() use ($Connection) {
      it('executes a statement and returns a response', function() use ($Connection) {
        $query = $Connection->query('Select 1 as a');
        $result = $query->fetch(PDO::FETCH_ASSOC);
        expect($query instanceof PDOStatement)->toBe(true);
        expect($result)->toBe(['a' => '1']);
      });
      it('accepts parameters for that query!', function() use ($Connection) {
        $query = $Connection->query('Select :a as a', ['a' => 2]);
        $result = $query->fetch(PDO::FETCH_ASSOC);
        expect($result)->toBe(['a' => '2']);
      });
    });
    describe('MySQL Query', function() use ($Connection) {
      describe('get', function() use ($Connection) {
        $result = $Connection->from('test')->select('name')-> get();
        expect($result)->toBe(['name' => 'Wow']);
      });
      describe('getAll', function() use ($Connection) {
        $result = $Connection->from('test')->select('name')->limit(2)->getAll();
        expect($result)->toBe([['name' => 'Wow'], ['name' => 'Hey']]);
      });
      describe('exists', function() use ($Connection) {
        $result = $Connection->from('test')->select('name')->where('name = "Wow"')->exists();
        expect($result)->toBe(true);
        $result = $Connection->from('test')->select('name')->where('name = "Beep"')->exists();
        expect($result)->toBe(false);
      });
    });
    describe('insert && MySQLQuery::{update, delete}', function() use ($Connection) {
      $result = $Connection->from('test')->select('name')->where('name = "Beep"')->exists();
      expect($result)->toBe(false);

      $Connection->insert('test', ['name' => 'Beep']);

      $result = $Connection->from('test')->select('name')->where('name = "Beep"')->exists();
      expect($result)->toBe(true);

      $Connection->from('test')->where('name = "Beep"')->update('name = "haha"');

      $result = $Connection->from('test')->select('name')->where('name = "haha"')->exists();
      expect($result)->toBe(true);

      $Connection->from('test')->where('name = "Beep"')->delete();
      $result = $Connection->from('test')->select('name')->where('name = "Beep"')->exists();
      expect($result)->toBe(false);
    });
  });
}

runTests();
