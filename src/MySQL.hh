<?hh //strict

namespace steelbrain;
use PDO;
use PDOStatement;

type MySQLconfig = shape(
  'User' => string,
  'Host' => string,
  'Name' => string,
  'Pass' => string
);

class MySQL {
  public int $lastInsertedId = 0;
  public function __construct(public PDO $link) {}
  public function query(string $statement, ?array<string, mixed> $parameters = null): PDOStatement {
    $query = $this->link->prepare($statement);
    if ($parameters !== null) {
      $query->execute($parameters);
    } else {
      $query->execute();
    }
    return $query;
  }
  public function insert(string $table, array<string, mixed> $paramters): int {
    $keys = [];
    $values = [];
    $placeholders = [];

    foreach($paramters as $key => $value) {
      $keys[] = '`'.$key.'`';
      $values[":$key"] = $value;
      $placeholders[] = ":$key";
    }

    $keys = implode(', ', $keys);
    $placeholders = implode(', ', $placeholders);

    $this->query("Insert into $table ($keys) VALUES ($placeholders)", $values);

    return $this->lastInsertedId = (int) $this->link->lastInsertId();
  }
  public function from<T>(string $table): MySQLQuery<T> {
    return (new MySQLQuery($this))->from($table);
  }

  public static function create(MySQLconfig $config, string $charset = 'utf8mb4'): MySQL {
    $link = new PDO("mysql:charset=$charset;host=$config[Host];dbname=$config[Name]", $config['User'], $config['Pass'], [
      PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION
    ]);
    return new MySQL($link);
  }
}
