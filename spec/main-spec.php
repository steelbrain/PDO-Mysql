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
    describe('fetch', function() use ($Connection) {
      $query = $Connection->fetch('Select name from test where id = 1 LIMIT 1');
      expect($query)->toBe(['name' => 'Wow']);
    });
    describe('fetchAll', function() use ($Connection) {
      $query = $Connection->fetchAll('Select name from test');
      expect($query)->toBe([['name' => 'Wow'], ['name' => 'Hey']]);
    });
    describe('fetchCol', function() use ($Connection) {
      $query = $Connection->fetchCol('Select name from test LIMIT 1');
      expect($query)->toBe('Wow');
    });
    describe('exists', function() use ($Connection) {
      $query = $Connection->exists('Select 1 from test where name = "Wow" LIMIT 1');
      expect($query)->toBe(true);
      $query = $Connection->exists('Select 1 from test where name = "Beep" LIMIT 1');
      expect($query)->toBe(false);
    });
    describe('insert', function() use ($Connection) {
      $query = $Connection->exists('Select 1 from test where name = "Beep" LIMIT 1');
      expect($query)->toBe(false);
      $Connection->insert('test', ['name' => 'Beep']);
      $query = $Connection->exists('Select 1 from test where name = "Beep" LIMIT 1');
      expect($query)->toBe(true);
      $Connection->query('Delete from test where name = "Beep"');
    });
  });
}

runTests();
