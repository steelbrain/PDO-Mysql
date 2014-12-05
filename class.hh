<?hh
/**
 * @Package PDO-Mysql
 * @License SEE THE LICENSE FILE
 */
type DataBaseConfig = shape('host' => string,'user' => string,'db' => string,'pass' => string);
class DataBase{
    /**
    * @var PDO
    */
    public static ?PDO $link = null;
    public static int $lastInserted = 0;
    public static DataBaseConfig $config = shape('host' => 'localhost','user' => 'root','db' => '','pass' => '');
    public function __construct(?DataBaseConfig $config = null){
        if($config !== null){
            self::$config = $config;
        }
        self::__init();
    }
    public static function init(?DataBaseConfig $config = null):void{
        if($config !== null){
            self::$config = $config;
        }
        self::__init();
    }
    public static function __preinit():void{
        if(defined('DB_HOST')){
            self::$config['host'] = DB_HOST;
        }
        if(defined('DB_USER')){
            self::$config['user'] = DB_USER;
        }
        if(defined('DB_NAME')){
            self::$config['db'] = DB_NAME;
        }
        if(defined('DB_PASS')){
            self::$config['pass'] = DB_PASS;
        }
    }
    private static function __init():void{
        self::$link = new PDO('mysql:host='.self::$config['host'].';dbname='.self::$config['db'],self::$config['user'],self::$config['pass'],[PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8']);
        self::$link->setAttribute(PDO::ATTR_ERRMODE,PDO::ERRMODE_EXCEPTION);
    }
    public static function query(string $q,?array<string,string> $args = null):PDOStatement{
          if(self::$link === null){
               self::init();
          }
          if($args === null)$args = [];
          //UNSAFE -- HHVM doesn't support awesomeness at the moment see https://github.com/facebook/hhvm/issues/4243
          $q = self::$link->prepare($q);
          $q->execute($args);
          return $q;
     }
     public static function ping():void{
          try {
               self::query("Select 1",null);
          } catch(PDOException $e){
               self::__init();
          }
     }
     public static function fetch(string $q, ?array<string,string> $args = null):array<string,string>{
          $ret = self::query($q,$args)->fetch(PDO::FETCH_ASSOC);
          if(count($ret) == 0)return [];
          return $ret;
     }
     public static function fetchCol(string $q, ?array<string,string> $args = null):string{
          $query = self::query($q,$args)->fetch(PDO::FETCH_NUM);
          return strlen($query) > 0 ? $query[0] : '';
     }
     public static function fetchAll(string $q, ?array<string,string> $args = null):array<array<string,string>>{
          $ret = self::query($q,$args)->fetchAll(PDO::FETCH_ASSOC);
          if(count($ret) == 0)return [];
          return $ret;
     }
     public static function exists(string $q, ?array<string,string> $args = null):bool{
          return (bool) self::query("Select 1 from $q LIMIT 1",$args)->rowCount();
     }
     public static function insert(string $table,array<string,mixed>$args):int{
        // UNSAFE -- See https://github.com/facebook/hhvm/issues/4243
        if(self::$link === null)
            self::init();
        $keys = $values = $params = [];
        foreach($args as $key => &$value){
        if(is_array($value))$value = json_encode($value);
            $params[":$key"] = $value;
            $keys[] = $key;
            $values[] = ":$key";
        }
        $keys = implode(',',$keys);
        $values = implode(',',$values);
        self::query("Insert into $table ($keys) VALUES ($values)",$params);
        $last_id = self::$link->lastInsertId();
        self::$lastInserted = $last_id;
        return $last_id;
     }
};
DataBase::__preinit();
