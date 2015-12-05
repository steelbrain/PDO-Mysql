<?hh //strict

namespace steelbrain;
use PDO;

type MySQLconfig = shape(
  'User' => string,
  'Host' => string,
  'Name' => string,
  'Pass' => string
);

class MySQL {
  public int $lastInsertedId = 0;
  public function __construct(public PDO $link) {}
  public static function create(MySQLconfig $config, string $charset = 'utf8mb4'): MySQL {
    $link = new PDO("mysql:charset=$charset;host=$config[Host];dbname=$config[Name]", $config['User'], $config['Pass'], [
      PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION
    ]);
    return new MySQL($link);
  }
}
