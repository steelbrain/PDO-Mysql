PDO-Mysql
===========
PDO-Mysql is a PDO wrapper for HackLang, it's guranteed to make your life easier. It saves you from re-inventing the wheel.

#### API
```php
<?php
namespace steelbrain;
class MySQL {
  public int $lastInsertedId = 0
  public function __construct(PDO $link)
  public function insert(string $table, array<string, mixed> $parameters): int
  public function query(string $statement, array<string, mixed> $parameters): PDOStatement
  public function from(string $tableName): MySQLQuery

  public static function create(shape(
    'User' => string,
    'Host' => string,
    'Pass' => string,
    'Name' => string
    ) $config, string $charset = 'utf8mb4'
  ): MySQL
}
class MySQLQuery<T> {
  public function select(string | Array<string> $columns): this
  public function where(string | Array<string, mixed> $clause): this
  public function limit(int $limit):t his
  public function exists(): bool
  public function get(): ?T
  public function getAll(): Array<T>

}
```

#### Example Usage
```php
<php
use steelbrain\MySQL;
require('/path/to/pdo-mysql/src/autoload.hh');
$db = MySQL::create(shape(...));
$db->from('table')->where(['a' => 1])->delete();
$db->from('table')->where(['b' => 'c'])->exists();
$db->from('table')->select(['name', 'email'])->where(['id' => 1])->get()
$db->from('table')->select(['name', 'email'])->where(['id' => 1])->getAll()
```

#### License
This project is licensed under the terms of MIT License
